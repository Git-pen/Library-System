// management/UserHashMap.cpp
#include "UserHashMap.h"

// ============ CONSTRUCTOR & DESTRUCTOR ============

UserHashMap::UserHashMap() : UserHashMap(INITIAL_HASH_TABLE_SIZE) {}

UserHashMap::UserHashMap(int size) : tableSize(size), count(0) {
    userIDTable = new HashNode*[tableSize];
    usernameTable = new HashNode*[tableSize];
    
    for (int i = 0; i < tableSize; i++) {
        userIDTable[i] = nullptr;
        usernameTable[i] = nullptr;
    }
}

UserHashMap::~UserHashMap() {
    clear();
    delete[] userIDTable;
    delete[] usernameTable;
}

void UserHashMap::clear() {
    clearTable(userIDTable);
    clearTable(usernameTable);
    count = 0;
}

void UserHashMap::clearTable(HashNode** table) {
    for (int i = 0; i < tableSize; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            
            // Only delete User* once (from userIDTable)
            if (table == userIDTable) {
                delete temp->value;
            }
            delete temp;
        }
        table[i] = nullptr;
    }
}

// ============ HASH FUNCTION ============

int UserHashMap::hashFunction(const string& key) const {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash % tableSize;
}

// ============ INSERT ============

void UserHashMap::insert(User* user) {
    // FIX #5: Validate both tables before inserting
    if (existsInTable(userIDTable, user->getUserID()) ||
        existsInTable(usernameTable, user->getUsername())) {
        return;  // Already exists, don't insert
    }
    
    // Insert into both tables
    insertIntoTable(userIDTable, user->getUserID(), user);
    insertIntoTable(usernameTable, user->getUsername(), user);
    count++;
}

void UserHashMap::insertIntoTable(HashNode** table, const string& key, User* user) {
    int index = hashFunction(key);
    
    // Insert at beginning of chain (O(1))
    HashNode* newNode = new HashNode(key, user);
    newNode->next = table[index];
    table[index] = newNode;
}

// ============ SEARCH ============

User* UserHashMap::searchByID(const string& userID) const {
    return searchInTable(userIDTable, userID);
}

User* UserHashMap::searchByUsername(const string& username) const {
    return searchInTable(usernameTable, username);
}

User* UserHashMap::searchInTable(HashNode** table, const string& key) const {
    int index = hashFunction(key);
    HashNode* current = table[index];
    
    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return nullptr;
}

// ============ REMOVE ============

bool UserHashMap::remove(const string& userID) {
    // FIX #5: Get username before removing
    User* user = searchByID(userID);
    if (user == nullptr) {
        return false;
    }
    
    string username = user->getUsername();
    
    // Remove from both tables
    bool removed1 = removeFromTable(userIDTable, userID);
    bool removed2 = removeFromTable(usernameTable, username);
    
    if (removed1 && removed2) {
        delete user;  // Delete the User object
        count--;
        return true;
    }
    
    return false;
}

bool UserHashMap::removeFromTable(HashNode** table, const string& key) {
    int index = hashFunction(key);
    HashNode* current = table[index];
    HashNode* prev = nullptr;
    
    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;  // Delete the node (but not the User*)
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

// ============ UTILITY ============

int UserHashMap::getCount() const {
    return count;
}

bool UserHashMap::existsUsername(const string& username) const {
    return existsInTable(usernameTable, username);
}

bool UserHashMap::existsUserID(const string& userID) const {
    return existsInTable(userIDTable, userID);
}

bool UserHashMap::existsInTable(HashNode** table, const string& key) const {
    return searchInTable(table, key) != nullptr;
}

vector<User*> UserHashMap::getAllUsers() const {
    vector<User*> result;
    
    for (int i = 0; i < tableSize; i++) {
        HashNode* current = userIDTable[i];
        while (current != nullptr) {
            result.push_back(current->value);
            current = current->next;
        }
    }
    
    return result;
}