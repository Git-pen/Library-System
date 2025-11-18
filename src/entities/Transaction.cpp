// entities/Transaction.cpp
#include "Transaction.h"
#include "../utils/StringUtils.h"
#include <sstream>
#include <iomanip>
#include <ctime>

// Initialize static counter
int Transaction::transactionCounter = 1;

// ============ CONSTRUCTORS ============

Transaction::Transaction() 
    : transactionID(""), userID(""), isbn(""), type(""), 
      timestamp(""), userName(""), bookTitle("") {}

Transaction::Transaction(string userID, string isbn, string type, 
                        string userName, string bookTitle)
    : userID(userID), isbn(isbn), type(type), userName(userName), bookTitle(bookTitle) {
    this->transactionID = generateID();
    this->timestamp = generateTimestamp();
}

// ============ GETTERS ============

string Transaction::getTransactionID() const { return transactionID; }
string Transaction::getUserID() const { return userID; }
string Transaction::getISBN() const { return isbn; }
string Transaction::getType() const { return type; }
string Transaction::getTimestamp() const { return timestamp; }
string Transaction::getUserName() const { return userName; }
string Transaction::getBookTitle() const { return bookTitle; }

// ============ UTILITY METHODS ============

string Transaction::toString() const {
    stringstream ss;
    ss << "Transaction ID: " << transactionID << "\n"
       << "Type: " << type << "\n"
       << "User: " << userName << " (" << userID << ")\n"
       << "Book: " << bookTitle << " (" << isbn << ")\n"
       << "Date: " << timestamp;
    return ss.str();
}

string Transaction::toFileString() const {
    // Format: TransactionID,UserID,ISBN,Type,Timestamp,UserName,BookTitle
    stringstream ss;
    ss << StringUtils::escapeCSV(transactionID) << ","
       << StringUtils::escapeCSV(userID) << ","
       << StringUtils::escapeCSV(isbn) << ","
       << StringUtils::escapeCSV(type) << ","
       << StringUtils::escapeCSV(timestamp) << ","
       << StringUtils::escapeCSV(userName) << ","
       << StringUtils::escapeCSV(bookTitle);
    return ss.str();
}

Transaction Transaction::fromFileString(const string& line) {
    vector<string> fields = StringUtils::splitCSV(line);
    
    if (fields.size() < 7) {
        return Transaction();  // Invalid format
    }
    
    Transaction trans;
    trans.transactionID = StringUtils::unescapeCSV(fields[0]);
    trans.userID = StringUtils::unescapeCSV(fields[1]);
    trans.isbn = StringUtils::unescapeCSV(fields[2]);
    trans.type = StringUtils::unescapeCSV(fields[3]);
    trans.timestamp = StringUtils::unescapeCSV(fields[4]);
    trans.userName = StringUtils::unescapeCSV(fields[5]);
    trans.bookTitle = StringUtils::unescapeCSV(fields[6]);
    
    return trans;
}

// ============ STATIC METHODS ============

string Transaction::generateTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    stringstream ss;
    ss << setfill('0')
       << setw(4) << (1900 + ltm->tm_year) << "-"
       << setw(2) << (1 + ltm->tm_mon) << "-"
       << setw(2) << ltm->tm_mday << " "
       << setw(2) << ltm->tm_hour << ":"
       << setw(2) << ltm->tm_min << ":"
       << setw(2) << ltm->tm_sec;
    
    return ss.str();
}

string Transaction::generateID() {
    stringstream ss;
    ss << "T" << setfill('0') << setw(4) << transactionCounter++;
    return ss.str();
}