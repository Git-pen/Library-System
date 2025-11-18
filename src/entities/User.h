// entities/User.h
#ifndef USER_H
#define USER_H

#include <string>
#include <set>
using namespace std;

class User {
private:
    string userID;          // Auto-generated (U001, U002...)
    string username;        // For login (unique)
    string passwordHash;    // Hashed password
    string fullName;        
    string email;           
    string phoneNumber;     
    set<string> borrowedISBNs;  // Set prevents duplicates, O(log n) lookup
    bool active;            // Account status

public:
    // Constructors
    User();
    User(string username, string password, string fullName, string email, string phone);
    
    // Authentication
    bool authenticate(const string& password) const;
    static string hashPassword(const string& password);
    
    // Getters
    string getUserID() const;
    string getUsername() const;
    string getFullName() const;
    string getEmail() const;
    string getPhoneNumber() const;
    int getBorrowedCount() const;  // Computed from set size
    set<string> getBorrowedISBNs() const;
    bool isActive() const;
    
    // Setters
    void setUserID(const string& id);
    void setActive(bool status);
    void updateContact(const string& email, const string& phone);
    
    // Business Logic
    bool canBorrow() const;  // Check if under limit
    void addBorrowedBook(const string& isbn);
    void removeBorrowedBook(const string& isbn);
    bool hasBorrowedBook(const string& isbn) const;
    
    // Utility
    string toString() const;
    string toFileString() const;
    static User fromFileString(const string& line);
    
    // Static counter for ID generation
    static int userCounter;
    static string generateUserID();
};

#endif // USER_H