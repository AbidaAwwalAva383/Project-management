#include "ProjectExceptions.h"

FileAccessException::FileAccessException(const std::string& message)
    : std::runtime_error(message) {}

UsernameTakenException::UsernameTakenException(const std::string& username)
    : std::runtime_error("Registration Failed: Username '" + username + "' is already taken.") {}

LoginFailedException::LoginFailedException()
    : std::runtime_error("Invalid username or password.") {}

DuplicateLoanException::DuplicateLoanException(const std::string& bookId, const std::string& username)
    : std::runtime_error("Duplicate loan prevented: user '" + username + "' already has an active loan for book '" + bookId + "'.") {}
