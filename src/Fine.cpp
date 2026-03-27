#include "Fine.hpp"
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

// ============================================================
//  Private Helper
// ============================================================
void Fine::validateAmount(double amt) const {
    if (amt < 0)
        throw invalid_argument("Fine amount cannot be negative.");
}

// ============================================================
//  Constructors
// ============================================================
Fine::Fine()
    : amount(0.0), isPaid(false), reason("Unknown"), overdueDays(0) {}

Fine::Fine(double amount, const string& reason, int overdueDays)
    : isPaid(false), reason(reason), overdueDays(overdueDays) {
    validateAmount(amount);
    this->amount = amount;
}

Fine::Fine(const Fine& other)
    : amount(other.amount), isPaid(other.isPaid),
      reason(other.reason), overdueDays(other.overdueDays) {}

Fine::~Fine() {}   // No dynamic memory; virtual destructor satisfies Payable

// ============================================================
//  Getters
// ============================================================
double Fine::getAmount()      const { return amount; }
bool   Fine::getIsPaid()      const { return isPaid; }
string Fine::getReason()      const { return reason; }
int    Fine::getOverdueDays() const { return overdueDays; }

// ============================================================
//  Setters
// ============================================================
void Fine::setAmount(double amt) {
    validateAmount(amt);
    amount = amt;
}

void Fine::setReason(const string& r) {
    reason = r;
}

// ============================================================
//  Payable Interface Implementation (POLYMORPHISM)
// ============================================================
void Fine::processPayment() {
    if (isPaid) {
        cout << "[Fine] Already paid.\n";
        return;
    }
    isPaid = true;
    cout << "[Fine] Payment of $" << amount << " processed. Reason: " << reason << "\n";
}

string Fine::getPaymentDetails() const {
    ostringstream oss;
    oss << "Fine: $" << amount
        << " | Reason: " << reason
        << " | Days overdue: " << overdueDays
        << " | Status: " << (isPaid ? "PAID" : "UNPAID");
    return oss.str();
}

// ============================================================
//  Own Methods
// ============================================================
void Fine::displayInfo() const {
    cout << getPaymentDetails() << "\n";
}

bool Fine::isWaivable() const {
    return (!isPaid && amount < 10.0);
}

void Fine::waiveFine() {
    if (isWaivable()) {
        isPaid = true;
        amount = 0.0;
        cout << "[Fine] Fine waived (amount was under $10).\n";
    } else {
        cout << "[Fine] Fine is not waivable.\n";
    }
}

// ============================================================
//  Static Method
// ============================================================
double Fine::calculateFineAmount(int overdueDays, double ratePerDay) {
    return (overdueDays > 0) ? overdueDays * ratePerDay : 0.0;
}

// ============================================================
//  Operator Overloading
// ============================================================

// Merge two fines into one combined fine
Fine Fine::operator+(const Fine& other) const {
    Fine merged;
    merged.amount      = this->amount + other.amount;
    merged.reason      = this->reason + " + " + other.reason;
    merged.overdueDays = this->overdueDays + other.overdueDays;
    merged.isPaid      = false;   // merged fine starts unpaid
    return merged;
}

bool Fine::operator>(const Fine& other) const {
    return this->amount > other.amount;
}

bool Fine::operator==(const Fine& other) const {
    return (this->amount == other.amount &&
            this->reason == other.reason &&
            this->overdueDays == other.overdueDays);
}

Fine& Fine::operator=(const Fine& other) {
    if (this != &other) {
        amount      = other.amount;
        isPaid      = other.isPaid;
        reason      = other.reason;
        overdueDays = other.overdueDays;
    }
    return *this;
}

ostream& operator<<(ostream& os, const Fine& fine) {
    os << fine.getPaymentDetails();
    return os;
}
