// management/BookBST.h
#ifndef BOOKBST_H
#define BOOKBST_H

#include "../entities/Book.h"
#include <vector>
#include <algorithm>
using namespace std;

class BookBST {
private:
    struct BookNode {
        Book data;           // Store book by value (FIX #1 - no pointers!)
        BookNode* left;
        BookNode* right;
        int height;
        
        BookNode(const Book& book) 
            : data(book), left(nullptr), right(nullptr), height(1) {}
    };
    
    BookNode* root;
    int nodeCount;
    
    // Private helper methods
    BookNode* insert(BookNode* node, const Book& book);
    BookNode* search(BookNode* node, const string& isbn);
    BookNode* deleteNode(BookNode* node, const string& isbn);
    BookNode* findMin(BookNode* node);
    void inorderTraversal(BookNode* node, vector<Book*>& result);
    void destroy(BookNode* node);
    
    // AVL balancing methods
    int getHeight(BookNode* node);
    int getBalance(BookNode* node);
    BookNode* rotateRight(BookNode* y);
    BookNode* rotateLeft(BookNode* x);
    
public:
    BookBST();
    ~BookBST();
    
    // Main operations
    void insert(const Book& book);
    Book* search(const string& isbn);
    bool remove(const string& isbn);
    vector<Book*> getAllBooksSorted();
    
    // Utility
    int getCount() const;
    bool isEmpty() const;
    void clear();
};

#endif // BOOKBST_H