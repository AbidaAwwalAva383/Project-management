#ifndef BOOKCATALOG_H
#define BOOKCATALOG_H

#include "Book.h"
#include "Repository.h"
#include <string>
#include <vector>

class BookCatalog {
private:
    Repository<Book> books;
    void displayBookTable(const std::vector<const Book*>& bookList) const;

public:
    BookCatalog();

    void  addBook(const Book& book);
    std::string getBookTitle(const std::string& id);
    Book* getBook(const std::string& id);
    bool  removeBook(const std::string& id);

    void displayAllBooks()                      const;
    void searchByTitle(const std::string& kw)   const;
    void searchByAuthor(const std::string& kw)  const;

    int getTotalBooks() const;

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif
