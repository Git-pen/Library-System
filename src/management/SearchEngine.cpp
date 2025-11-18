// management/SearchEngine.cpp
#include "SearchEngine.h"
#include <sstream>
#include <algorithm>

// ============ CONSTRUCTOR & DESTRUCTOR ============

SearchEngine::SearchEngine() : bookTree(nullptr) {}

SearchEngine::~SearchEngine() {
    clear();
}

void SearchEngine::setBookTree(BookBST* tree) {
    bookTree = tree;
}

void SearchEngine::clear() {
    titleIndex.clear();
    authorIndex.clear();
}

// ============ HELPER METHODS ============

string SearchEngine::normalize(const string& str) const {
    string normalized = StringUtils::toLower(str);
    normalized = StringUtils::trim(normalized);
    return normalized;
}

vector<string> SearchEngine::tokenize(const string& str) const {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while (ss >> token) {
        // Remove punctuation
        token.erase(remove_if(token.begin(), token.end(), ::ispunct), token.end());
        if (!token.empty()) {
            tokens.push_back(StringUtils::toLower(token));
        }
    }
    
    return tokens;
}

// ============ INDEX MANAGEMENT ============

void SearchEngine::buildIndices() {
    if (bookTree == nullptr) return;
    
    clear();
    
    vector<Book*> allBooks = bookTree->getAllBooksSorted();
    for (Book* book : allBooks) {
        addBookToIndex(*book);
    }
}

void SearchEngine::addBookToIndex(const Book& book) {
    string isbn = book.getISBN();
    
    // Index by full title (normalized)
    string normalizedTitle = normalize(book.getTitle());
    titleIndex.insert({normalizedTitle, isbn});
    
    // Index by each word in title
    vector<string> titleWords = tokenize(book.getTitle());
    for (const string& word : titleWords) {
        if (word.length() > 2) {  // Skip very short words
            titleIndex.insert({word, isbn});
        }
    }
    
    // Index by full author name (normalized)
    string normalizedAuthor = normalize(book.getAuthor());
    authorIndex.insert({normalizedAuthor, isbn});
    
    // Index by each word in author name
    vector<string> authorWords = tokenize(book.getAuthor());
    for (const string& word : authorWords) {
        if (word.length() > 2) {
            authorIndex.insert({word, isbn});
        }
    }
}

void SearchEngine::removeBookFromIndex(const string& isbn) {
    // Remove from title index
    for (auto it = titleIndex.begin(); it != titleIndex.end(); ) {
        if (it->second == isbn) {
            it = titleIndex.erase(it);
        } else {
            ++it;
        }
    }
    
    // Remove from author index
    for (auto it = authorIndex.begin(); it != authorIndex.end(); ) {
        if (it->second == isbn) {
            it = authorIndex.erase(it);
        } else {
            ++it;
        }
    }
}

void SearchEngine::rebuildIndices() {
    buildIndices();
}

// ============ SEARCH OPERATIONS ============

vector<Book*> SearchEngine::searchByTitle(const string& title) const {
    if (bookTree == nullptr) return vector<Book*>();
    
    string normalized = normalize(title);
    vector<string> foundISBNs;
    
    // Find all matching ISBNs
    auto range = titleIndex.equal_range(normalized);
    for (auto it = range.first; it != range.second; ++it) {
        foundISBNs.push_back(it->second);
    }
    
    // If no exact match, try tokenized search
    if (foundISBNs.empty()) {
        vector<string> words = tokenize(title);
        for (const string& word : words) {
            auto wordRange = titleIndex.equal_range(word);
            for (auto it = wordRange.first; it != wordRange.second; ++it) {
                foundISBNs.push_back(it->second);
            }
        }
    }
    
    // Remove duplicates
    sort(foundISBNs.begin(), foundISBNs.end());
    foundISBNs.erase(unique(foundISBNs.begin(), foundISBNs.end()), foundISBNs.end());
    
    // Convert ISBNs to Book pointers
    vector<Book*> results;
    for (const string& isbn : foundISBNs) {
        Book* book = bookTree->search(isbn);
        if (book != nullptr) {
            results.push_back(book);
        }
    }
    
    return results;
}

vector<Book*> SearchEngine::searchByAuthor(const string& author) const {
    if (bookTree == nullptr) return vector<Book*>();
    
    string normalized = normalize(author);
    vector<string> foundISBNs;
    
    // Find all matching ISBNs
    auto range = authorIndex.equal_range(normalized);
    for (auto it = range.first; it != range.second; ++it) {
        foundISBNs.push_back(it->second);
    }
    
    // If no exact match, try tokenized search
    if (foundISBNs.empty()) {
        vector<string> words = tokenize(author);
        for (const string& word : words) {
            auto wordRange = authorIndex.equal_range(word);
            for (auto it = wordRange.first; it != wordRange.second; ++it) {
                foundISBNs.push_back(it->second);
            }
        }
    }
    
    // Remove duplicates
    sort(foundISBNs.begin(), foundISBNs.end());
    foundISBNs.erase(unique(foundISBNs.begin(), foundISBNs.end()), foundISBNs.end());
    
    // Convert ISBNs to Book pointers
    vector<Book*> results;
    for (const string& isbn : foundISBNs) {
        Book* book = bookTree->search(isbn);
        if (book != nullptr) {
            results.push_back(book);
        }
    }
    
    return results;
}

vector<Book*> SearchEngine::searchByKeyword(const string& keyword) const {
    // Search in both title and author
    vector<Book*> titleResults = searchByTitle(keyword);
    vector<Book*> authorResults = searchByAuthor(keyword);
    
    // Combine and remove duplicates
    vector<Book*> combined = titleResults;
    combined.insert(combined.end(), authorResults.begin(), authorResults.end());
    
    sort(combined.begin(), combined.end());
    combined.erase(unique(combined.begin(), combined.end()), combined.end());
    
    return combined;
}

Book* SearchEngine::searchByISBN(const string& isbn) const {
    if (bookTree == nullptr) return nullptr;
    return bookTree->search(isbn);
}

vector<Book*> SearchEngine::searchAvailableBooks() const {
    if (bookTree == nullptr) return vector<Book*>();
    
    vector<Book*> allBooks = bookTree->getAllBooksSorted();
    vector<Book*> available;
    
    for (Book* book : allBooks) {
        if (book->isAvailable()) {
            available.push_back(book);
        }
    }
    
    return available;
}