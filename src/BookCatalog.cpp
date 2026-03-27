#include "BookCatalog.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <vector>

static std::string toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return r;
}

BookCatalog::BookCatalog() {}

void BookCatalog::addBook(const Book& book) {
    books.add(book);
}

std::string BookCatalog::getBookTitle(const std::string& id) {
    Book* b = getBook(id);
    return b ? b->getTitle() : "Unknown Book";
}

Book* BookCatalog::getBook(const std::string& id) {
    for (int i = 0; i < books.getSize(); i++) {
        if (books[i].getId() == id) {
            return &books[i];
        }
    }
    return nullptr;
}

bool BookCatalog::removeBook(const std::string& id) {
    for (int i = 0; i < books.getSize(); i++) {
        if (books[i].getId() == id) {
            std::cout << "\"" << books[i].getTitle() << "\" removed from catalog.\n";
            books.removeAt(i);
            return true;
        }
    }
    std::cout << "Book with ID \"" << id << "\" not found.\n";
    return false;
}

void BookCatalog::displayBookTable(const std::vector<const Book*>& bookList) const {
    if (bookList.empty()) {
        std::cout << "No matching books found.\n";
        return;
    }

    std::cout << std::left
              << "| " << std::setw(8)  << "ID"
              << " | " << std::setw(42) << "Title"
              << " | " << std::setw(28) << "Author"
              << " | " << std::setw(12) << "Status" << " |\n";
    std::cout << std::string(102, '-') << "\n";

    for (size_t i = 0; i < bookList.size(); ++i) {
        const Book* b = bookList[i];
        std::cout << std::left
                  << "| " << std::setw(8)  << b->getId()
                  << " | " << std::setw(42) << b->getTitle().substr(0, 40)
                  << " | " << std::setw(28) << b->getAuthor().substr(0, 26)
                  << " | " << std::setw(12) << (b->getAvailability() ? "Available" : "Borrowed")
                  << " |\n";
    }
}

void BookCatalog::displayAllBooks() const {
    if (books.getSize() == 0) {
        std::cout << "The catalog is empty.\n";
        return;
    }

    std::vector<const Book*> sortedBooks;
    for (int i = 0; i < books.getSize(); ++i) {
        sortedBooks.push_back(&books[i]);
    }

    std::sort(sortedBooks.begin(), sortedBooks.end(),
              [](const Book* a, const Book* b) {
                  return toLower(a->getTitle()) < toLower(b->getTitle());
              });

    std::cout << "\n======= Book Catalog (Sorted Alphabetically) =======\n";
    displayBookTable(sortedBooks);
    std::cout << std::string(102, '=') << "\n";
}

void BookCatalog::searchByTitle(const std::string& keyword) const {
    std::string kw = toLower(keyword);
    std::vector<const Book*> matches;

    for (int i = 0; i < books.getSize(); i++) {
        if (toLower(books[i].getTitle()).find(kw) != std::string::npos) {
            matches.push_back(&books[i]);
        }
    }

    std::sort(matches.begin(), matches.end(),
              [](const Book* a, const Book* b) {
                  return toLower(a->getTitle()) < toLower(b->getTitle());
              });

    std::cout << "\n--- Results for title \"" << keyword << "\" ---\n";
    displayBookTable(matches);
}

void BookCatalog::searchByAuthor(const std::string& keyword) const {
    std::string kw = toLower(keyword);
    std::vector<const Book*> matches;

    for (int i = 0; i < books.getSize(); i++) {
        if (toLower(books[i].getAuthor()).find(kw) != std::string::npos) {
            matches.push_back(&books[i]);
        }
    }

    std::sort(matches.begin(), matches.end(),
              [](const Book* a, const Book* b) {
                  return toLower(a->getTitle()) < toLower(b->getTitle());
              });

    std::cout << "\n--- Results for author \"" << keyword << "\" ---\n";
    displayBookTable(matches);
}

int BookCatalog::getTotalBooks() const {
    return books.getSize();
}

void BookCatalog::saveToFile(const std::string& filename) const {
    books.saveAll(filename);
}

void BookCatalog::loadFromFile(const std::string& filename) {
    books.loadAll(filename);
}
