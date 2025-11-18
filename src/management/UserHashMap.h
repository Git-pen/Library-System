// management/UserHashMap.h
#ifndef USERHASHMAP_H
#define USERHASHMAP_H

#include "../entities/User.h"
#include "../Config.h"
#include <vector>
using namespace std;

class UserHashMap {
private:
    struct HashNode {
        string key;
        User* value;
        HashNode* next;
        
        HashNode(string k, User* v) : key(k), value(v), next(nullptr) {}
    };
    
    HashNode** userIDTable;      // Hash by userID
    HashNode** usernameTable;    // Hash by username
    int tableSize;
    int count;
    
    // Private helpers
    int hashFunction(const string& key) const;
    void insertIntoTable(HashNode** table, const string& key, User* user);
    User* searchInTable(HashNode** table, const string& key) const;
    bool removeFromTable(HashNode** table, const string& key);
    bool existsInTable(HashNode** table, const string& key) const;
    void clearTable(HashNode** table);
    
public:
    UserHashMap();
    UserHashMap(int size);
    ~UserHashMap();
    
    // Main operations
    void insert(User* user);
    User* searchByID(const string& userID) const;
    User* searchByUsername(const string& username) const;
    bool remove(const string& userID);
    vector<User*> getAllUsers() const;
    
    // Utility
    int getCount() const;
    bool existsUsername(const string& username) const;
    bool existsUserID(const string& userID) const;
    void clear();
};

#endif // USERHASHMAP_H