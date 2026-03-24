#include "BookCatalog.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

BookCatalog::BookCatalog() {}

// These are the core operations

void BookCatalog::addBook(const Book& book) {
    books.push_back(book);
    std::cout << "\"" << book.getTitle() << "\" added to catalog successfully.\n";
}

bool BookCatalog::removeBook(const std::string& id) {
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->getId() == id) {
            std::cout << "\"" << it->getTitle() << "\" removed from catalog.\n";
            books.erase(it);
            return true;
        }
    }
    std::cout << "Book with ID \"" << id << "\" not found.\n";
    return false;
}

Book* BookCatalog::getBook(const std::string& id) {
    for (size_t i = 0; i < books.size(); i++) {
        if (books[i].getId() == id)
            return &books[i];
    }
    return nullptr;
}

// display 

void BookCatalog::displayAllBooks() const {
    if (books.empty()) {
        std::cout << "The catalog is empty.\n";
        return;
    }
    std::cout << "\n======= Book Catalog (" << books.size() << " books) =======\n";
    for (size_t i = 0; i < books.size(); i++) {
        std::cout << "---\n";
        books[i].display();
    }
    std::cout << "==========================================\n";
}

//  search 

// Helper: converts a string to lowercase for case-insensitive comparison
static std::string toLower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return result;
}

void BookCatalog::searchByTitle(const std::string& keyword) const {
    std::string kw = toLower(keyword);
    bool found = false;
    std::cout << "\n--- Results for title \"" << keyword << "\" ---\n";
    for (const auto& b : books) {
        if (toLower(b.getTitle()).find(kw) != std::string::npos) {
            b.display();
            std::cout << "---\n";
            found = true;
        }
    }
    if (!found) std::cout << "No matching books found.\n";
}

void BookCatalog::searchByAuthor(const std::string& keyword) const {
    std::string kw = toLower(keyword);
    bool found = false;
    std::cout << "\n--- Results for author \"" << keyword << "\" ---\n";
    for (const auto& b : books) {
        if (toLower(b.getAuthor()).find(kw) != std::string::npos) {
            b.display();
            std::cout << "---\n";
            found = true;
        }
    }
    if (!found) std::cout << "No matching books found.\n";
}

int BookCatalog::getTotalBooks() const { return books.size(); }

//  file I/O Operation
// Each line: ID|Title|Author|1 (available) or 0 (borrowed)

void BookCatalog::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: could not open \"" << filename << "\" for writing.\n";
        return;
    }
    for (const auto& b : books) {
        file << b.getId()     << "|"
             << b.getTitle()  << "|"
             << b.getAuthor() << "|"
             << (b.getAvailability() ? "1" : "0")
             << "\n";
    }
    file.close();
    std::cout << "Catalog saved to \"" << filename << "\" (" << books.size() << " books).\n";
}

void BookCatalog::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No existing catalog file found. Starting fresh.\n";
        return;
    }
    books.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, title, author, availStr;
        std::getline(ss, id,       '|');
        std::getline(ss, title,    '|');
        std::getline(ss, author,   '|');
        std::getline(ss, availStr, '|');

        Book b(id, title, author);
        b.setAvailability(availStr == "1");
        books.push_back(b);
    }
    file.close();
    std::cout << books.size() << " book(s) loaded from \"" << filename << "\".\n";
}
