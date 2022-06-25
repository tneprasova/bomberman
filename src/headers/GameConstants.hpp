#pragma once

#include "Utilities.hpp"

// Path to the configurations file
#ifndef TESTING
const char * const config = "./examples/config.txt";
#endif

#ifdef TESTING
const char * const config = "./examples/config-test.txt";
#endif

// Screen dimensions
const int screenWidth   = 2208;
const int screenHeight  = 1440;

// Tile width on screen
const int tileWidth     = 96;

// Game speed
const int FPS           = 60;
const int frameDelay    = 1000 / FPS;

// Map dimensions
const int mapWidth      = (screenWidth / tileWidth) % 2 == 0 ? screenWidth / tileWidth - 1 : screenWidth / tileWidth;
const int mapHeight     = (screenHeight / tileWidth) % 2 == 0 ? screenHeight / tileWidth - 1 : screenHeight / tileWidth;

// Entity characteristics
const int playerSpeed   = (tileWidth / 32) / ((double)FPS / 60) + 1;
const int enemySpeed    = (tileWidth / 32) / ((double)FPS / 60);

// Defines for the classes to shorten the code
#define Events  std::list<std::tuple<EEvent, std::pair<int, int>, int, CObject *>>
#define Event   std::tuple<EEvent, std::pair<int, int>, int, CObject *>
#define TileSet std::vector<std::vector<std::unique_ptr<CTile>>>
#define Map     std::vector<std::vector<ETileType>>