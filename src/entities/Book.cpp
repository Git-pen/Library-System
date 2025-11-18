// Book.cpp
#include "Book.h"
#include <sstream>
#include <iomanip>
#include "../utils/StringUtils.h"

// ============ CONSTRUCTORS ============

Book::Book() : isbn(""), title(""), author(""), quantity(0), availableCopies(0) {}

Book::Book(string isbn, string title, string author, int quantity) 
    : isbn(isbn), title(title), author(author), quantity(quantity), availableCopies(quantity) {}

// ============ GETTERS ============

string Book::getISBN() const { return isbn; }
string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
int Book::getQuantity() const { return quantity; }
int Book::getAvailableCopies() const { return availableCopies; }
bool Book::isAvailable() const { return availableCopies > 0; }

// ============ SETTERS ============

void Book::setQuantity(int qty) {
    quantity = qty;
    // Adjust available copies if needed
    if (availableCopies > quantity) {
        availableCopies = quantity;
    }
}

void Book::setAvailableCopies(int copies) {
    if (copies >= 0 && copies <= quantity) {
        availableCopies = copies;
    }
}

// ============ BUSINESS LOGIC ============

bool Book::borrowBook() {
    if (availableCopies > 0) {
        availableCopies--;
        return true;
    }
    return false;
}

bool Book::returnBook() {
    if (availableCopies < quantity) {
        availableCopies++;
        return true;
    }
    return false;  // Can't return more than total quantity
}

// ============ UTILITY METHODS ============

string Book::toString() const {
    stringstream ss;
    ss << "ISBN: " << isbn << "\n"
       << "Title: " << title << "\n"
       << "Author: " << author << "\n"
       << "Total Copies: " << quantity << "\n"
       << "Available: " << availableCopies << "\n"
       << "Status: " << (isAvailable() ? "Available" : "Not Available");
    return ss.str();
}

string Book::toFileString() const {
    // Format: ISBN,Title,Author,Quantity,AvailableCopies
    stringstream ss;
    ss << StringUtils::escapeCSV(isbn) << ","
       << StringUtils::escapeCSV(title) << ","
       << StringUtils::escapeCSV(author) << ","
       << quantity << ","
       << availableCopies;
    return ss.str();
}

Book Book::fromFileString(string line) {
    vector<string> fields = StringUtils::splitCSV(line);
    
    if (fields.size() < 5) {
        // Invalid format, return empty book
        return Book();
    }
    
    string isbn = StringUtils::unescapeCSV(fields[0]);
    string title = StringUtils::unescapeCSV(fields[1]);
    string author = StringUtils::unescapeCSV(fields[2]);
    int quantity = stoi(fields[3]);
    int availableCopies = stoi(fields[4]);
    
    Book book(isbn, title, author, quantity);
    book.setAvailableCopies(availableCopies);
    
    return book;
}