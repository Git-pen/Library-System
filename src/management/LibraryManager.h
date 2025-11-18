// management/LibraryManager.h
#ifndef LIBRARYMANAGER_H
#define LIBRARYMANAGER_H

#include "../entities/Book.h"
#include "../entities/User.h"
#include "../entities/Transaction.h"
#include "BookBST.h"
#include "UserHashMap.h"
#include "TransactionList.h"
#include "SearchEngine.h"
#include "AuthManager.h"
#include "../Config.h"
#include <string>
#include <vector>
using namespace std;

class LibraryManager {
private:
    static LibraryManager* instance;
    
    BookBST* bookTree;
    UserHashMap* userMap;
    TransactionList* transactionList;
    SearchEngine* searchEngine;
    AuthManager* authManager;
    
    // Private constructor (Singleton)
    LibraryManager();
    
    // Private helper methods (no auth check)
    bool addBookInternal(const string& isbn, const string& title, 
                        const string& author, int quantity);

public:
    static LibraryManager* getInstance();
    ~LibraryManager();
    
    void setAuthManager(AuthManager* auth);
    
    // ============ ADMIN OPERATIONS ============
    
    // Book Management
    bool addBook(const string& isbn, const string& title, 
                const string& author, int quantity);
    bool removeBook(const string& isbn);
    bool updateBookDetails(const string& isbn, const string& newTitle, 
                          const string& newAuthor);
    bool updateBookQuantity(const string& isbn, int newQuantity);
    vector<Book*> getAllBooks();
    vector<Book*> getAvailableBooks();
    
    // User Management
    vector<User*> getAllUsers();
    bool removeUser(const string& userID);
    bool deactivateUser(const string& userID);
    bool activateUser(const string& userID);
    User* getUserDetails(const string& userID);
    // In LibraryManager class, add public method:
    UserHashMap* getUserMap() { return userMap; }
    
    // Transaction Management
    vector<Transaction*> getAllTransactions();
    vector<Transaction*> getUserTransactions(const string& userID);
    vector<Transaction*> getBookTransactions(const string& isbn);
    vector<Transaction*> getRecentTransactions(int count);
    
    // Reports & Statistics
    int getTotalBooks();
    int getTotalAvailableBooks();
    int getTotalUsers();
    int getTotalTransactions();
    int getActiveUsersCount();
    
    // ============ USER OPERATIONS ============
    
    // Browse & Search
    vector<Book*> searchBooksByTitle(const string& title);
    vector<Book*> searchBooksByAuthor(const string& author);
    vector<Book*> searchBooksByKeyword(const string& keyword);
    Book* searchBookByISBN(const string& isbn);
    
    // Borrow & Return
    bool borrowBook(const string& isbn);
    bool returnBook(const string& isbn);
    vector<Book*> getMyBorrowedBooks();
    vector<Transaction*> getMyTransactions();
    
    // ============ COMMON OPERATIONS ============
    
    // Data Persistence
    bool saveAllData();
    bool loadAllData();
    void initializeSampleData();
    
    // Validation
    bool isISBNValid(const string& isbn);
    bool isBookAvailable(const string& isbn);
};

#endif // LIBRARYMANAGER_H