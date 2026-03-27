#include "Loan.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdio>
#include <stdexcept>

using namespace std;

const double Loan::FINE_RATE_PER_DAY = 5.0;

// ============================================================
//  Private date helpers
// ============================================================

// Returns today's date as "YYYY-MM-DD"
string Loan::today() {
    time_t t = time(nullptr);
    tm* lt   = localtime(&t);
    ostringstream oss;
    oss << (1900 + lt->tm_year) << "-"
        << setfill('0') << setw(2) << (lt->tm_mon + 1) << "-"
        << setfill('0') << setw(2) << lt->tm_mday;
    return oss.str();
}

// Parses "YYYY-MM-DD" into a tm struct (year/mon/mday only — time is zeroed)
static tm parseDateStr(const string& s) {
    tm t = {};
    sscanf(s.c_str(), "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
    t.tm_year -= 1900;
    t.tm_mon  -= 1;
    t.tm_isdst = -1;
    return t;
}

// Adds N calendar days to a date string and returns a new date string
string Loan::addDays(const string& date, int days) {
    tm t     = parseDateStr(date);
    time_t ts = mktime(&t);
    ts       += (time_t)days * 86400;
    tm* nt   = localtime(&ts);
    ostringstream oss;
    oss << (1900 + nt->tm_year) << "-"
        << setfill('0') << setw(2) << (nt->tm_mon + 1) << "-"
        << setfill('0') << setw(2) << nt->tm_mday;
    return oss.str();
}

// Returns (to - from) in whole days. Positive = 'to' is later.
int Loan::daysBetween(const string& from, const string& to) {
    tm tfrom = parseDateStr(from);
    tm tto   = parseDateStr(to);
    time_t t1 = mktime(&tfrom);
    time_t t2 = mktime(&tto);
    if (t1 == -1 || t2 == -1) return 0;
    return static_cast<int>((t2 - t1) / 86400);
}

// ============================================================
//  Constructors
// ============================================================

Loan::Loan()
    : bookId(""), memberUsername(""), issueDate(""), dueDate(""), returnDate("") {}

Loan::Loan(const string& bookId, const string& memberUsername,
           const string& issueDate, int loanPeriodDays)
    : bookId(bookId), memberUsername(memberUsername),
      issueDate(issueDate), returnDate("") {
    if (loanPeriodDays <= 0)
        throw invalid_argument("Loan period must be positive.");
    dueDate = addDays(issueDate, loanPeriodDays);
}

Loan::Loan(const Loan& other)
    : bookId(other.bookId), memberUsername(other.memberUsername),
      issueDate(other.issueDate), dueDate(other.dueDate),
      returnDate(other.returnDate) {}

Loan& Loan::operator=(const Loan& other) {
    if (this != &other) {
        bookId         = other.bookId;
        memberUsername = other.memberUsername;
        issueDate      = other.issueDate;
        dueDate        = other.dueDate;
        returnDate     = other.returnDate;
    }
    return *this;
}

Loan::~Loan() {}

// ============================================================
//  Getters
// ============================================================
string Loan::getBookId()         const { return bookId; }
string Loan::getMemberUsername() const { return memberUsername; }
string Loan::getIssueDate()      const { return issueDate; }
string Loan::getDueDate()        const { return dueDate; }
string Loan::getReturnDate()     const { return returnDate; }
bool   Loan::isReturned()        const { return !returnDate.empty(); }

// ============================================================
//  Core UML method: isOverdue()
//  If already returned: was it returned after the due date?
//  If still out:        is today past the due date?
// ============================================================
bool Loan::isOverdue() const {
    if (dueDate.empty()) return false;
    const string& checkDate = isReturned() ? returnDate : today();
    return daysBetween(dueDate, checkDate) > 0;
}

// Returns days past the due date (0 if on time)
int Loan::overdueDays() const {
    if (!isOverdue()) return 0;
    const string& checkDate = isReturned() ? returnDate : today();
    return daysBetween(dueDate, checkDate);
}

// Returns the dollar fine owed
double Loan::fineAmount(double ratePerDay) const {
    int days = overdueDays();
    return days > 0 ? days * ratePerDay : 0.0;
}

// ============================================================
//  markReturned
// ============================================================
void Loan::markReturned(const string& date) {
    returnDate = date.empty() ? today() : date;
}

// ============================================================
//  Display
// ============================================================
void Loan::displayInfo() const {
    cout << "Book:    " << bookId         << "\n"
         << "Member:  " << memberUsername << "\n"
         << "Issued:  " << issueDate      << "\n"
         << "Due:     " << dueDate        << "\n"
         << "Returned:" << (isReturned() ? returnDate : "not yet") << "\n"
         << "Overdue: " << (isOverdue()  ? "YES (" + to_string(overdueDays()) + " days)" : "No") << "\n";
}

// ============================================================
//  Operator overloading
// ============================================================
bool Loan::operator==(const Loan& other) const {
    return bookId == other.bookId && memberUsername == other.memberUsername;
}

ostream& operator<<(ostream& os, const Loan& loan) {
    os << "[Loan] " << loan.bookId << " -> " << loan.memberUsername
       << " | Issued: " << loan.issueDate
       << " | Due: "    << loan.dueDate
       << " | "         << (loan.isReturned()
                              ? "Returned: " + loan.returnDate
                              : "Not returned")
       << (loan.isOverdue() ? " [OVERDUE]" : "");
    return os;
}

// ============================================================
//  File I/O
//  Format: bookId|memberUsername|issueDate|dueDate|returnDate
//  returnDate is stored as empty string "-" when not returned
// ============================================================
string Loan::serialize() const {
    return bookId + "|" + memberUsername + "|" + issueDate + "|"
           + dueDate + "|" + (returnDate.empty() ? "-" : returnDate);
}

Loan Loan::deserialize(const string& data) {
    stringstream ss(data);
    string bId, mUser, issued, due, returned;
    getline(ss, bId,     '|');
    getline(ss, mUser,   '|');
    getline(ss, issued,  '|');
    getline(ss, due,     '|');
    getline(ss, returned,'|');

    Loan loan;
    loan.bookId         = bId;
    loan.memberUsername = mUser;
    loan.issueDate      = issued;
    loan.dueDate        = due;
    loan.returnDate     = (returned == "-") ? "" : returned;
    return loan;
}
