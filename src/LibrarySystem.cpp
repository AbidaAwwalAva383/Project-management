#include "LibrarySystem.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

// ============================================================
//  Static Member Initialization
// ============================================================
// This must be initialized in the .cpp file
int Member::nextMemberId = 1000;

// ============================================================
//  Security: DJB2 Hash
// ============================================================
string UserAccount::hashPassword(const string& pwd) {
    std::uint64_t hash = 5381;
    for (char c : pwd) hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    stringstream ss; 
    ss << hex << hash;
    return ss.str();
}

// ============================================================
//  Person
// ============================================================
Person::Person(const string& name, int age) : name(name), age(age) {}

// ============================================================
//  UserAccount
// ============================================================
UserAccount::UserAccount(const string& uname, const string& pwd,
                         const string& r, bool isHashed)
    : username(uname), role(r), isLoggedIn(false) {
    passwordHash = isHashed ? pwd : hashPassword(pwd);
}

bool UserAccount::verifyPassword(const string& inputPwd) const {
    return hashPassword(inputPwd) == passwordHash;
}

// FIXED: Deserialization now accounts for the new memberId field
UserAccount* UserAccount::deserialize(const string& data) {
    stringstream ss(data);
    string part;
    vector<string> v;
    while (getline(ss, part, '|')) v.push_back(part);
    
    if (v.empty()) return nullptr;

    // Mapping based on your serialize() format:
    // 0:Type, 1:uname, 2:pwdHash, 3:role, 4:name, 5:age, 6:ID/StaffId
    if (v[0] == "MEMBER") {
        // Args: name(v4), age(v5), uname(v1), pwd(v2), id(v6), isHashed(true)
        return new Member(v[4], stoi(v[5]), v[1], v[2], v[6], true);
    } 
    if (v[0] == "LIBRARIAN") {
        // Args: name(v4), age(v5), uname(v1), pwd(v2), staffId(v6), isHashed(true)
        return new Librarian(v[4], stoi(v[5]), v[1], v[2], v[6], true);
    }
    return nullptr;
}

// ============================================================
//  Member
// ============================================================
Member::Member(const string& name, int age, const string& uname, const string& pwd)
        : Person(name, age), UserAccount(uname, pwd, "Member") {
    stringstream ss;
    ss << "M" << setfill('0') << setw(4) << nextMemberId++;
    memberId = ss.str();
}

Member::Member(const string& name, int age, const string& uname, const string& pwd, 
               const string& id, bool isHashed)
        : Person(name, age), UserAccount(uname, pwd, "Member", isHashed), memberId(id) {
    
    // Update the counter to ensure the next new member doesn't conflict
    try {
        int numericId = stoi(id.substr(1));
        if (numericId >= nextMemberId) nextMemberId = numericId + 1;
    } catch (...) { /* Handle potential parsing errors from file */ }
}

string Member::getMemberId() const { return memberId; }

void Member::login() {
    isLoggedIn = true;
    cout << "[Login] Member " << name << " is now online.\n";
}

void Member::logout() {
    isLoggedIn = false;
    cout << "[Logout] Member " << name << " logged out.\n";
}

void Member::displayInfo() const {
    cout << "Member: " << name << " (ID: " << memberId << ", Age: " << age << ")\n";
}


string Member::serialize() const {
    return "MEMBER|" + username + "|" + passwordHash + "|Member|" + 
           name + "|" + to_string(age) + "|" + memberId;
}

// ============================================================
//  Librarian
// ============================================================
Librarian::Librarian(const string& name, int age, const string& uname,
                     const string& pwd, const string& staffId, bool isHashed)
    : Person(name, age), UserAccount(uname, pwd, "Librarian", isHashed),
      staffId(staffId), totalFinesCollected(0.0) {}//multiple inheritance

void Librarian::logAction(const string& action) const {
    cout << "[LOG] Librarian " << staffId << ": " << action << "\n";
}

void Librarian::login() {
    isLoggedIn = true;
    logAction("Logged in");
}

void Librarian::logout() {
    isLoggedIn = false;
    logAction("Logged out");
}

void Librarian::displayInfo() const {
    cout << "Librarian: " << name << " | StaffID: " << staffId << "\n";
}

void Librarian::addBook(Book& book) {
    if (!isLoggedIn) return;
    book.setAvailability(true);
    managedBooks.add(&book);
    logAction("Added book: " + book.getTitle());
}

void Librarian::collectFine(Fine& fine) {
    if (!isLoggedIn || fine.getIsPaid()) return;
    fine.processPayment();
    collectedFines.add(&fine); // Assuming repository stores pointers
    totalFinesCollected += fine.getAmount();
    logAction("Collected fine: " + fine.getReason());
}

double Librarian::calculateTotalFines() const {
    return totalFinesCollected;
}

void Librarian::showCollectedFines() const {
    cout << "--- Fines collected by " << name << " ---\n";
    for (int i = 0; i < collectedFines.getSize(); i++) {
        Fine* f = collectedFines[i];
        if (f) cout << *f << "\n";
    }
}

void Librarian::showManagedBooks() const {
    cout << "--- Books managed by " << name << " ---\n";
    for (int i = 0; i < managedBooks.getSize(); i++) {
        Book* b = managedBooks[i];
        if (b) b->display();
    }
}

string Librarian::serialize() const {
    return "LIBRARIAN|" + username + "|" + passwordHash
           + "|Librarian|" + name + "|" + to_string(age) + "|" + staffId;
}

bool Librarian::operator==(const Librarian& other) const {
    return staffId == other.staffId;
}

Librarian& Librarian::operator=(const Librarian& other) {
    if (this != &other) {
        name = other.name; age = other.age;
        username = other.username; passwordHash = other.passwordHash;
        staffId = other.staffId; totalFinesCollected = other.totalFinesCollected;
    }
    return *this;
}

ostream& operator<<(ostream& os, const Librarian& lib) {
    os << "Librarian: " << lib.name << " (ID: " << lib.staffId << ")";
    return os;
}
