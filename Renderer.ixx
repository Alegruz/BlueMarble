module;
#include "pch.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

export module Renderer;

import Board;
import City;
import Tile;

namespace bm
{
	export constexpr const size_t DEFAULT_WINDOW_WIDTH = 1920;
	export constexpr const size_t DEFAULT_WINDOW_HEIGHT = 1080;

	static constexpr const float BOARD_SIZE_RATE = 0.8f;

	export struct Vector2f final
	{
		float X = 0.0f;
		float Y = 0.0f;
	};

	inline constexpr float Dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs) noexcept { return lhs.x * rhs.x + lhs.y + rhs.y; }
	inline float GetSizeSquared(const sf::Vector2f& vec) noexcept { return Dot(vec, vec); }
	inline float GetSize(const sf::Vector2f& vec) noexcept { return std::sqrt(GetSizeSquared(vec)); }
	inline sf::Vector2f Normalize(const sf::Vector2f& vec) noexcept { const float size = GetSize(vec); return size != 0.0f ? vec / GetSize(vec) : vec; }

	inline sf::Vector2f operator*(const sf::Vector2f& lhs, const sf::Vector2f& rhs) noexcept { return sf::Vector2f(lhs.x * rhs.x, lhs.y * rhs.y); }
	inline sf::Vector2f operator/(const sf::Vector2f& lhs, const sf::Vector2f& rhs) noexcept { return sf::Vector2f(lhs.x / rhs.x, lhs.y / rhs.y); }

	class UIElement final
	{
	public:
		enum class eType : uint8_t
		{
			ELEMENT,
			TEXT,
			COUNT,
		};

		struct CreateInfo final
		{
			std::string Id;
			sf::Vector2f Position;
			bool bIsPositionNormalized;
			sf::Vector2f Size;
			bool bIsSizeNormalized;
			sf::Color BackgroundColor;
			sf::Color OutlineColor;
			float OutlineThickness = 0.0f;
			const UIElement* ParentOrNull;
		};

		struct TextCreateInfo final
		{
			std::string Id;
			sf::Vector2f Position;
			bool bIsPositionNormalized;
			sf::Color OutlineColor;
			float OutlineThickness = 0.0f;
			sf::Font Font;
			std::string Text;
			size_t CharacterSizeInPixels;
			sf::Color TextColor;
			sf::Text::Style Style;
			const UIElement* ParentOrNull;
		};

	public:
		explicit UIElement(CreateInfo& createInfo) noexcept
			: UIElement(std::move(createInfo))
		{
		}

		explicit UIElement(CreateInfo&& createInfo) noexcept
			: mId(createInfo.Id)
			, mType(eType::ELEMENT)
			, mShape()
			, mText()
			, mNormalizedPosition()
			, mNormalizedSize()
			, mChildren()
			, mParentOrNull(createInfo.ParentOrNull)
		{
			const sf::Vector2f parentSize = mParentOrNull ? mParentOrNull->mShape.getSize() : sf::Vector2f(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
			const sf::Vector2f parentPosition = mParentOrNull ? mParentOrNull->mShape.getPosition() : sf::Vector2f(0.0f, 0.0f);

			mNormalizedPosition = createInfo.bIsPositionNormalized ? createInfo.Position : createInfo.Position / parentSize;
			mNormalizedSize = createInfo.bIsSizeNormalized ? createInfo.Size : (createInfo.Size / parentSize);

			const sf::Vector2f shapePosition = (createInfo.bIsPositionNormalized ? parentSize * createInfo.Position : createInfo.Position) + parentPosition;
			const sf::Vector2f shapeSize = createInfo.bIsPositionNormalized ? parentSize * createInfo.Size : createInfo.Size;

			mShape.setPosition(shapePosition.x, shapePosition.y);
			mShape.setSize(sf::Vector2f(shapeSize.x, shapeSize.y));
			mShape.setFillColor(createInfo.BackgroundColor);
			if (createInfo.OutlineThickness != 0.0f)
			{
				mShape.setOutlineThickness(createInfo.OutlineThickness);
				mShape.setOutlineColor(createInfo.OutlineColor);
			}
		}

		explicit UIElement(TextCreateInfo& createInfo) noexcept
			: UIElement(std::move(createInfo))
		{
		}

		explicit UIElement(TextCreateInfo&& createInfo) noexcept
			: mId(createInfo.Id)
			, mType(eType::TEXT)
			, mShape()
			, mText()
			, mNormalizedPosition()
			, mNormalizedSize()
			, mChildren()
			, mParentOrNull(createInfo.ParentOrNull)
		{
			mText.setFont(createInfo.Font);

			const sf::Vector2f parentSize = mParentOrNull ? mParentOrNull->mShape.getSize() : sf::Vector2f(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
			const sf::Vector2f parentPosition = mParentOrNull ? mParentOrNull->mShape.getPosition() : sf::Vector2f(0.0f, 0.0f);

			mNormalizedPosition = createInfo.bIsPositionNormalized ? createInfo.Position : createInfo.Position / parentSize;

			const sf::Vector2f shapePosition = (createInfo.bIsPositionNormalized ? parentSize * createInfo.Position : createInfo.Position) + parentPosition;

			mText.setString(createInfo.Text.c_str());
			mText.setCharacterSize(static_cast<uint32_t>(createInfo.CharacterSizeInPixels));
			mText.setFillColor(createInfo.TextColor);
			mText.setStyle(createInfo.Style);
			mText.setPosition(shapePosition.x, shapePosition.y);
			if (createInfo.OutlineThickness != 0.0f)
			{
				mShape.setOutlineThickness(createInfo.OutlineThickness);
				mShape.setOutlineColor(createInfo.OutlineColor);
			}
		}

		UIElement(UIElement&&) = default;
		UIElement& operator=(UIElement&&) = default;

		inline constexpr const sf::RectangleShape& GetShape() const noexcept { return mShape; }
		inline constexpr const sf::Text& GetText() const noexcept { return mText; }

		//void AddChild(UIElement&& element) noexcept
		//{
		//	std::string text = element.GetText().getString();
		//	element.mParentOrNull = this;
		//	mChildren.push_back(std::move(element));
		//	mChildren.back().mText.setString(text.c_str());
		//}
		void AddChild(UIElement::CreateInfo&& createInfo) noexcept
		{
			createInfo.ParentOrNull = this;
			mChildren.push_back(UIElement(createInfo));
		}

		void AddChild(UIElement::TextCreateInfo&& createInfo) noexcept
		{
			createInfo.ParentOrNull = this;
			mChildren.push_back(UIElement(createInfo));
		}

		void AddChild(UIElement::CreateInfo& createInfo) noexcept
		{
			AddChild(std::move(createInfo));
		}

		void AddChild(UIElement::TextCreateInfo& createInfo) noexcept
		{
			AddChild(std::move(createInfo));
		}

		void Draw(sf::RenderWindow& window) const noexcept \
		{  
			switch (mType)
			{
			case UIElement::eType::ELEMENT:
				window.draw(mShape);
				break;
			case UIElement::eType::TEXT:
				window.draw(mText);
				break;
			case UIElement::eType::COUNT:
				[[fallthrough]];
			default:
				assert(false);
				break;
			}
		}

		void SetFocus(const float focusRate) noexcept
		{
			mShape.setOutlineThickness(-5.0f * focusRate);
		}

		void ClearFocus() noexcept
		{
			mShape.setOutlineThickness(-1.0f);
		}

		inline constexpr const std::vector<UIElement>& GetChildren() const noexcept { return mChildren; }
		inline constexpr std::vector<UIElement>& GetChildren() noexcept { return mChildren; }
		inline constexpr const UIElement* GetParentOrNull() const noexcept { return mParentOrNull; }

	private:
		std::string mId;
		eType mType;
		sf::RectangleShape mShape;
		sf::Text mText;
		sf::Vector2f mNormalizedPosition;
		sf::Vector2f mNormalizedSize;
		std::vector<UIElement> mChildren;
		const UIElement* mParentOrNull;
	};

	export class Renderer final
	{
	public:
		explicit Renderer(sf::RenderWindow& window, const Board& board)
			: mWindow(window)
			, mBoard(board)
			, mFont()
			, mRootWindow(UIElement::CreateInfo{ .Id = "Root", .Position = sf::Vector2f(0.0f, 0.0f), .bIsPositionNormalized = true, .Size = sf::Vector2f(1.0f, 1.0f), .bIsSizeNormalized = true, .BackgroundColor = sf::Color::Black})
			, mBoardPanel(nullptr)
		{
			const sf::Vector2f& rootPosition = mRootWindow.GetShape().getPosition();
			const sf::Vector2f& rootSize = mRootWindow.GetShape().getSize();
			const float maximumLength = std::max(rootSize.x, rootSize.y);
			const float minimumLength = std::min(rootSize.x, rootSize.y);
			const float size = minimumLength * BOARD_SIZE_RATE;
			const sf::Vector2f position = sf::Vector2f((maximumLength - size) / 2.0f, 0.0f) + rootPosition;

			UIElement::CreateInfo createInfo =
			{
				.Id = "BoardPanel",
				.Position = position,
				.bIsPositionNormalized = false,
				.Size = sf::Vector2f(size, size),
				.bIsSizeNormalized = false,
				.BackgroundColor = sf::Color(124, 252, 0),
				.ParentOrNull = &mRootWindow
			};
			mRootWindow.AddChild(createInfo);
			mBoardPanel = &mRootWindow.GetChildren().back();

			assert(mFont.loadFromFile("arial.ttf"));
		}

		void AddTileRenderData(const Tile& tile, const size_t tileIndex) noexcept
		{
			const sf::Vector2f parentSize = mBoardPanel->GetShape().getSize();
			const sf::Vector2f parentPosition = mBoardPanel->GetShape().getPosition();

			const float maximumLength = std::max(parentSize.x, parentSize.y) / 6.5f;
			const float minimumLength = std::min(parentSize.x, parentSize.y) / 13.0f;

			sf::Color color(34, 139, 34);
			sf::Color headerColor = sf::Color::Transparent;
			sf::Vector2f position;
			sf::Vector2f size;

			if (1 <= tileIndex && tileIndex < 10)
			{
				position = sf::Vector2f((11 - tileIndex) / 13.0f, 11.0f / 13.0f);
				size = sf::Vector2f(minimumLength / parentSize.x, maximumLength / parentSize.y);
			}
			else if (11 <= tileIndex && tileIndex < 20)
			{
				position = sf::Vector2f(0.0f, (11 - (tileIndex - 10)) / 13.0f);
				size = sf::Vector2f(maximumLength / parentSize.x, minimumLength / parentSize.y);
			}
			else if (21 <= tileIndex && tileIndex < 30)
			{
				position = sf::Vector2f((1 + (tileIndex - 20)) / 13.0f, 0.0f);
				size = sf::Vector2f(minimumLength / parentSize.x, maximumLength / parentSize.y);
			}
			else if (31 <= tileIndex && tileIndex < 40)
			{
				position = sf::Vector2f(11.0f / 13.0f, (1 + (tileIndex - 30)) / 13.0f);
				size = sf::Vector2f(maximumLength / parentSize.x, minimumLength / parentSize.y);
			}

			sf::Vector2f headerPosition(0.0f, 0.0f);
			sf::Vector2f headerSize;
			std::string tileName;
			switch (tile.GetType())
			{
			case Tile::eType::START:
			{
				position = sf::Vector2f(11.0f / 13.0f, 11.0f / 13.0f);
				size = sf::Vector2f(maximumLength / parentSize.x, maximumLength / parentSize.y);
				tileName = "Start";
			}
			break;
			case Tile::eType::CITY:
			{
				const size_t cityIndex = tile.GetIndex();
				const City& city = mBoard.GetCity(cityIndex);
				const City::eColor cityColor = city.GetColor();
				tileName = city.GetName();
				switch (cityColor)
				{
				case City::eColor::RED:
					headerColor = sf::Color::Red;
					headerSize = sf::Vector2f(1.0f, 0.333333f);
					break;
				case City::eColor::GREEN:
					headerColor = sf::Color::Green;
					headerPosition = sf::Vector2f(0.666667f, 0.0f);
					headerSize = sf::Vector2f(0.333333f, 1.0f);
					break;
				case City::eColor::YELLOW:
					headerColor = sf::Color::Yellow;
					headerPosition = sf::Vector2f(0.0f, 0.666667f);
					headerSize = sf::Vector2f(1.0f, 0.333333f);
					break;
				case City::eColor::BLACK:
					headerColor = sf::Color::Black;
					headerSize = sf::Vector2f(0.333333f, 1.0f);
					break;
				case City::eColor::COUNT:
					[[fallthrough]];
				default:
					assert(false);
					break;
				}
			}
			break;
			case Tile::eType::SPECIAL_CITY:
				break;
			case Tile::eType::GOLDEN_KEY:
				color = sf::Color(255, 215, 0);
				tileName = "Golden Key";
				break;
			case Tile::eType::NO_MANS_LAND:
			{
				color = sf::Color(173, 255, 47);
				position = sf::Vector2f(0.0f, 11.0f / 13.0f);
				size = sf::Vector2f(maximumLength / parentSize.x, maximumLength / parentSize.y);
				tileName = "No Man's Land";
			}
			break;
			case Tile::eType::SOCIAL_WELFARE_FUND_WITHDRAW:
			{
				color = sf::Color(240, 255, 255);
				position = sf::Vector2f(0.0f, 0.0f);
				size = sf::Vector2f(maximumLength / parentSize.x, maximumLength / parentSize.y);
				tileName = "Social Welfare Fund Withdraw";
			}
			break;
			case Tile::eType::SPACE_TRAVEL:
			{
				color = sf::Color(70, 130, 180);
				position = sf::Vector2f(11.0f / 13.0f, 0.0f);
				size = sf::Vector2f(maximumLength / parentSize.x, maximumLength / parentSize.y);
				tileName = "Space Travel";
			}
			break;
			case Tile::eType::SOCIAL_WELFARE_FUND_DEPOSIT:
				color = sf::Color(240, 255, 255);
				tileName = "Social Welfare Fund Deposit";
				break;
			case Tile::eType::COUNT:
				[[fallthrough]];
			default:
				assert(false);
				break;
			}

			UIElement::CreateInfo tileCreateInfo =
			{
				.Id = "Tile::" + tileName,
				.Position = position,
				.bIsPositionNormalized = true,
				.Size = size,
				.bIsSizeNormalized = true,
				.BackgroundColor = color,
				.ParentOrNull = mBoardPanel
			};
			
			mBoardPanel->AddChild(tileCreateInfo);
			UIElement& tileElement = mBoardPanel->GetChildren().back();
			{
				UIElement::CreateInfo tileHeaderCreateInfo =
				{
					.Id = "Tile::Header::" + tileName,
					.Position = headerPosition,
					.bIsPositionNormalized = true,
					.Size = headerSize,
					.bIsSizeNormalized = true,
					.BackgroundColor = headerColor,
					.ParentOrNull = &tileElement
				};
				tileElement.AddChild(tileHeaderCreateInfo);
			}
			{
				UIElement& tileHeaderElement = tileElement.GetChildren().back();
				UIElement::TextCreateInfo tileNameCreateInfo =
				{
					.Id = tileName,
					.Position = sf::Vector2f(0.0f, 0.0f),
					.bIsPositionNormalized = true,
					.OutlineColor = sf::Color::Black,
					.OutlineThickness = 1.0f,
					.Font = mFont,
					.Text = tileName,
					.CharacterSizeInPixels = 12,
					.TextColor = sf::Color::White,
					.Style = sf::Text::Style::Bold,
					.ParentOrNull = &tileHeaderElement
				};
				tileHeaderElement.AddChild(tileNameCreateInfo);
			}

			{
				tileCreateInfo.BackgroundColor = sf::Color::Transparent;
				tileCreateInfo.OutlineColor = sf::Color::Black;
				tileCreateInfo.OutlineThickness = -2.0f;
			}
			mBoardPanel->AddChild(tileCreateInfo);
		}

		void Update(const float deltaTime, const size_t currentTileIndex)
		{
			constexpr const size_t DELTA_TIME_FREQUENCY = 3;
			static float accumulatedDeltaTime = 0.0f;

			std::vector<UIElement>& children = mBoardPanel->GetChildren();
			const size_t numChildren = children.size();
			assert(currentTileIndex < numChildren);
			UIElement& child = children[currentTileIndex];
			if (&child != mTileInFocus)
			{
				if (mTileInFocus != nullptr)
				{
					mTileInFocus->ClearFocus();
				}

				mTileInFocus = &child;
			}

			accumulatedDeltaTime += deltaTime;
			const size_t accumulatedDeltaTimeIntegerPartClamped = static_cast<size_t>(accumulatedDeltaTime) % DELTA_TIME_FREQUENCY;
			accumulatedDeltaTime -= accumulatedDeltaTimeIntegerPartClamped;
			child.SetFocus(accumulatedDeltaTime);
		}

		void Render() noexcept
		{
			mWindow.clear(sf::Color::Black);

			std::vector<UIElement*> uiElementsToDraw;
			uiElementsToDraw.push_back(&mRootWindow);
			while (uiElementsToDraw.empty() == false)
			{
				UIElement& uiElement = *uiElementsToDraw.back();
				uiElementsToDraw.pop_back();
				uiElement.Draw(mWindow);

				std::vector<UIElement>& children = uiElement.GetChildren();
				for (UIElement& child : children)
				{
					uiElementsToDraw.push_back(&child);
				}
			}

			mWindow.display();
		}

	private:
		sf::RenderWindow& mWindow;
		const Board& mBoard;
		sf::Font mFont;

		UIElement mRootWindow;
		UIElement* mBoardPanel;
		UIElement* mTileInFocus;
	};
}