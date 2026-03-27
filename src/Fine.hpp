#ifndef FINE_HPP
#define FINE_HPP

#include <string>
#include <ostream>
using namespace std;

// ============================================================
//  Abstract Base Class: Payable  (ABSTRACTION + POLYMORPHISM)
//  Any class that represents something payable must implement
//  processPayment() and getPaymentDetails()
// ============================================================
class Payable {
public:
    virtual void processPayment() = 0;           // Pure virtual (forces override)
    virtual string getPaymentDetails() const = 0; // Pure virtual
    virtual ~Payable() {}                         // Virtual destructor (good OOP practice)
};


// ============================================================
//  Fine Class
//  - Inherits from Payable (INHERITANCE + ABSTRACTION)
//  - Demonstrates: Encapsulation, Polymorphism, Operator Overloading
// ============================================================
class Fine : public Payable {
private:
    // ENCAPSULATION: all data is private
    double amount;
    bool isPaid;
    string reason;      // Why was the fine issued?
    int overdueDays;    // How many days overdue

    // ENCAPSULATION: private helper method
    void validateAmount(double amt) const;

public:
    // --- Constructors (CONSTRUCTOR OVERLOADING) ---
    Fine();                                                        // Default
    Fine(double amount, const string& reason, int overdueDays);   // Parameterized
    Fine(const Fine& other);                                       // Copy constructor

    // Destructor
    virtual ~Fine();

    // --- Getters (ENCAPSULATION) ---
    double getAmount()     const;
    bool   getIsPaid()     const;
    string getReason()     const;
    int    getOverdueDays() const;

    // --- Setters (ENCAPSULATION) ---
    void setAmount(double amount);
    void setReason(const string& reason);

    // --- Overriding Payable (POLYMORPHISM) ---
    void   processPayment()            override;  // implements pure virtual
    string getPaymentDetails()   const override;  // implements pure virtual

    // --- Own Methods ---
    void displayInfo() const;
    bool isWaivable()  const;   // Fine under $10 can be waived
    void waiveFine();

    // --- Static Method ---
    static double calculateFineAmount(int overdueDays, double ratePerDay = 5.0);

    // --- OPERATOR OVERLOADING ---
    Fine   operator+(const Fine& other) const;  // Merge two fines
    bool   operator>(const Fine& other) const;  // Compare amounts
    bool   operator==(const Fine& other) const; // Equality check
    Fine&  operator=(const Fine& other);        // Assignment operator
    friend ostream& operator<<(ostream& os, const Fine& fine); // << for printing
};

#endif // FINE_HPP
