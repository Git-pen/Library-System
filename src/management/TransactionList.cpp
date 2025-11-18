// management/TransactionList.cpp
#include "TransactionList.h"

// ============ CONSTRUCTOR & DESTRUCTOR ============

TransactionList::TransactionList() : head(nullptr), tail(nullptr), count(0) {}

TransactionList::~TransactionList() {
    clear();
}

void TransactionList::clear() {
    TransactionNode* current = head;
    while (current != nullptr) {
        TransactionNode* temp = current;
        current = current->next;
        delete temp->data;  // Delete Transaction object
        delete temp;        // Delete node
    }
    head = tail = nullptr;
    count = 0;
    userTransIndex.clear();
    bookTransIndex.clear();
}

// ============ APPEND (Add at end) ============

void TransactionList::append(Transaction* trans) {
    TransactionNode* newNode = new TransactionNode(trans);
    
    if (tail == nullptr) {
        // Empty list
        head = tail = newNode;
    } else {
        // Add to end
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    
    count++;
    
    // Update indices for O(1) lookup
    userTransIndex[trans->getUserID()].push_back(trans);
    bookTransIndex[trans->getISBN()].push_back(trans);
}

// ============ PREPEND (Add at beginning) ============

void TransactionList::prepend(Transaction* trans) {
    TransactionNode* newNode = new TransactionNode(trans);
    
    if (head == nullptr) {
        // Empty list
        head = tail = newNode;
    } else {
        // Add to beginning
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    
    count++;
    
    // Update indices
    userTransIndex[trans->getUserID()].push_back(trans);
    bookTransIndex[trans->getISBN()].push_back(trans);
}

// ============ RETRIEVAL ============

vector<Transaction*> TransactionList::getAll() const {
    vector<Transaction*> result;
    TransactionNode* current = head;
    
    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }
    
    return result;
}

vector<Transaction*> TransactionList::getByUserID(const string& userID) const {
    auto it = userTransIndex.find(userID);
    if (it != userTransIndex.end()) {
        return it->second;
    }
    return vector<Transaction*>();  // Empty vector
}

vector<Transaction*> TransactionList::getByISBN(const string& isbn) const {
    auto it = bookTransIndex.find(isbn);
    if (it != bookTransIndex.end()) {
        return it->second;
    }
    return vector<Transaction*>();
}

vector<Transaction*> TransactionList::getRecent(int n) const {
    vector<Transaction*> result;
    TransactionNode* current = tail;  // Start from end
    
    int count = 0;
    while (current != nullptr && count < n) {
        result.push_back(current->data);
        current = current->prev;  // Move backward
        count++;
    }
    
    return result;  // Most recent first
}

// ============ UTILITY ============

int TransactionList::getCount() const {
    return count;
}