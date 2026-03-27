#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include <string>
#include <vector>
#include <iostream>
#include "Fine.hpp"
#include "Book.h"
#include "Repository.h"

using namespace std;

// ============================================================
//  Abstract Base Class: Person
// ============================================================
class Person {
protected:
    string name;
    int age;
public:
    Person(const string& name, int age);
    virtual ~Person() {}

    string getName() const { return name; }
    int    getAge()  const { return age;  }
    virtual void displayInfo() const = 0;
};

// ============================================================
//  Abstract Base Class: UserAccount
// ============================================================
class UserAccount {
protected:
    string username;
    string passwordHash;
    string role;
    bool   isLoggedIn;

public:
    UserAccount(const string& uname, const string& pwd,
                const string& r, bool isHashed = false);
    virtual ~UserAccount() {}

    virtual void   login()               = 0;
    virtual void   logout()              = 0;
    virtual string serialize()     const = 0;

    static string        hashPassword(const string& pwd);
    bool                 verifyPassword(const string& inputPwd) const;
    static UserAccount* deserialize(const string& data);

    string getUsername()    const { return username;    }
    string getRole()        const { return role;        }
    bool   getIsLoggedIn()  const { return isLoggedIn;  }
};

// ============================================================
//  Member
// ============================================================
class Member : public Person, public UserAccount {
private:
    string memberId;
    static int nextMemberId; // Initialized in .cpp
public:
    // Constructor for new users signing up
    Member(const string& name, int age, const string& uname, const string& pwd);
    
    // FIXED: Added 'const string& id' to match the .cpp and allow deserialization
    Member(const string& name, int age, const string& uname, 
           const string& pwd, const string& id, bool isHashed = false);

    string getMemberId() const;
    void   login()               override;
    void   logout()              override;
    void   displayInfo()   const override;
    string serialize()     const override;
};

// ============================================================
//  Librarian
// ============================================================
class Librarian : public Person, public UserAccount {
private:
    string staffId;
    // FIXED: Changed to Repository<Fine*> to match the .cpp pointer logic
    Repository<Fine*>  collectedFines; 
    Repository<Book*>  managedBooks;
    double totalFinesCollected;

    void logAction(const string& action) const;

public:
    Librarian(const string& name, int age,
              const string& uname, const string& pwd,
              const string& staffId, bool isHashed = false);

    string getStaffId()             const { return staffId; }
    void   setStaffId(const string& id)   { staffId = id;   }
    int    getBooksAddedCount()     const { return managedBooks.getSize(); }
    double getTotalFinesCollected() const { return totalFinesCollected;    }

    void   login()               override;
    void   logout()              override;
    void   displayInfo()   const override;
    string serialize()     const override;

    void   addBook(Book& book);
    void   collectFine(Fine& fine);
    void   showCollectedFines() const;
    void   showManagedBooks()   const;
    double calculateTotalFines() const;

    bool       operator==(const Librarian& other) const;
    Librarian& operator= (const Librarian& other);
    friend ostream& operator<<(ostream& os, const Librarian& lib);
};

#endif // LIBRARYSYSTEM_H