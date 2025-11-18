// Book.h
#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book {
private:
    string isbn;           // Unique identifier
    string title;          // Book title
    string author;         // Book author
    int quantity;          // Total copies
    int availableCopies;   // Available copies

public:
    // Constructors
    Book();
    Book(string isbn, string title, string author, int quantity);
    
    // Getters
    string getISBN() const;
    string getTitle() const;
    string getAuthor() const;
    int getQuantity() const;
    int getAvailableCopies() const;
    bool isAvailable() const;  // Computed: availableCopies > 0
    
    // Setters
    void setQuantity(int qty);
    void setAvailableCopies(int copies);
    
    // Business Logic
    bool borrowBook();   // Decrease available copies
    bool returnBook();   // Increase available copies
    
    // Utility
    string toString() const;           // For display
    string toFileString() const;       // For saving to CSV
    static Book fromFileString(string line);  // For loading from CSV
};

#endif // BOOK_H