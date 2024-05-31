// BlueMarble.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

import Game;

int main(int argc, char** argv)
{
	size_t numPlayers = bm::DEFAULT_PLAYER_COUNT;
	for (size_t argumentIndex = 0; argumentIndex < static_cast<size_t>(argc); ++argumentIndex)
	{
		const char* argument = argv[argumentIndex];

		if (std::strcmp(argument, "num_players") == 0)
		{
			if (argumentIndex < static_cast<size_t>(argc))
			{
				++argumentIndex;
				numPlayers = std::clamp(static_cast<size_t>(std::atoi(argv[argumentIndex + 1])), bm::DEFAULT_PLAYER_COUNT, bm::MAX_PLAYER_COUNT);
			}
			break;
		}
	}

	bm::Game game(numPlayers);
	game.Run();

	return 0;
}