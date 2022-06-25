#include "Exceptions.hpp"

FileException::FileException(char const* const message) noexcept
: std::runtime_error(message)
{}

FileException::FileException(const std::string message) noexcept
: std::runtime_error(message)
{}

char const* FileException::what() const noexcept
{
    return std::runtime_error::what();
}