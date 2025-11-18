// entities/User.cpp
#include "User.h"
#include "../utils/StringUtils.h"
#include "../Config.h"
#include <sstream>
#include <iomanip>
#include <functional>

// Initialize static counter
int User::userCounter = 1;

// ============ CONSTRUCTORS ============

User::User() : userID(""), username(""), passwordHash(""), fullName(""), 
               email(""), phoneNumber(""), active(true) {}

User::User(string username, string password, string fullName, string email, string phone)
    : username(username), fullName(fullName), email(email), phoneNumber(phone), active(true) {
    this->userID = generateUserID();
    this->passwordHash = hashPassword(password);
}

// ============ AUTHENTICATION ============

string User::hashPassword(const string& password) {
    // Simple hash for demonstration (NOT cryptographically secure)
    // In production, use bcrypt, scrypt, or Argon2
    hash<string> hasher;
    size_t hashValue = hasher(password + "LIBRARY_SALT_2024");
    return to_string(hashValue);
}

bool User::authenticate(const string& password) const {
    return passwordHash == hashPassword(password);
}

// ============ GETTERS ============

string User::getUserID() const { return userID; }
string User::getUsername() const { return username; }
string User::getFullName() const { return fullName; }
string User::getEmail() const { return email; }
string User::getPhoneNumber() const { return phoneNumber; }
int User::getBorrowedCount() const { return borrowedISBNs.size(); }
set<string> User::getBorrowedISBNs() const { return borrowedISBNs; }
bool User::isActive() const { return active; }

// ============ SETTERS ============

void User::setUserID(const string& id) { userID = id; }
void User::setActive(bool status) { active = status; }

void User::updateContact(const string& newEmail, const string& newPhone) {
    email = newEmail;
    phoneNumber = newPhone;
}

// ============ BUSINESS LOGIC ============

bool User::canBorrow() const {
    return active && (getBorrowedCount() < MAX_BORROW_LIMIT);
}

void User::addBorrowedBook(const string& isbn) {
    borrowedISBNs.insert(isbn);  // Set automatically handles duplicates
}

void User::removeBorrowedBook(const string& isbn) {
    borrowedISBNs.erase(isbn);
}

bool User::hasBorrowedBook(const string& isbn) const {
    return borrowedISBNs.find(isbn) != borrowedISBNs.end();
}

// ============ UTILITY METHODS ============

string User::toString() const {
    stringstream ss;
    ss << "User ID: " << userID << "\n"
       << "Username: " << username << "\n"
       << "Full Name: " << fullName << "\n"
       << "Email: " << email << "\n"
       << "Phone: " << phoneNumber << "\n"
       << "Books Borrowed: " << getBorrowedCount() << "/" << MAX_BORROW_LIMIT << "\n"
       << "Status: " << (active ? "Active" : "Inactive");
    return ss.str();
}

string User::toFileString() const {
    // Format: UserID,Username,PasswordHash,FullName,Email,Phone,Active,ISBN1;ISBN2;ISBN3
    stringstream ss;
    ss << StringUtils::escapeCSV(userID) << ","
       << StringUtils::escapeCSV(username) << ","
       << StringUtils::escapeCSV(passwordHash) << ","
       << StringUtils::escapeCSV(fullName) << ","
       << StringUtils::escapeCSV(email) << ","
       << StringUtils::escapeCSV(phoneNumber) << ","
       << (active ? "1" : "0") << ",";
    
    // Add borrowed ISBNs separated by semicolons
    bool first = true;
    for (const auto& isbn : borrowedISBNs) {
        if (!first) ss << ";";
        ss << isbn;
        first = false;
    }
    
    return ss.str();
}

User User::fromFileString(const string& line) {
    vector<string> fields = StringUtils::splitCSV(line);
    
    if (fields.size() < 7) {
        return User();  // Invalid format
    }
    
    User user;
    user.userID = StringUtils::unescapeCSV(fields[0]);
    user.username = StringUtils::unescapeCSV(fields[1]);
    user.passwordHash = StringUtils::unescapeCSV(fields[2]);
    user.fullName = StringUtils::unescapeCSV(fields[3]);
    user.email = StringUtils::unescapeCSV(fields[4]);
    user.phoneNumber = StringUtils::unescapeCSV(fields[5]);
    user.active = (fields[6] == "1");
    
    // Parse borrowed ISBNs (semicolon-separated)
    if (fields.size() > 7 && !fields[7].empty()) {
        stringstream ss(fields[7]);
        string isbn;
        while (getline(ss, isbn, ';')) {
            user.borrowedISBNs.insert(StringUtils::trim(isbn));
        }
    }
    
    return user;
}

// ============ STATIC METHODS ============

string User::generateUserID() {
    stringstream ss;
    ss << "U" << setfill('0') << setw(3) << userCounter++;
    return ss.str();
}