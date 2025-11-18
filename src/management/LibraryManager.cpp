// management/LibraryManager.cpp
#include "LibraryManager.h"
#include "../utils/FileHandler.h"
#include <iostream>
#include <algorithm>

// Initialize static instance
LibraryManager* LibraryManager::instance = nullptr;

// ============ SINGLETON ============

LibraryManager::LibraryManager() {
    bookTree = new BookBST();
    userMap = new UserHashMap();
    transactionList = new TransactionList();
    searchEngine = new SearchEngine();
    authManager = nullptr;
    
    // Link search engine to book tree
    searchEngine->setBookTree(bookTree);
}

LibraryManager* LibraryManager::getInstance() {
    if (instance == nullptr) {
        instance = new LibraryManager();
    }
    return instance;
}

LibraryManager::~LibraryManager() {
    delete bookTree;
    delete userMap;
    delete transactionList;
    delete searchEngine;
    // Don't delete authManager (owned externally)
}

void LibraryManager::setAuthManager(AuthManager* auth) {
    authManager = auth;
}

// ============ ADMIN OPERATIONS - BOOK MANAGEMENT ============

bool LibraryManager::addBook(const string& isbn, const string& title, 
                             const string& author, int quantity) {
    // Check admin privileges
    if (authManager == nullptr || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required." << endl;
        return false;
    }
    
    return addBookInternal(isbn, title, author, quantity);
}

// FIX #4: Private helper without auth check (for initialization)
bool LibraryManager::addBookInternal(const string& isbn, const string& title, 
                                    const string& author, int quantity) {
    // Validate ISBN
    if (!isISBNValid(isbn)) {
        cout << "Error: Invalid ISBN format." << endl;
        return false;
    }
    
    // Check if book already exists
    Book* existing = bookTree->search(isbn);
    if (existing != nullptr) {
        cout << "Error: Book with ISBN " << isbn << " already exists." << endl;
        return false;
    }
    
    // Create and insert book
    Book newBook(isbn, title, author, quantity);
    bookTree->insert(newBook);
    
    // Update search indices
    searchEngine->addBookToIndex(newBook);
    
    cout << "Success: Book added successfully." << endl;
    return true;
}

bool LibraryManager::removeBook(const string& isbn) {
    if (authManager == nullptr || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required." << endl;
        return false;
    }
    
    Book* book = bookTree->search(isbn);
    if (book == nullptr) {
        cout << "Error: Book not found." << endl;
        return false;
    }
    
    // Check if any copies are currently borrowed
    int borrowed = book->getQuantity() - book->getAvailableCopies();
    if (borrowed > 0) {
        cout << "Error: Cannot remove book. " << borrowed 
             << " copies are currently borrowed." << endl;
        return false;
    }
    
    // Remove from indices first
    searchEngine->removeBookFromIndex(isbn);
    
    // Remove from tree
    if (bookTree->remove(isbn)) {
        cout << "Success: Book removed successfully." << endl;
        return true;
    }
    
    return false;
}

bool LibraryManager::updateBookDetails(const string& isbn, const string& newTitle, 
                                       const string& newAuthor) {
    if (authManager == nullptr || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required." << endl;
        return false;
    }
    
    Book* book = bookTree->search(isbn);
    if (book == nullptr) {
        cout << "Error: Book not found." << endl;
        return false;
    }
    
    // Update book details
    Book updatedBook(isbn, newTitle, newAuthor, book->getQuantity());
    updatedBook.setAvailableCopies(book->getAvailableCopies());
    
    // Remove old indices
    searchEngine->removeBookFromIndex(isbn);
    
    // Update in tree
    bookTree->insert(updatedBook);  // Will replace existing
    
    // Add new indices
    searchEngine->addBookToIndex(updatedBook);
    
    cout << "Success: Book details updated." << endl;
    return true;
}

bool LibraryManager::updateBookQuantity(const string& isbn, int newQuantity) {
    if (authManager == nullptr || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required." << endl;
        return false;
    }
    
    Book* book = bookTree->search(isbn);
    if (book == nullptr) {
        cout << "Error: Book not found." << endl;
        return false;
    }
    
    int borrowed = book->getQuantity() - book->getAvailableCopies();
    
    if (newQuantity < borrowed) {
        cout << "Error: Cannot reduce quantity below borrowed copies (" 
             << borrowed << ")." << endl;
        return false;
    }
    
    int difference = newQuantity - book->getQuantity();
    book->setQuantity(newQuantity);
    book->setAvailableCopies(book->getAvailableCopies() + difference);
    
    cout << "Success: Book quantity updated." << endl;
    return true;
}

vector<Book*> LibraryManager::getAllBooks() {
    if (authManager == nullptr || !authManager->isAdmin()) {
        return vector<Book*>();
    }
    return bookTree->getAllBooksSorted();
}

vector<Book*> LibraryManager::getAvailableBooks() {
    return searchEngine->searchAvailableBooks();
}

// ============ ADMIN OPERATIONS - USER MANAGEMENT ============

vector<User*> LibraryManager::getAllUsers() {
    if (authManager == nullptr || !authManager->isAdmin()) {
        return vector<User*>();
    }
    return userMap->getAllUsers();
}

bool LibraryManager::removeUser(const string& userID) {
    if (authManager == nullptr || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required." << endl;
        return false;
    }
    
    User* user = userMap->searchByID(userID);
    if (user == nullptr) {
        cout << "Error: User not found." << endl;
        return false;
    }
    
    // Check if user has borrowed books
    if (user->getBorrowedCount() > 0) {
        cout << "Error: Cannot remove user. User has " 
             << user->getBorrowedCount() << " borrowed books." << endl;
        return false;
    }
    
    if (userMap->remove(userID)) {
        cout << "Success: User removed successfully." << endl;
        return true;
    }
    
    return false;
}

bool LibraryManager::deactivateUser(const string& userID) {
    if (authManager == nullptr || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required." << endl;
        return false;
    }
    
    User* user = userMap->searchByID(userID);
    if (user == nullptr) {
        cout << "Error: User not found." << endl;
        return false;
    }
    
    user->setActive(false);
    cout << "Success: User account deactivated." << endl;
    return true;
}

bool LibraryManager::activateUser(const string& userID) {
    if (authManager == nullptr || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required." << endl;
        return false;
    }
    
    User* user = userMap->searchByID(userID);
    if (user == nullptr) {
        cout << "Error: User not found." << endl;
        return false;
    }
    
    user->setActive(true);
    cout << "Success: User account activated." << endl;
    return true;
}

User* LibraryManager::getUserDetails(const string& userID) {
    if (authManager == nullptr || !authManager->isAdmin()) {
        return nullptr;
    }
    return userMap->searchByID(userID);
}

// ============ ADMIN OPERATIONS - TRANSACTIONS ============

vector<Transaction*> LibraryManager::getAllTransactions() {
    if (authManager == nullptr || !authManager->isAdmin()) {
        return vector<Transaction*>();
    }
    return transactionList->getAll();
}

vector<Transaction*> LibraryManager::getUserTransactions(const string& userID) {
    if (authManager == nullptr || !authManager->isAdmin()) {
        return vector<Transaction*>();
    }
    return transactionList->getByUserID(userID);
}

vector<Transaction*> LibraryManager::getBookTransactions(const string& isbn) {
    if (authManager == nullptr || !authManager->isAdmin()) {
        return vector<Transaction*>();
    }
    return transactionList->getByISBN(isbn);
}

vector<Transaction*> LibraryManager::getRecentTransactions(int count) {
    if (authManager == nullptr || !authManager->isAdmin()) {
        return vector<Transaction*>();
    }
    return transactionList->getRecent(count);
}

// ============ ADMIN OPERATIONS - STATISTICS ============

int LibraryManager::getTotalBooks() {
    return bookTree->getCount();
}

int LibraryManager::getTotalAvailableBooks() {
    vector<Book*> books = bookTree->getAllBooksSorted();
    int availableCount = 0;
    for (Book* book : books) {
        availableCount += book->getAvailableCopies();
    }
    return availableCount;
}

int LibraryManager::getTotalUsers() {
    return userMap->getCount();
}

int LibraryManager::getTotalTransactions() {
    return transactionList->getCount();
}

int LibraryManager::getActiveUsersCount() {
    vector<User*> users = userMap->getAllUsers();
    int activeCount = 0;
    for (User* user : users) {
        if (user->isActive()) {
            activeCount++;
        }
    }
    return activeCount;
}

// ============ USER OPERATIONS - SEARCH ============

vector<Book*> LibraryManager::searchBooksByTitle(const string& title) {
    return searchEngine->searchByTitle(title);
}

vector<Book*> LibraryManager::searchBooksByAuthor(const string& author) {
    return searchEngine->searchByAuthor(author);
}

vector<Book*> LibraryManager::searchBooksByKeyword(const string& keyword) {
    return searchEngine->searchByKeyword(keyword);
}

Book* LibraryManager::searchBookByISBN(const string& isbn) {
    return searchEngine->searchByISBN(isbn);
}

// ============ USER OPERATIONS - BORROW & RETURN ============

bool LibraryManager::borrowBook(const string& isbn) {
    // Check if user is logged in
    if (authManager == nullptr || !authManager->isUser()) {
        cout << "Access Denied: Please login as user." << endl;
        return false;
    }
    
    User* currentUser = authManager->getCurrentUser();
    if (currentUser == nullptr) {
        cout << "Error: User session invalid." << endl;
        return false;
    }
    
    // Check if user can borrow
    if (!currentUser->canBorrow()) {
        cout << "Error: Borrowing limit reached (" << MAX_BORROW_LIMIT 
             << " books)." << endl;
        return false;
    }
    
    // Find book
    Book* book = bookTree->search(isbn);
    if (book == nullptr) {
        cout << "Error: Book not found." << endl;
        return false;
    }
    
    // Check if book is available
    if (!book->isAvailable()) {
        cout << "Error: Book is not available." << endl;
        return false;
    }
    
    // Check if user already has this book
    if (currentUser->hasBorrowedBook(isbn)) {
        cout << "Error: You have already borrowed this book." << endl;
        return false;
    }
    
    // Perform borrowing
    if (book->borrowBook()) {
        currentUser->addBorrowedBook(isbn);
        
        // Create transaction record
        Transaction* trans = new Transaction(
            currentUser->getUserID(),
            isbn,
            "BORROW",
            currentUser->getFullName(),
            book->getTitle()
        );
        transactionList->append(trans);
        
        cout << "Success: Book borrowed successfully." << endl;
        cout << "Books borrowed: " << currentUser->getBorrowedCount() 
             << "/" << MAX_BORROW_LIMIT << endl;
        return true;
    }
    
    return false;
}

bool LibraryManager::returnBook(const string& isbn) {
    // Check if user is logged in
    if (authManager == nullptr || !authManager->isUser()) {
        cout << "Access Denied: Please login as user." << endl;
        return false;
    }
    
    User* currentUser = authManager->getCurrentUser();
    if (currentUser == nullptr) {
        cout << "Error: User session invalid." << endl;
        return false;
    }
    
    // Check if user has borrowed this book
    if (!currentUser->hasBorrowedBook(isbn)) {
        cout << "Error: You have not borrowed this book." << endl;
        return false;
    }
    
    // Find book
    Book* book = bookTree->search(isbn);
    if (book == nullptr) {
        cout << "Error: Book not found." << endl;
        return false;
    }
    
    // Perform return
    if (book->returnBook()) {
        currentUser->removeBorrowedBook(isbn);
        
        // Create transaction record
        Transaction* trans = new Transaction(
            currentUser->getUserID(),
            isbn,
            "RETURN",
            currentUser->getFullName(),
            book->getTitle()
        );
        transactionList->append(trans);
        
        cout << "Success: Book returned successfully." << endl;
        cout << "Books borrowed: " << currentUser->getBorrowedCount() 
             << "/" << MAX_BORROW_LIMIT << endl;
        return true;
    }
    
    return false;
}

vector<Book*> LibraryManager::getMyBorrowedBooks() {
    if (authManager == nullptr || !authManager->isUser()) {
        return vector<Book*>();
    }
    
    User* currentUser = authManager->getCurrentUser();
    if (currentUser == nullptr) {
        return vector<Book*>();
    }
    
    vector<Book*> borrowedBooks;
    set<string> borrowedISBNs = currentUser->getBorrowedISBNs();
    
    for (const string& isbn : borrowedISBNs) {
        Book* book = bookTree->search(isbn);
        if (book != nullptr) {
            borrowedBooks.push_back(book);
        }
    }
    
    return borrowedBooks;
}

vector<Transaction*> LibraryManager::getMyTransactions() {
    if (authManager == nullptr || !authManager->isUser()) {
        return vector<Transaction*>();
    }
    
    User* currentUser = authManager->getCurrentUser();
    if (currentUser == nullptr) {
        return vector<Transaction*>();
    }
    
    return transactionList->getByUserID(currentUser->getUserID());
}

// ============ DATA PERSISTENCE ============

bool LibraryManager::saveAllData() {
    bool success = true;
    
    success &= FileHandler::saveBooks(BOOKS_FILE, bookTree);
    success &= FileHandler::saveUsers(USERS_FILE, userMap);
    success &= FileHandler::saveTransactions(TRANSACTIONS_FILE, transactionList);
    
    if (success) {
        cout << "Success: All data saved successfully." << endl;
    } else {
        cout << "Warning: Some data may not have been saved." << endl;
    }
    
    return success;
}

bool LibraryManager::loadAllData() {
    bool success = true;
    
    success &= FileHandler::loadBooks(BOOKS_FILE, bookTree);
    success &= FileHandler::loadUsers(USERS_FILE, userMap);
    success &= FileHandler::loadTransactions(TRANSACTIONS_FILE, transactionList);
    
    // Rebuild search indices after loading books
    searchEngine->buildIndices();
    
    if (success) {
        cout << "Success: All data loaded successfully." << endl;
    }
    
    return success;
}

void LibraryManager::initializeSampleData() {
    cout << "Initializing sample data..." << endl;
    
    // Add sample books
    addBookInternal("978-0-13-468599-1", "The C++ Programming Language", 
                    "Bjarne Stroustrup", 5);
    addBookInternal("978-0-321-56384-2", "Effective C++", 
                    "Scott Meyers", 3);
    addBookInternal("978-0-262-03384-8", "Introduction to Algorithms", 
                    "Thomas Cormen", 4);
    addBookInternal("978-0-201-35088-5", "Data Structures and Algorithms", 
                    "Alfred Aho", 3);
    addBookInternal("978-0-672-32692-7", "Data Structures Using C++", 
                    "D.S. Malik", 4);
    addBookInternal("978-0-201-63361-0", "Design Patterns", 
                    "Gang of Four", 2);
    addBookInternal("978-0-596-00927-5", "Head First Design Patterns", 
                    "Eric Freeman", 3);
    addBookInternal("978-0-201-89683-1", "The Art of Computer Programming Vol 1", 
                    "Donald Knuth", 2);
    addBookInternal("978-0-13-110362-7", "The C Programming Language", 
                    "Brian Kernighan", 5);
    addBookInternal("978-0-134-68599-4", "Clean Code", 
                    "Robert Martin", 4);
    
    // Rebuild indices
    searchEngine->buildIndices();
    
    cout << "Sample data initialized: " << getTotalBooks() << " books added." << endl;
}

// ============ VALIDATION ============

bool LibraryManager::isISBNValid(const string& isbn) {
    // Basic ISBN validation (ISBN-10 or ISBN-13 format)
    if (isbn.length() < 10) return false;
    
    // Check for ISBN-13 format: 978-X-XXX-XXXXX-X
    if (isbn.find("978") == 0 || isbn.find("979") == 0) {
        return true;  // Simplified validation
    }
    
    return isbn.length() >= 10;  // Basic length check
}

bool LibraryManager::isBookAvailable(const string& isbn) {
    Book* book = bookTree->search(isbn);
    return (book != nullptr && book->isAvailable());
}