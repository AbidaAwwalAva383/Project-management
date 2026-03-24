#include "Book.h"
#include <iostream>
#include <sstream>
#include <iomanip>

int Book::nextIdNumber = 1;

Book::Book(const std::string& title, const std::string& author)
    : title(title), author(author), isAvailable(true) {
    std::stringstream ss;
    ss << "B" << std::setfill('0') << std::setw(3) << nextIdNumber;
    id = ss.str();
    nextIdNumber++;
}

Book::Book(const std::string& id,
           const std::string& title,
           const std::string& author)
    : id(id), title(title), author(author), isAvailable(true) {}

const std::string& Book::getId()     const { return id; }
const std::string& Book::getTitle()  const { return title; }
const std::string& Book::getAuthor() const { return author; }
bool Book::getAvailability()         const { return isAvailable; }

void Book::setAvailability(bool status) { isAvailable = status; }

void Book::display() const {
    std::cout << "ID:     " << id     << std::endl;
    std::cout << "Title:  " << title  << std::endl;
    std::cout << "Author: " << author << std::endl;
    std::cout << "Status: " << (isAvailable ? "Available" : "Borrowed") << std::endl;
}

std::string Book::peekNextId() {
    std::stringstream ss;
    ss << "B" << std::setfill('0') << std::setw(3) << nextIdNumber;
    return ss.str();
}
