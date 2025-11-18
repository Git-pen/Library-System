// utils/FileHandler.cpp
#include "FileHandler.h"
#include "../utils/StringUtils.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sys/stat.h>  // For directory creation

#ifdef _WIN32
    #include <direct.h>
    #define mkdir _mkdir
#endif

// ============ HELPER: CREATE DIRECTORY ============

bool createDirectory(const string& path) {
    #ifdef _WIN32
        return _mkdir(path.c_str()) == 0 || errno == EEXIST;
    #else
        return mkdir(path.c_str(), 0755) == 0 || errno == EEXIST;
    #endif
}

// ============ FILE OPERATIONS ============

bool FileHandler::fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

bool FileHandler::createFile(const string& filename) {
    // Extract directory path
    size_t lastSlash = filename.find_last_of("/\\");
    if (lastSlash != string::npos) {
        string dir = filename.substr(0, lastSlash);
        createDirectory(dir);
    }
    
    ofstream file(filename);
    if (file.is_open()) {
        file.close();
        return true;
    }
    return false;
}

vector<string> FileHandler::readLines(const string& filename) {
    vector<string> lines;
    ifstream file(filename);
    
    if (!file.is_open()) {
        return lines;  // Empty vector
    }
    
    string line;
    while (getline(file, line)) {
        line = StringUtils::trim(line);
        if (!line.empty() && line[0] != '#') {  // Skip empty lines and comments
            lines.push_back(line);
        }
    }
    
    file.close();
    return lines;
}

bool FileHandler::writeLines(const string& filename, const vector<string>& lines) {
    // Ensure directory exists
    size_t lastSlash = filename.find_last_of("/\\");
    if (lastSlash != string::npos) {
        string dir = filename.substr(0, lastSlash);
        createDirectory(dir);
    }
    
    ofstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file for writing: " << filename << endl;
        return false;
    }
    
    for (const string& line : lines) {
        file << line << endl;
    }
    
    file.close();
    return true;
}

// ============ BOOK OPERATIONS ============

bool FileHandler::saveBooks(const string& filename, BookBST* bookTree) {
    if (bookTree == nullptr) {
        cerr << "Error: BookBST is null" << endl;
        return false;
    }
    
    vector<string> lines;
    lines.push_back("# Library Books Data");
    lines.push_back("# Format: ISBN,Title,Author,Quantity,AvailableCopies");
    
    vector<Book*> books = bookTree->getAllBooksSorted();
    for (Book* book : books) {
        lines.push_back(book->toFileString());
    }
    
    bool success = writeLines(filename, lines);
    if (success) {
        cout << "  ✓ Books saved: " << books.size() << " records" << endl;
    } else {
        cerr << "  ✗ Failed to save books" << endl;
    }
    return success;
}

bool FileHandler::loadBooks(const string& filename, BookBST* bookTree) {
    if (bookTree == nullptr) {
        cerr << "Error: BookBST is null" << endl;
        return false;
    }
    
    if (!fileExists(filename)) {
        cout << "  ℹ No books file found (will be created on first save)" << endl;
        return false;  // Not an error, just no data yet
    }
    
    vector<string> lines = readLines(filename);
    int loaded = 0;
    
    for (const string& line : lines) {
        try {
            Book book = Book::fromFileString(line);
            if (!book.getISBN().empty()) {  // Valid book
                bookTree->insert(book);
                loaded++;
            }
        } catch (...) {
            cerr << "Warning: Skipped invalid book line" << endl;
        }
    }
    
    cout << "  ✓ Books loaded: " << loaded << " records" << endl;
    return loaded > 0;
}

// ============ USER OPERATIONS ============

bool FileHandler::saveUsers(const string& filename, UserHashMap* userMap) {
    if (userMap == nullptr) {
        cerr << "Error: UserHashMap is null" << endl;
        return false;
    }
    
    vector<string> lines;
    lines.push_back("# Library Users Data");
    lines.push_back("# Format: UserID,Username,PasswordHash,FullName,Email,Phone,Active,BorrowedISBNs");
    
    vector<User*> users = userMap->getAllUsers();
    for (User* user : users) {
        lines.push_back(user->toFileString());
    }
    
    bool success = writeLines(filename, lines);
    if (success) {
        cout << "  ✓ Users saved: " << users.size() << " records" << endl;
    } else {
        cerr << "  ✗ Failed to save users" << endl;
    }
    return success;
}

bool FileHandler::loadUsers(const string& filename, UserHashMap* userMap) {
    if (userMap == nullptr) {
        cerr << "Error: UserHashMap is null" << endl;
        return false;
    }
    
    if (!fileExists(filename)) {
        cout << "  ℹ No users file found (will be created on first save)" << endl;
        return false;
    }
    
    vector<string> lines = readLines(filename);
    int maxUserID = 0;
    int loaded = 0;
    
    for (const string& line : lines) {
        try {
            User user = User::fromFileString(line);
            if (!user.getUserID().empty()) {  // Valid user
                // Extract user ID number for counter update
                string idStr = user.getUserID().substr(1);  // Remove 'U'
                int idNum = stoi(idStr);
                maxUserID = max(maxUserID, idNum);
                
                // Insert user (need to allocate on heap)
                User* userPtr = new User(user);
                userMap->insert(userPtr);
                loaded++;
            }
        } catch (...) {
            cerr << "Warning: Skipped invalid user line" << endl;
        }
    }
    
    // FIX #3: Update user counter to prevent duplicate IDs
    User::userCounter = maxUserID + 1;
    
    cout << "  ✓ Users loaded: " << loaded << " records" << endl;
    return loaded > 0;
}

// ============ TRANSACTION OPERATIONS ============

bool FileHandler::saveTransactions(const string& filename, TransactionList* transList) {
    if (transList == nullptr) {
        cerr << "Error: TransactionList is null" << endl;
        return false;
    }
    
    vector<string> lines;
    lines.push_back("# Library Transactions Data");
    lines.push_back("# Format: TransactionID,UserID,ISBN,Type,Timestamp,UserName,BookTitle");
    
    vector<Transaction*> transactions = transList->getAll();
    for (Transaction* trans : transactions) {
        lines.push_back(trans->toFileString());
    }
    
    bool success = writeLines(filename, lines);
    if (success) {
        cout << "  ✓ Transactions saved: " << transactions.size() << " records" << endl;
    } else {
        cerr << "  ✗ Failed to save transactions" << endl;
    }
    return success;
}

bool FileHandler::loadTransactions(const string& filename, TransactionList* transList) {
    if (transList == nullptr) {
        cerr << "Error: TransactionList is null" << endl;
        return false;
    }
    
    if (!fileExists(filename)) {
        cout << "  ℹ No transactions file found (will be created on first save)" << endl;
        return false;
    }
    
    vector<string> lines = readLines(filename);
    int maxTransID = 0;
    int loaded = 0;
    
    for (const string& line : lines) {
        try {
            Transaction trans = Transaction::fromFileString(line);
            if (!trans.getTransactionID().empty()) {  // Valid transaction
                // Extract transaction ID number for counter update
                string idStr = trans.getTransactionID().substr(1);  // Remove 'T'
                int idNum = stoi(idStr);
                maxTransID = max(maxTransID, idNum);
                
                // Insert transaction (allocate on heap)
                Transaction* transPtr = new Transaction(trans);
                transList->append(transPtr);
                loaded++;
            }
        } catch (...) {
            cerr << "Warning: Skipped invalid transaction line" << endl;
        }
    }
    
    // FIX #3: Update transaction counter to prevent duplicate IDs
    Transaction::transactionCounter = maxTransID + 1;
    
    cout << "  ✓ Transactions loaded: " << loaded << " records" << endl;
    return loaded > 0;
}