#pragma once

/**
 * @brief The various types of tiles on the game map
 * 
 * @note ETileType_MAX holds the number of different tile types
 */
enum ETileType
{
    EMPTY,
    WALL,
    BREAKABLE,
    PLAYER1,
    PLAYER2,
    ENEMY,
    BONUS,
    BOMB,
    BOOM,
    DOOR,
    ETileType_MAX = DOOR
};