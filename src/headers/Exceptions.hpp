#pragma once

#include <string>
#include <stdexcept>

/**
 * @brief Custom exeption for file handling errors
 */
struct FileException : public std::runtime_error
{
    /**
     * @brief FileException constructor
     * 
     * @param message the error message
     */
    FileException(char const* const message) noexcept;

    /**
     * @brief FileException constructor
     * 
     * @param message the error message
     */
    FileException(const std::string message) noexcept;

    /**
     * @brief Shows the error message
     * 
     * @return the message
     */
    char const* what() const noexcept override;

};