// management/SearchEngine.h
#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "../entities/Book.h"
#include "../utils/StringUtils.h"
#include "BookBST.h"
#include <map>
#include <vector>
using namespace std;

class SearchEngine {
private:
    // FIX #2: Store ISBNs instead of Book pointers (pointers can become invalid)
    multimap<string, string> titleIndex;    // normalized title -> ISBN
    multimap<string, string> authorIndex;   // normalized author -> ISBN
    
    BookBST* bookTree;  // Reference to book tree for ISBN lookup
    
    // Helper methods
    string normalize(const string& str) const;
    vector<string> tokenize(const string& str) const;
    
public:
    SearchEngine();
    ~SearchEngine();
    
    void setBookTree(BookBST* tree);
    
    // Index management
    void buildIndices();
    void addBookToIndex(const Book& book);
    void removeBookFromIndex(const string& isbn);
    void rebuildIndices();
    void clear();
    
    // Search operations (return pointers from BST)
    vector<Book*> searchByTitle(const string& title) const;
    vector<Book*> searchByAuthor(const string& author) const;
    vector<Book*> searchByKeyword(const string& keyword) const;
    Book* searchByISBN(const string& isbn) const;
    
    // Advanced search
    vector<Book*> searchAvailableBooks() const;
};

#endif // SEARCHENGINE_H