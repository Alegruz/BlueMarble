module;

#include "pch.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

export module Game;

import Bank;
import Board;
import Player;
#if defined(BM_USE_SFML)
import Renderer;
#endif	// defined(BM_USE_SFML)
import Tile;

namespace bm
{
	export class Game final
	{
	public:
		explicit Game(const size_t numPlayers) noexcept
			: mBoard()
			, mNumPlayers(numPlayers)
			, mPlayers
			{
				Player(0, Bank::MAX_BALANCE / numPlayers),
				Player(1, Bank::MAX_BALANCE / numPlayers),
				Player(2, Bank::MAX_BALANCE / numPlayers),
				Player(3, Bank::MAX_BALANCE / numPlayers),
			}
			, mSocialWelfareFund(0)
#if defined(BM_USE_SFML)
			, mWindow(sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), "BlueMarble", sf::Style::Default)
			, mRenderer(mWindow, mBoard)
#endif	// defined(BM_USE_SFML)
		{
			for (size_t playerIndex = 0; playerIndex < mNumPlayers; ++playerIndex)
			{
				mPlayers[playerIndex].MoveTo(0);
			}
#if defined(BM_USE_SFML)
			mWindow.setFramerateLimit(60);
#endif	// defined(BM_USE_SFML)
		}

		void Run() noexcept
		{
			initialize();

#if defined(BM_USE_SFML)
			sf::Clock clock;
			size_t currentPlayerIndex = 0;
			while (mWindow.isOpen())
			{
				sf::Time elapsedTime = clock.restart();
				processInput();
				Player& currentPlayer = mPlayers[currentPlayerIndex];

				update(elapsedTime.asSeconds(), currentPlayer.GetCurrentTileIndex());
				mRenderer.Render();
			}
#else	// NOT defined(BM_USE_SFML)
			size_t currentNumPlayers = mNumPlayers;
			bool bIsGameDone = false;
			while (bIsGameDone == false)
			{
				update(currentNumPlayers, bIsGameDone);
			}
#endif	// defined(BM_USE_SFML)
		}

	private:
		static constexpr const size_t MAX_CONSOLE_LINE_LENGTH = 128;
		static const std::string CONSOLE_HEADER;

	private:
		void initialize() noexcept
		{
			srand(static_cast<uint32_t>(time(nullptr)));

#if defined(BM_USE_SFML)
			for (size_t tileIndex = 0; tileIndex < Tile::COUNT; ++tileIndex)
			{
				Tile& tile = mBoard.GetTile(tileIndex);
				mRenderer.AddTileRenderData(tile, tileIndex);
			}
#endif	// defined(BM_USE_SFML)
		}

		void processInput() noexcept
		{
#if defined(BM_USE_SFML)
			sf::Event event;
			while (mWindow.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					mWindow.close();
				}
			}
#endif	// defined(BM_USE_SFML)
		}

		void update(
#if defined(BM_USE_SFML)
			const float deltaTime, const size_t currentTileIndex
#else	// NOT defined(BM_USE_SFML)
			size_t& currentNumPlayers
			, bool& bIsGameDone
#endif	// defined(BM_USE_SFML)
		) noexcept
		{
#if defined(BM_USE_SFML)
			mRenderer.Update(deltaTime, currentTileIndex);
#else	// NOT defined(BM_USE_SFML)
			for (size_t playerIndex = 0; playerIndex < mNumPlayers; ++playerIndex)
			{

				Player& currentPlayer = mPlayers[playerIndex];
				if (currentPlayer.IsAlive() == false)
				{
					continue;
				}

				std::string playerStr = "Player[";
				const char playerIndexChar = static_cast<char>(playerIndex) + '0';
				playerStr.append(&playerIndexChar, 1);
				playerStr.append("]");

				const std::string header = "Current Player: " + playerStr + " ";
				logHeader(header);

				// check state
				if (currentNumPlayers == 1)
				{
					logMessage(playerStr + " Won!!");
					logFooter();
					bIsGameDone = true;
					break;
				}

				// perform pre-input action
				Tile& preInputTile = mBoard.GetTile(currentPlayer.GetCurrentTileIndex());
				processTilePreInput(preInputTile, currentPlayer);

				const size_t remainingNoMansLandTurns = currentPlayer.GetRemainingNoMansLandTurns();
				if (remainingNoMansLandTurns > 0)
				{
					logMessage("Roll double to escape from the No Man's Land!!");
					std::stringstream ss;
					ss << "Remaining turns to escape: " << remainingNoMansLandTurns;
					logMessage(ss.str());
				}

				// ask player input
				size_t numTilesToMove = 0;
				bool isDoubleDie = false;
				do
				{
					const size_t dice0 = rollDice();
					const size_t dice1 = rollDice();
					isDoubleDie = dice0 == dice1;
					numTilesToMove += (dice0 + dice1);

					if (isDoubleDie == false)
					{
						break;
					}
					else if (remainingNoMansLandTurns > 0)
					{
						logMessage("Double Die!! You have escaped No Mans Land!! Rolling once more!!");
						currentPlayer.ExitNoMansLand();
						numTilesToMove = 0;
					}
					else
					{
						logMessage("Double Die!! Rolling once more!!");
					}
				} while (true);

				// perform post-input action
				currentPlayer.Update();
				if (remainingNoMansLandTurns > 0)
				{
					logMessage("You have failed to escape the No Man's Land!!");
					std::stringstream ss;
					ss << "Remaining turns to escape: " << currentPlayer.GetRemainingNoMansLandTurns();
					logMessage(ss.str());
				}
				else
				{
					const size_t prevTileIndex = currentPlayer.GetCurrentTileIndex();
					const size_t postInputTileIndex = currentPlayer.MoveBy(numTilesToMove);

					const size_t numCycles = ((numTilesToMove + prevTileIndex) / Tile::COUNT);
					for (size_t cycleIndex = (postInputTileIndex == 0); cycleIndex < numCycles; ++cycleIndex)
					{
						logMessage("You have crossed the starting line!! You have earned your 200,000 KRW salaray!!");
						currentPlayer.Deposit(200'000);
					}

					Tile& postInputTile = mBoard.GetTile(postInputTileIndex);
					const bool bResult = processTilePostInput(postInputTile, currentPlayer);
					if (bResult == false)
					{
						logMessage(playerStr + " has bankrupted!!");
						currentPlayer.GoBankrupt();
						--currentNumPlayers;
					}
				}

				logFooter();
			}
#endif	// defined(BM_USE_SFML)
		}

		void logHeader(const std::string& message) noexcept
		{
			const size_t remainingSpaces = MAX_CONSOLE_LINE_LENGTH - CONSOLE_HEADER.length() - message.length();
			const std::string asterisks(remainingSpaces, '*');
			std::cout << CONSOLE_HEADER << message << asterisks << '\n';
		}

		void logMessage(const std::string& message) noexcept
		{
			const size_t remainingSpaces = MAX_CONSOLE_LINE_LENGTH - CONSOLE_HEADER.length() - message.length();
			const std::string remainingWhiteSpaces(remainingSpaces - 1, ' ');
			std::cout << CONSOLE_HEADER << message << remainingWhiteSpaces << "*\n";
		}

		void logFooter() noexcept
		{
			static const std::string asterisks(MAX_CONSOLE_LINE_LENGTH, '*');
			std::cout << asterisks << '\n';
		}

		inline size_t rollDice() noexcept
		{
			std::string input;
			std::string trash;
			while (std::cin.eof() == false)
			{
				logMessage("Press r to roll a dice...");
				std::cin >> input;
				if (std::cin.fail() == true)
				{
					std::cin.clear();
					std::cin >> trash;
				}
				else
				{
					if (input.size() == 1 && input[0] == 'r')
					{
						break;
					}
				}
			}
			const size_t dice = (rand() % 6) + 1;
			std::stringstream ss;
			ss << "You rolled " << dice << "!!";
			logMessage(ss.str());

			return dice;
		}

		bool processCityPreInput(Tile& tile, Player& player) noexcept
		{
			const size_t tileOwnerPlayerIndex = tile.GetOwnerPlayerIndex();
			const size_t cityIndex = tile.GetIndex();

			City& city = mBoard.GetCity(cityIndex);

			if (tileOwnerPlayerIndex == player.GetIndex())
			{
				// Construct
				constexpr const size_t NUM_AVAILABLE_CHOICES = static_cast<size_t>(City::eOwnerChoice::COUNT);
				const bool availableChoices[NUM_AVAILABLE_CHOICES] =
				{
					true,
					city.GetNumVillas() < City::MAX_NUM_VILLAS,
					city.HasBuilding() == false,
					city.HasHotel() == false,
				};
				bool bHasNoAvailableChoice = true;
				for (size_t availableChoiceIndex = 1; availableChoiceIndex < NUM_AVAILABLE_CHOICES; ++availableChoiceIndex)
				{
					if (availableChoices[availableChoiceIndex] == true)
					{
						bHasNoAvailableChoice = false;
						break;
					}
				}

				std::string playerStr = "Player[";
				const char playerIndexChar = static_cast<char>(player.GetIndex()) + '0';
				playerStr.append(&playerIndexChar, 1);
				playerStr.append("]");

				logMessage("You(" + playerStr + ") are the owner of the city!!");

				if (bHasNoAvailableChoice)
				{
					logMessage("There is nothing you can do right now. Skipping turn...");
				}
				else
				{
					logMessage("What do you want to do?");
					logMessage("0: Do Nothing");

					if (availableChoices[static_cast<size_t>(City::eOwnerChoice::CONSTRUCT_VILLA)])
					{
						logMessage("1: Construct a Villa");
					}

					if (availableChoices[static_cast<size_t>(City::eOwnerChoice::CONSTRUCT_BUILDING)])
					{
						logMessage("2: Construct a Building");
					}

					if (availableChoices[static_cast<size_t>(City::eOwnerChoice::CONSTRUCT_HOTEL)])
					{
						logMessage("3: Construct a Hotel");
					}

					City::eOwnerChoice choice = City::eOwnerChoice::COUNT;
					size_t choiceNumber = INVALID_SIZE;
					std::string trash;
					while (std::cin.eof() == false)
					{
						std::cin >> choiceNumber;
						if (std::cin.fail() == true)
						{
							std::cin.clear();
							std::cin >> trash;
						}
						else
						{
							if (choiceNumber < static_cast<size_t>(City::eOwnerChoice::COUNT) && availableChoices[choiceNumber])
							{
								choice = static_cast<City::eOwnerChoice>(choiceNumber);
								break;
							}
						}
					}

					switch (choice)
					{
					case City::eOwnerChoice::DO_NOTHING:
						break;
					case City::eOwnerChoice::CONSTRUCT_VILLA:
					{
						city.ConstructVilla();
						const size_t cost = City::CITY_CONSTRUCTION_FEES[cityIndex].Villa;
						player.Withdraw(cost);
						{
							std::stringstream ss;
							ss << "Constructed a villa!! Cost: " << cost << " Total villas: " << city.GetNumVillas();
							logMessage(ss.str());
						}

						{
							std::stringstream ss;
							ss << "Remaining balance: " << player.GetBalance();
							logMessage(ss.str());
						}
					}
					break;
					case City::eOwnerChoice::CONSTRUCT_BUILDING:
					{
						city.ConstructBuilding();
						const size_t cost = City::CITY_CONSTRUCTION_FEES[cityIndex].Building;
						player.Withdraw(cost);

						{
							std::stringstream ss;
							ss << "Constructed a building!! Cost: " << cost;
							logMessage(ss.str());
						}

						{
							std::stringstream ss;
							ss << "Remaining balance: " << player.GetBalance();
							logMessage(ss.str());
						}
					}
					break;
					case City::eOwnerChoice::CONSTRUCT_HOTEL:
					{
						city.ConstructHotel();
						const size_t cost = City::CITY_CONSTRUCTION_FEES[cityIndex].Hotel;
						player.Withdraw(cost);

						{
							std::stringstream ss;
							ss << "Constructed a hotel!! Cost: " << cost;
							logMessage(ss.str());
						}

						{
							std::stringstream ss;
							ss << "Remaining balance: " << player.GetBalance();
							logMessage(ss.str());
						}
					}
					break;
					case City::eOwnerChoice::COUNT:
						[[fallthrough]];
					default:
						assert(false);
						break;
					}
				}
			}

			return true;
		}

		bool processCityPostInput(Tile& tile, Player& player) noexcept
		{
			const size_t tileOwnerPlayerIndex = tile.GetOwnerPlayerIndex();
			const size_t cityIndex = tile.GetIndex();

			City& city = mBoard.GetCity(cityIndex);
			const std::string& cityName = city.GetName();

			std::string welcomeMessage = "Welcome to " + cityName + "!!";
			logMessage(welcomeMessage);
			if (tileOwnerPlayerIndex == INVALID_SIZE)
			{
				// Buy
				const size_t landFee = City::CITY_LAND_ACQUIRE_FEES[cityIndex];
				std::stringstream ss;
				ss << "Press b to buy the land for " << landFee << " KRW, s to skip...";
				const std::string inputMessage = ss.str();

				char choice = 0;
				std::string input;
				std::string trash;
				while (std::cin.eof() == false)
				{
					logMessage(inputMessage);
					std::cin >> input;
					if (std::cin.fail() == true)
					{
						std::cin.clear();
						std::cin >> trash;
					}
					else
					{
						if (choice = input[0]; input.size() == 1 && (input[0] == 'b' || input[0] == 's'))
						{
							break;
						}
					}
				}

				if (choice == 'b')
				{
					const bool bResult = player.BuyCity(tile, city);
					if (bResult == false)
					{
						ss.str("");
						ss.clear();
						ss << "You have not enough balance(" << player.GetBalance() << ") to buy " << cityName << "(" << City::CITY_LAND_ACQUIRE_FEES[cityIndex] << ")!!";
						logMessage(ss.str());
						return false;
					}

					{
						ss.str("");
						ss.clear();
						ss << "You have bought " << cityName << " for " << City::CITY_LAND_ACQUIRE_FEES[cityIndex] << "!! "
							<< "Remaining balance: " << player.GetBalance();
						logMessage(ss.str());
					}
				}
				else if (choice == 's')
				{
					logMessage("You have chosen not to buy the land!!");
				}
				else
				{
					assert(false);
				}
			}
			else if (Player& tileOwner = mPlayers[tileOwnerPlayerIndex]; tileOwner.IsAlive() == true)
			{
				// Pay
				const City::CityUsageFee& usageFee = City::CITY_USAGE_FEES[cityIndex];

				size_t totalUsageFee = usageFee.Land + city.HasBuilding() * usageFee.Building + city.HasHotel() * usageFee.Hotel;
				const size_t numVillas = city.GetNumVillas();
				if (numVillas == 1)
				{
					totalUsageFee += usageFee.Villa;
				}
				else if (numVillas == 2)
				{
					totalUsageFee += usageFee.DoubleVilla;
				}

				const bool bResult = player.Withdraw(totalUsageFee);
				if (bResult == false)
				{
					std::stringstream ss;
					ss << "You have not enough balance(" << player.GetBalance() << ") to pay " << totalUsageFee << "!!";
					logMessage(ss.str());
					return false;
				}
				tileOwner.Deposit(totalUsageFee);

				std::stringstream ss;
				ss << "You have paid " << totalUsageFee << " to Player[" << tileOwnerPlayerIndex << "]!!\n"
					<< "Remaining balance: " << player.GetBalance();
				logMessage(ss.str());
			}

			return true;
		}

		void processSocialWelfareWithdraw(Player& player) noexcept
		{
			player.Deposit(mSocialWelfareFund);
			mSocialWelfareFund = 0;
		}

		void processBegin(Player& player)
		{
			logMessage("You have at the starting line!! You have earned your 200,000 KRW salaray!!");
			player.Deposit(200'000);
		}

		constexpr bool processTilePreInput(Tile& tile, Player& player) noexcept
		{
			bool bResult = true;

			switch (tile.GetType())
			{
			case Tile::eType::START:
				break;
			case Tile::eType::CITY:
				bResult = processCityPreInput(tile, player);
				break;
			case Tile::eType::SPECIAL_CITY:
				break;
			case Tile::eType::GOLDEN_KEY:
				break;
			case Tile::eType::NO_MANS_LAND:
				break;
			case Tile::eType::SOCIAL_WELFARE_FUND_WITHDRAW:
				break;
			case Tile::eType::SPACE_TRAVEL:
				break;
			case Tile::eType::SOCIAL_WELFARE_FUND_DEPOSIT:
				break;
			case Tile::eType::COUNT:
				[[fallthrough]];
			default:
				assert(false);
				break;
			}

			return bResult;
		}

		constexpr bool processTilePostInput(Tile& tile, Player& player) noexcept
		{
			bool bResult = true;

			switch (tile.GetType())
			{
			case Tile::eType::START:
				processBegin(player);
				break;
			case Tile::eType::CITY:
				bResult = processCityPostInput(tile, player);
				break;
			case Tile::eType::SPECIAL_CITY:
				break;
			case Tile::eType::GOLDEN_KEY:
				processGoldenKeyPostInput(player);
				break;
			case Tile::eType::NO_MANS_LAND:
				player.EnterNoMansLand();
				logMessage("You are caught up in the No Man's Land for 3 turns!!");
				logMessage("Roll a double to escape next turn!!");
				break;
			case Tile::eType::SOCIAL_WELFARE_FUND_WITHDRAW:
				processSocialWelfareWithdraw(player);
				break;
			case Tile::eType::SPACE_TRAVEL:
				break;
			case Tile::eType::SOCIAL_WELFARE_FUND_DEPOSIT:
				break;
			case Tile::eType::COUNT:
				[[fallthrough]];
			default:
				assert(false);
				break;
			}

			return bResult;
		}

		bool processGoldenKeyPostInput(Player& player) noexcept
		{
			logMessage("Golden key!!");

			std::string input;
			std::string trash;
			while (std::cin.eof() == false)
			{
				logMessage("Press g to pick a golden key card!!");
				std::cin >> input;
				if (std::cin.fail() == true)
				{
					std::cin.clear();
					std::cin >> trash;
				}
				else
				{
					if (input.size() == 1 && input[0] == 'g')
					{
						break;
					}
				}
			}

			assert(input[0] == 'g');
			const size_t randomGoldenKeyIndex = rand() % GoldenKey::Index::COUNT;
			player.AddGoldenKey(randomGoldenKeyIndex);
			std::stringstream ss;
			ss << "You picked \"" << GoldenKey::LogName(randomGoldenKeyIndex) << "\" card!!";
			logMessage(ss.str());
			logMessage(GoldenKey::LogDescription(randomGoldenKeyIndex));

			return true;
		}

	private:
		Board mBoard;
		size_t mNumPlayers;
		Player mPlayers[MAX_PLAYER_COUNT];
		size_t mSocialWelfareFund;
#if defined(BM_USE_SFML)
		sf::RenderWindow mWindow;
		Renderer mRenderer;
#endif	// defined(BM_USE_SFML)
	};

	const std::string Game::CONSOLE_HEADER = "* ";
}