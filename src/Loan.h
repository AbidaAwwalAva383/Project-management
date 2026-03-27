#ifndef LOAN_H
#define LOAN_H

#include <string>
#include <iostream>

// ============================================================
//  Loan Class
//  Tracks a single borrowing transaction.
//  Dates are stored as "YYYY-MM-DD" strings — simple and portable.
//  Matches UML: +issueDate, +dueDate, +returnDate, +isOverdue()
// ============================================================
class Loan {
private:
    std::string bookId;
    std::string memberUsername;
    std::string issueDate;    // date book was borrowed  (YYYY-MM-DD)
    std::string dueDate;      // issueDate + loanPeriod  (YYYY-MM-DD)
    std::string returnDate;   // empty string = not yet returned

    static const int DEFAULT_LOAN_DAYS = 14;
    static const double FINE_RATE_PER_DAY;   // defined in .cpp

    // --- Private date helpers ---
    // Adds N days to a YYYY-MM-DD string and returns a new YYYY-MM-DD string
    static std::string addDays(const std::string& date, int days);

    // Returns the number of calendar days between two YYYY-MM-DD strings.
    // Positive result means 'to' is after 'from'.
    static int daysBetween(const std::string& from, const std::string& to);

public:
    // --- Constructors ---
    Loan();   // needed for deserialization

    // Returns today as a YYYY-MM-DD string (public so TransactionManager can use it)
    static std::string today();

    // Creates an active (not yet returned) loan.
    // dueDate is calculated automatically as issueDate + loanPeriod.
    Loan(const std::string& bookId,
         const std::string& memberUsername,
         const std::string& issueDate,
         int loanPeriodDays = DEFAULT_LOAN_DAYS);

    // Copy constructor
    Loan(const Loan& other);

    // Assignment operator
    Loan& operator=(const Loan& other);

    // Destructor
    ~Loan();

    // --- Getters ---
    std::string getBookId()         const;
    std::string getMemberUsername() const;
    std::string getIssueDate()      const;
    std::string getDueDate()        const;
    std::string getReturnDate()     const;
    bool        isReturned()        const;   // true if returnDate is not empty

    // --- Core UML method ---
    // Returns true if today (or returnDate if already returned) is past dueDate
    bool isOverdue() const;

    // Returns how many days overdue (0 if not overdue or not yet returned)
    int overdueDays() const;

    // Returns the calculated fine amount based on overdue days
    double fineAmount(double ratePerDay = FINE_RATE_PER_DAY) const;

    // --- Setters ---
    // Called when the book is returned. Sets returnDate to today if empty.
    void markReturned(const std::string& date = "");

    // --- Display ---
    void displayInfo() const;

    // --- Operator overloading ---
    bool operator==(const Loan& other) const;   // same book + same member
    friend std::ostream& operator<<(std::ostream& os, const Loan& loan);

    // --- File I/O (required by Repository<Loan>) ---
    std::string serialize()                     const;
    static Loan deserialize(const std::string& data);
};

#endif // LOAN_H
