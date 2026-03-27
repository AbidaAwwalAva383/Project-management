#ifndef PROJECTEXCEPTIONS_H
#define PROJECTEXCEPTIONS_H

#include <stdexcept>
#include <string>

class FileAccessException : public std::runtime_error {
public:
    explicit FileAccessException(const std::string& message);
};

class UsernameTakenException : public std::runtime_error {
public:
    explicit UsernameTakenException(const std::string& username);
};

class LoginFailedException : public std::runtime_error {
public:
    LoginFailedException();
};

class DuplicateLoanException : public std::runtime_error {
public:
    DuplicateLoanException(const std::string& bookId, const std::string& username);
};

#endif
