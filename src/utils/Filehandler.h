// utils/FileHandler.h
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "../entities/Book.h"
#include "../entities/User.h"
#include "../entities/Transaction.h"
#include "../management/BookBST.h"
#include "../management/UserHashMap.h"
#include "../management/TransactionList.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class FileHandler {
public:
    // File operations
    static bool fileExists(const string& filename);
    static bool createFile(const string& filename);
    static vector<string> readLines(const string& filename);
    static bool writeLines(const string& filename, const vector<string>& lines);
    
    // Data-specific operations
    static bool saveBooks(const string& filename, BookBST* bookTree);
    static bool loadBooks(const string& filename, BookBST* bookTree);
    
    static bool saveUsers(const string& filename, UserHashMap* userMap);
    static bool loadUsers(const string& filename, UserHashMap* userMap);
    
    static bool saveTransactions(const string& filename, TransactionList* transList);
    static bool loadTransactions(const string& filename, TransactionList* transList);
};

#endif // FILEHANDLER_H