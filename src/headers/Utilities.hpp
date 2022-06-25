#pragma once

#include <random>
#include <fstream>
#include <sstream>
#include <tuple>
#include <algorithm>

#include "Exceptions.hpp"

/**
 * @brief Retruns a random integer in a given closed interval
 * 
 * @param from begin of the interval
 * @param to end of the interval
 * @return the integer
 */
int randomInt(const int & from, const int & to);

/**
 * @brief Trims the leading and ending cahracters from a string
 * 
 * @param str the string to be trimmed
 * @param charsToAvoid the characters to trim away
 */
void trim(std::string & str, const std::string & charsToAvoid = " \t\n");

/**
 * @brief Loads integer data from a file of given format
 * 
 * @param filePath the path to the file
 * @param item the label of the data
 * @return the data
 */
int loadData(const char * filePath, const char * item);

/**
 * @brief Loads multiline data from a file of given format
 * 
 * @param filePath the path to the file
 * @param item the label of the data
 * @param flag has to be there, so the functions can overload
 * @return the stream of data
 */
std::stringstream loadData(const char * filePath, const char * item, const bool & flag);

/**
 * @brief Saves integer data into a file of given format
 * 
 * @param filePath the path to the file
 * @param item the label of the data
 * @param num the integer data to be saved
 */
void saveData(const char * filePath, const char * item, const int & num);

/**
 * @brief Saves multiline data into a file of given format
 * 
 * @param filePath the path to the file
 * @param item the label of the data
 * @param data stream of data to be saved
 */
void saveData(const char * filePath, const char * item, const std::stringstream & data);

/**
 * @brief Sets a string to lower case
 * 
 * @param str the string
 * @return the lower case string
 */
std::string toLower(const std::string & str);

/**
 * @brief Determines, whether the string is a label
 * 
 * A label must begin and end with a quotation mark
 * 
 * @param line a line of a file
 * @return true - it is a label
 * @return false - otherwise
 */
bool isLabel(const std::string & line);