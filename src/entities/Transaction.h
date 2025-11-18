// entities/Transaction.h
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
using namespace std;

class Transaction {
private:
    string transactionID;   // Auto-generated (T001, T002...)
    string userID;          
    string isbn;            
    string type;            // "BORROW" or "RETURN"
    string timestamp;       
    string userName;        // Cached for display
    string bookTitle;       // Cached for display

public:
    // Constructors
    Transaction();
    Transaction(string userID, string isbn, string type, 
                string userName, string bookTitle);
    
    // Getters
    string getTransactionID() const;
    string getUserID() const;
    string getISBN() const;
    string getType() const;
    string getTimestamp() const;
    string getUserName() const;
    string getBookTitle() const;
    
    // Utility
    string toString() const;
    string toFileString() const;
    static Transaction fromFileString(const string& line);
    static string generateTimestamp();
    static string generateID();
    
    // Static counter for ID generation
    static int transactionCounter;
};

#endif // TRANSACTION_H