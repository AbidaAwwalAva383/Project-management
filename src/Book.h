#ifndef BOOK_H
#define BOOK_H
#include <string>

class Book {
private:
    std::string id;
    std::string title;
    std::string author;
    bool isAvailable;
    static int nextIdNumber;

public:
    // Auto-ID constructor (used when adding new books)
    Book(const std::string& title, const std::string& author);

    // Manual-ID constructor (used when loading from file)
    Book(const std::string& id, const std::string& title, const std::string& author);

    // Getters
    const std::string& getId()     const;
    const std::string& getTitle()  const;
    const std::string& getAuthor() const;
    bool getAvailability()         const;

    // Setters
    void setAvailability(bool status);

    // Display
    void display() const;

    // Static utility
    static std::string peekNextId();

    // File I/O (required by Repository<Book>)
    std::string serialize() const;
    static Book deserialize(const std::string& data);
};

#endif // BOOK_H
