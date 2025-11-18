// management/TransactionList.h
#ifndef TRANSACTIONLIST_H
#define TRANSACTIONLIST_H

#include "../entities/Transaction.h"
#include <vector>
#include <unordered_map>
using namespace std;

class TransactionList {
private:
    struct TransactionNode {
        Transaction* data;
        TransactionNode* prev;
        TransactionNode* next;
        
        TransactionNode(Transaction* t) 
            : data(t), prev(nullptr), next(nullptr) {}
    };
    
    TransactionNode* head;
    TransactionNode* tail;
    int count;
    
    // Quick access indices
    unordered_map<string, vector<Transaction*>> userTransIndex;   // userID -> transactions
    unordered_map<string, vector<Transaction*>> bookTransIndex;   // ISBN -> transactions
    
public:
    TransactionList();
    ~TransactionList();
    
    // Main operations
    void append(Transaction* trans);        // Add at end O(1)
    void prepend(Transaction* trans);       // Add at beginning O(1)
    vector<Transaction*> getAll() const;
    vector<Transaction*> getByUserID(const string& userID) const;
    vector<Transaction*> getByISBN(const string& isbn) const;
    vector<Transaction*> getRecent(int n) const;
    
    // Utility
    int getCount() const;
    void clear();
};

#endif // TRANSACTIONLIST_H