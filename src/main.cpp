// main.cpp
#include "entities/Book.h"
#include "entities/User.h"
#include "entities/Transaction.h"
#include "management/LibraryManager.h"
#include "management/AuthManager.h"
#include "Config.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>


#ifdef _WIN32
    #include <conio.h>
    #define CLEAR_SCREEN "cls"
#else
    #include <termios.h>
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"
#endif

using namespace std;

// ============ UTILITY FUNCTIONS ============

void clearScreen() {
    system(CLEAR_SCREEN);
}

void printLine(char ch = '=', int length = 70) {
    cout << string(length, ch) << endl;
}

void printDoubleLine() {
    printLine('=', 70);
}

void printSingleLine() {
    printLine('-', 70);
}

void printHeader(const string& title) {
    clearScreen();
    printDoubleLine();
    cout << setw(45) << right << title << endl;
    printDoubleLine();
    cout << endl;
}

void printSubHeader(const string& subtitle) {
    cout << endl;
    printSingleLine();
    cout << "  " << subtitle << endl;
    printSingleLine();
}

void pressEnterToContinue() {
    cout << endl;
    printSingleLine();
    cout << "Press ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string getInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int getIntInput(const string& prompt) {
    string input;
    int value;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        try {
            value = stoi(input);
            return value;
        } catch (...) {
            cout << "❌ Invalid input. Please enter a number." << endl;
        }
    }
}

string getPasswordInput(const string& prompt) {
    string password;
    cout << prompt;
    
    #ifdef _WIN32
        char ch;
        while ((ch = _getch()) != '\r') {  // '\r' is Enter key
            if (ch == '\b') {  // Backspace
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            } else {
                password += ch;
                cout << '*';
            }
        }
        cout << endl;
    #else
        // Unix/Linux - disable echo
        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        getline(cin, password);
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        cout << endl;
    #endif
    
    return password;
}

bool confirmAction(const string& message) {
    cout << endl << "⚠️  " << message << " (y/n): ";
    string response;
    getline(cin, response);
    return (response == "y" || response == "Y" || response == "yes" || response == "YES");
}

void printSuccess(const string& message) {
    cout << endl << "✅ " << message << endl;
}

void printError(const string& message) {
    cout << endl << "❌ " << message << endl;
}

void printInfo(const string& message) {
    cout << endl << "ℹ️  " << message << endl;
}

void displayBookDetails(Book* book) {
    if (book == nullptr) return;
    
    cout << "  📚 " << book->getTitle() << endl;
    cout << "      Author: " << book->getAuthor() << endl;
    cout << "      ISBN: " << book->getISBN() << endl;
    cout << "      Available: " << book->getAvailableCopies() 
         << "/" << book->getQuantity() << endl;
    cout << "      Status: " << (book->isAvailable() ? "✓ Available" : "✗ Not Available") << endl;
}

void displayBookTable(const vector<Book*>& books) {
    if (books.empty()) {
        printInfo("No books found.");
        return;
    }
    
    cout << endl;
    cout << setw(5) << left << "No."
         << setw(20) << left << "ISBN"
         << setw(30) << left << "Title"
         << setw(25) << left << "Author"
         << setw(12) << left << "Available" << endl;
    printSingleLine();
    
    int count = 1;
    for (Book* book : books) {
        cout << setw(5) << left << count++
             << setw(20) << left << book->getISBN().substr(0, 17) + "..."
             << setw(30) << left << (book->getTitle().length() > 27 ? 
                                     book->getTitle().substr(0, 27) + "..." : book->getTitle())
             << setw(25) << left << (book->getAuthor().length() > 22 ? 
                                     book->getAuthor().substr(0, 22) + "..." : book->getAuthor())
             << setw(12) << left << (to_string(book->getAvailableCopies()) + "/" + 
                                     to_string(book->getQuantity())) << endl;
    }
    
    cout << endl << "Total: " << books.size() << " book(s)" << endl;
}

void displayUserTable(const vector<User*>& users) {
    if (users.empty()) {
        printInfo("No users found.");
        return;
    }
    
    cout << endl;
    cout << setw(8) << left << "User ID"
         << setw(18) << left << "Username"
         << setw(25) << left << "Full Name"
         << setw(12) << left << "Borrowed"
         << setw(10) << left << "Status" << endl;
    printSingleLine();
    
    for (User* user : users) {
        cout << setw(8) << left << user->getUserID()
             << setw(18) << left << (user->getUsername().length() > 15 ? 
                                     user->getUsername().substr(0, 15) + "..." : user->getUsername())
             << setw(25) << left << (user->getFullName().length() > 22 ? 
                                     user->getFullName().substr(0, 22) + "..." : user->getFullName())
             << setw(12) << left << (to_string(user->getBorrowedCount()) + "/" + 
                                     to_string(MAX_BORROW_LIMIT))
             << setw(10) << left << (user->isActive() ? "Active" : "Inactive") << endl;
    }
    
    cout << endl << "Total: " << users.size() << " user(s)" << endl;
}

void displayTransactionTable(const vector<Transaction*>& transactions) {
    if (transactions.empty()) {
        printInfo("No transactions found.");
        return;
    }
    
    cout << endl;
    cout << setw(10) << left << "Trans ID"
         << setw(10) << left << "Type"
         << setw(20) << left << "User"
         << setw(25) << left << "Book"
         << setw(20) << left << "Date/Time" << endl;
    printSingleLine();
    
    for (Transaction* trans : transactions) {
        cout << setw(10) << left << trans->getTransactionID()
             << setw(10) << left << trans->getType()
             << setw(20) << left << (trans->getUserName().length() > 17 ? 
                                     trans->getUserName().substr(0, 17) + "..." : trans->getUserName())
             << setw(25) << left << (trans->getBookTitle().length() > 22 ? 
                                     trans->getBookTitle().substr(0, 22) + "..." : trans->getBookTitle())
             << setw(20) << left << trans->getTimestamp() << endl;
    }
    
    cout << endl << "Total: " << transactions.size() << " transaction(s)" << endl;
}

// ============ LOGIN & REGISTRATION ============

void displayLoginScreen() {
    printHeader("📖 LIBRARY MANAGEMENT SYSTEM 📖");
    cout << "  Welcome to the Digital Library" << endl;
    cout << endl;
    printSubHeader("LOGIN MENU");
    cout << "  1. Admin Login" << endl;
    cout << "  2. User Login" << endl;
    cout << "  3. Register New User" << endl;
    cout << "  4. Exit" << endl;
    printSingleLine();
}

bool handleAdminLogin(AuthManager* auth) {
    printHeader("🔐 ADMIN LOGIN");
    
    string username = getInput("  Username: ");
    string password = getPasswordInput("  Password: ");
    
    if (auth->loginAsAdmin(username, password)) {
        printSuccess("Admin login successful!");
        printInfo("Welcome, Administrator!");
        pressEnterToContinue();
        return true;
    } else {
        printError("Invalid admin credentials!");
        pressEnterToContinue();
        return false;
    }
}

bool handleUserLogin(AuthManager* auth) {
    printHeader("🔐 USER LOGIN");
    
    string username = getInput("  Username: ");
    string password = getPasswordInput("  Password: ");
    
    if (auth->loginAsUser(username, password)) {
        User* user = auth->getCurrentUser();
        printSuccess("Login successful!");
        printInfo("Welcome back, " + user->getFullName() + "!");
        pressEnterToContinue();
        return true;
    } else {
        printError("Invalid credentials or inactive account!");
        pressEnterToContinue();
        return false;
    }
}

void handleUserRegistration(AuthManager* auth) {
    printHeader("📝 USER REGISTRATION");
    
    string username = getInput("  Choose Username: ");
    
    if (!auth->isUsernameAvailable(username)) {
        printError("Username already taken!");
        pressEnterToContinue();
        return;
    }
    
    string password = getPasswordInput("  Choose Password (min 6 chars): ");
    
    if (!auth->validatePassword(password)) {
        printError("Password too short! Must be at least 6 characters.");
        pressEnterToContinue();
        return;
    }
    
    string confirmPassword = getPasswordInput("  Confirm Password: ");
    
    if (password != confirmPassword) {
        printError("Passwords do not match!");
        pressEnterToContinue();
        return;
    }
    
    string fullName = getInput("  Full Name: ");
    string email = getInput("  Email: ");
    
    if (!auth->validateEmail(email)) {
        printError("Invalid email format!");
        pressEnterToContinue();
        return;
    }
    
    string phone = getInput("  Phone Number: ");
    
    if (auth->registerUser(username, password, fullName, email, phone)) {
        printSuccess("Registration successful!");
        printInfo("You can now login with your credentials.");
        pressEnterToContinue();
    } else {
        printError("Registration failed!");
        pressEnterToContinue();
    }
}

// ============ ADMIN INTERFACE ============

void displayAdminMenu() {
    printHeader("👨‍💼 ADMINISTRATOR DASHBOARD");
    cout << "  1. 📚 Book Management" << endl;
    cout << "  2. 👥 User Management" << endl;
    cout << "  3. 📋 Transaction Management" << endl;
    cout << "  4. 📊 Reports & Statistics" << endl;
    cout << "  5. 🔍 Search Operations" << endl;
    cout << "  6. 💾 Save Data" << endl;
    cout << "  7. 🚪 Logout" << endl;
    printSingleLine();
}

void adminBookManagement(LibraryManager* library) {
    while (true) {
        printHeader("📚 BOOK MANAGEMENT");
        cout << "  1. Add New Book" << endl;
        cout << "  2. Remove Book" << endl;
        cout << "  3. Update Book Details" << endl;
        cout << "  4. Update Book Quantity" << endl;
        cout << "  5. View All Books" << endl;
        cout << "  6. View Available Books" << endl;
        cout << "  7. Back to Main Menu" << endl;
        printSingleLine();
        
        int choice = getIntInput("  Enter choice: ");
        
        switch (choice) {
            case 1: {
                printHeader("➕ ADD NEW BOOK");
                string isbn = getInput("  ISBN: ");
                string title = getInput("  Title: ");
                string author = getInput("  Author: ");
                int quantity = getIntInput("  Quantity: ");
                
                library->addBook(isbn, title, author, quantity);
                pressEnterToContinue();
                break;
            }
            
            case 2: {
                printHeader("➖ REMOVE BOOK");
                string isbn = getInput("  Enter ISBN: ");
                
                if (confirmAction("Are you sure you want to remove this book?")) {
                    library->removeBook(isbn);
                }
                pressEnterToContinue();
                break;
            }
            
            case 3: {
                printHeader("✏️ UPDATE BOOK DETAILS");
                string isbn = getInput("  Enter ISBN: ");
                Book* book = library->searchBookByISBN(isbn);
                
                if (book != nullptr) {
                    cout << endl << "Current Details:" << endl;
                    displayBookDetails(book);
                    cout << endl;
                    
                    string newTitle = getInput("  New Title (leave empty to keep current): ");
                    string newAuthor = getInput("  New Author (leave empty to keep current): ");
                    
                    if (newTitle.empty()) newTitle = book->getTitle();
                    if (newAuthor.empty()) newAuthor = book->getAuthor();
                    
                    library->updateBookDetails(isbn, newTitle, newAuthor);
                } else {
                    printError("Book not found!");
                }
                pressEnterToContinue();
                break;
            }
            
            case 4: {
                printHeader("🔢 UPDATE BOOK QUANTITY");
                string isbn = getInput("  Enter ISBN: ");
                Book* book = library->searchBookByISBN(isbn);
                
                if (book != nullptr) {
                    cout << endl << "Current Quantity: " << book->getQuantity() << endl;
                    int newQuantity = getIntInput("  New Quantity: ");
                    library->updateBookQuantity(isbn, newQuantity);
                } else {
                    printError("Book not found!");
                }
                pressEnterToContinue();
                break;
            }
            
            case 5: {
                printHeader("📚 ALL BOOKS");
                vector<Book*> books = library->getAllBooks();
                displayBookTable(books);
                pressEnterToContinue();
                break;
            }
            
            case 6: {
                printHeader("✅ AVAILABLE BOOKS");
                vector<Book*> books = library->getAvailableBooks();
                displayBookTable(books);
                pressEnterToContinue();
                break;
            }
            
            case 7:
                return;
            
            default:
                printError("Invalid choice!");
                pressEnterToContinue();
        }
    }
}

void adminUserManagement(LibraryManager* library) {
    while (true) {
        printHeader("👥 USER MANAGEMENT");
        cout << "  1. View All Users" << endl;
        cout << "  2. View User Details" << endl;
        cout << "  3. Deactivate User" << endl;
        cout << "  4. Activate User" << endl;
        cout << "  5. Remove User" << endl;
        cout << "  6. Back to Main Menu" << endl;
        printSingleLine();
        
        int choice = getIntInput("  Enter choice: ");
        
        switch (choice) {
            case 1: {
                printHeader("👥 ALL USERS");
                vector<User*> users = library->getAllUsers();
                displayUserTable(users);
                pressEnterToContinue();
                break;
            }
            
            case 2: {
                printHeader("👤 USER DETAILS");
                string userID = getInput("  Enter User ID: ");
                User* user = library->getUserDetails(userID);
                
                if (user != nullptr) {
                    cout << endl << user->toString() << endl;
                    
                    cout << endl << "Borrowed Books:" << endl;
                    set<string> borrowedISBNs = user->getBorrowedISBNs();
                    if (borrowedISBNs.empty()) {
                        cout << "  None" << endl;
                    } else {
                        for (const string& isbn : borrowedISBNs) {
                            Book* book = library->searchBookByISBN(isbn);
                            if (book != nullptr) {
                                cout << "  - " << book->getTitle() << " (" << isbn << ")" << endl;
                            }
                        }
                    }
                } else {
                    printError("User not found!");
                }
                pressEnterToContinue();
                break;
            }
            
            case 3: {
                printHeader("🚫 DEACTIVATE USER");
                string userID = getInput("  Enter User ID: ");
                library->deactivateUser(userID);
                pressEnterToContinue();
                break;
            }
            
            case 4: {
                printHeader("✅ ACTIVATE USER");
                string userID = getInput("  Enter User ID: ");
                library->activateUser(userID);
                pressEnterToContinue();
                break;
            }
            
            case 5: {
                printHeader("❌ REMOVE USER");
                string userID = getInput("  Enter User ID: ");
                
                if (confirmAction("Are you sure you want to remove this user?")) {
                    library->removeUser(userID);
                }
                pressEnterToContinue();
                break;
            }
            
            case 6:
                return;
            
            default:
                printError("Invalid choice!");
                pressEnterToContinue();
        }
    }
}

void adminTransactionManagement(LibraryManager* library) {
    while (true) {
        printHeader("📋 TRANSACTION MANAGEMENT");
        cout << "  1. View All Transactions" << endl;
        cout << "  2. View User Transactions" << endl;
        cout << "  3. View Book Transactions" << endl;
        cout << "  4. View Recent Transactions" << endl;
        cout << "  5. Back to Main Menu" << endl;
        printSingleLine();
        
        int choice = getIntInput("  Enter choice: ");
        
        switch (choice) {
            case 1: {
                printHeader("📋 ALL TRANSACTIONS");
                vector<Transaction*> transactions = library->getAllTransactions();
                displayTransactionTable(transactions);
                pressEnterToContinue();
                break;
            }
            
            case 2: {
                printHeader("👤 USER TRANSACTIONS");
                string userID = getInput("  Enter User ID: ");
                vector<Transaction*> transactions = library->getUserTransactions(userID);
                displayTransactionTable(transactions);
                pressEnterToContinue();
                break;
            }
            
            case 3: {
                printHeader("📚 BOOK TRANSACTIONS");
                string isbn = getInput("  Enter ISBN: ");
                vector<Transaction*> transactions = library->getBookTransactions(isbn);
                displayTransactionTable(transactions);
                pressEnterToContinue();
                break;
            }
            
            case 4: {
                printHeader("⏰ RECENT TRANSACTIONS");
                int count = getIntInput("  Number of transactions to show: ");
                vector<Transaction*> transactions = library->getRecentTransactions(count);
                displayTransactionTable(transactions);
                pressEnterToContinue();
                break;
            }
            
            case 5:
                return;
            
            default:
                printError("Invalid choice!");
                pressEnterToContinue();
        }
    }
}

void adminReportsStatistics(LibraryManager* library) {
    printHeader("📊 SYSTEM STATISTICS");
    
    cout << "  📚 Total Books: " << library->getTotalBooks() << endl;
    cout << "  ✅ Available Copies: " << library->getTotalAvailableBooks() << endl;
    cout << "  👥 Total Users: " << library->getTotalUsers() << endl;
    cout << "  ✓ Active Users: " << library->getActiveUsersCount() << endl;
    cout << "  📋 Total Transactions: " << library->getTotalTransactions() << endl;
    
    printSubHeader("Recent Activity");
    vector<Transaction*> recent = library->getRecentTransactions(5);
    displayTransactionTable(recent);
    
    pressEnterToContinue();
}

void adminSearchOperations(LibraryManager* library) {
    while (true) {
        printHeader("🔍 SEARCH OPERATIONS");
        cout << "  1. Search by Title" << endl;
        cout << "  2. Search by Author" << endl;
        cout << "  3. Search by ISBN" << endl;
        cout << "  4. Search by Keyword" << endl;
        cout << "  5. Back to Main Menu" << endl;
        printSingleLine();
        
        int choice = getIntInput("  Enter choice: ");
        
        switch (choice) {
            case 1: {
                printHeader("🔍 SEARCH BY TITLE");
                string title = getInput("  Enter title: ");
                vector<Book*> results = library->searchBooksByTitle(title);
                displayBookTable(results);
                pressEnterToContinue();
                break;
            }
            
            case 2: {
                printHeader("🔍 SEARCH BY AUTHOR");
                string author = getInput("  Enter author: ");
                vector<Book*> results = library->searchBooksByAuthor(author);
                displayBookTable(results);
                pressEnterToContinue();
                break;
            }
            
            case 3: {
                printHeader("🔍 SEARCH BY ISBN");
                string isbn = getInput("  Enter ISBN: ");
                Book* book = library->searchBookByISBN(isbn);
                
                if (book != nullptr) {
                    cout << endl;
                    displayBookDetails(book);
                } else {
                    printError("Book not found!");
                }
                pressEnterToContinue();
                break;
            }
            
            case 4: {
                printHeader("🔍 SEARCH BY KEYWORD");
                string keyword = getInput("  Enter keyword: ");
                vector<Book*> results = library->searchBooksByKeyword(keyword);
                displayBookTable(results);
                pressEnterToContinue();
                break;
            }
            
            case 5:
                return;
            
            default:
                printError("Invalid choice!");
                pressEnterToContinue();
        }
    }
}

void adminInterface(LibraryManager* library, AuthManager* auth) {
    while (true) {
        displayAdminMenu();
        int choice = getIntInput("  Enter choice: ");
        
        switch (choice) {
            case 1:
                adminBookManagement(library);
                break;
            
            case 2:
                adminUserManagement(library);
                break;
            
            case 3:
                adminTransactionManagement(library);
                break;
            
            case 4:
                adminReportsStatistics(library);
                break;
            
            case 5:
                adminSearchOperations(library);
                break;
            
            case 6:
                printHeader("💾 SAVING DATA");
                library->saveAllData();
                pressEnterToContinue();
                break;
            
            case 7:
                printInfo("Logging out...");
                auth->logout();
                return;
            
            default:
                printError("Invalid choice!");
                pressEnterToContinue();
        }
    }
}

// ============ USER INTERFACE ============

void displayUserMenu(User* currentUser) {
    printHeader("👤 USER DASHBOARD");
    cout << "  Welcome, " << currentUser->getFullName() << "!" << endl;
    cout << "  Books Borrowed: " << currentUser->getBorrowedCount() 
         << "/" << MAX_BORROW_LIMIT << endl;
    cout << endl;
    cout << "  1. 📚 Browse Books" << endl;
    cout << "  2. 🔍 Search Books" << endl;
    cout << "  3. 📖 My Books" << endl;
    cout << "  4. 👤 My Profile" << endl;
    cout << "  5. 💾 Save Data" << endl;
    cout << "  6. 🚪 Logout" << endl;
    printSingleLine();
}

void userBrowseBooks(LibraryManager* library) {
    printHeader("📚 BROWSE BOOKS");
    cout << "  1. View All Available Books" << endl;
    cout << "  2. View Book Details" << endl;
    cout << "  3. Back to Main Menu" << endl;
    printSingleLine();
    
    int choice = getIntInput("  Enter choice: ");
    
    switch (choice) {
        case 1: {
            printHeader("📚 AVAILABLE BOOKS");
            vector<Book*> books = library->getAvailableBooks();
            displayBookTable(books);
            pressEnterToContinue();
            break;
        }
        
        case 2: {
            printHeader("📖 BOOK DETAILS");
            string isbn = getInput("  Enter ISBN: ");
            Book* book = library->searchBookByISBN(isbn);
            
            if (book != nullptr) {
                cout << endl;
                displayBookDetails(book);
            } else {
                printError("Book not found!");
            }
            pressEnterToContinue();
            break;
        }
        
        case 3:
            return;
        
        default:
            printError("Invalid choice!");
            pressEnterToContinue();
    }
}

void userSearchBooks(LibraryManager* library) {
    printHeader("🔍 SEARCH BOOKS");
    cout << "  1. Search by Title" << endl;
    cout << "  2. Search by Author" << endl;
    cout << "  3. Search by ISBN" << endl;
    cout << "  4. Search by Keyword" << endl;
    cout << "  5. Back to Main Menu" << endl;
    printSingleLine();
    
    int choice = getIntInput("  Enter choice: ");
    
    switch (choice) {
        case 1: {
            printHeader("🔍 SEARCH BY TITLE");
            string title = getInput("  Enter title: ");
            vector<Book*> results = library->searchBooksByTitle(title);
            displayBookTable(results);
            pressEnterToContinue();
            break;
        }
        
        case 2: {
            printHeader("🔍 SEARCH BY AUTHOR");
            string author = getInput("  Enter author: ");
            vector<Book*> results = library->searchBooksByAuthor(author);
            displayBookTable(results);
            pressEnterToContinue();
            break;
        }
        
        case 3: {
            printHeader("🔍 SEARCH BY ISBN");
            string isbn = getInput("  Enter ISBN: ");
            Book* book = library->searchBookByISBN(isbn);
            
            if (book != nullptr) {
                cout << endl;
                displayBookDetails(book);
            } else {
                printError("Book not found!");
            }
            pressEnterToContinue();
            break;
        }
        
        case 4: {
            printHeader("🔍 SEARCH BY KEYWORD");
            string keyword = getInput("  Enter keyword: ");
            vector<Book*> results = library->searchBooksByKeyword(keyword);
            displayBookTable(results);
            pressEnterToContinue();
            break;
        }
        
        case 5:
            return;
        
        default:
            printError("Invalid choice!");
            pressEnterToContinue();
    }
}

void userMyBooks(LibraryManager* library) {
    while (true) {
        printHeader("📖 MY BOOKS");
        cout << "  1. Borrow a Book" << endl;
        cout << "  2. Return a Book" << endl;
        cout << "  3. View My Borrowed Books" << endl;
        cout << "  4. View My Transaction History" << endl;
        cout << "  5. Back to Main Menu" << endl;
        printSingleLine();
        
        int choice = getIntInput("  Enter choice: ");
        
        switch (choice) {
            case 1: {
                printHeader("📥 BORROW BOOK");
                string isbn = getInput("  Enter ISBN: ");
                library->borrowBook(isbn);
                pressEnterToContinue();
                break;
            }
            
            case 2: {
                printHeader("📤 RETURN BOOK");
                string isbn = getInput("  Enter ISBN: ");
                library->returnBook(isbn);
                pressEnterToContinue();
                break;
            }
            
            case 3: {
                printHeader("📚 MY BORROWED BOOKS");
                vector<Book*> books = library->getMyBorrowedBooks();
                
                if (books.empty()) {
                    printInfo("You have no borrowed books.");
                } else {
                    displayBookTable(books);
                }
                pressEnterToContinue();
                break;
            }
            
            case 4: {
                printHeader("📋 MY TRANSACTION HISTORY");
                vector<Transaction*> transactions = library->getMyTransactions();
                displayTransactionTable(transactions);
                pressEnterToContinue();
                break;
            }
            
            case 5:
                return;
            
            default:
                printError("Invalid choice!");
                pressEnterToContinue();
        }
    }
}

void userMyProfile(LibraryManager* library, AuthManager* auth) {
    User* currentUser = auth->getCurrentUser();
    if (currentUser == nullptr) return;
    
    printHeader("👤 MY PROFILE");
    cout << endl << currentUser->toString() << endl;
    
    printSubHeader("Options");
    cout << "  1. Update Contact Information" << endl;
    cout << "  2. Back to Main Menu" << endl;
    printSingleLine();
    
    int choice = getIntInput("  Enter choice: ");
    
    if (choice == 1) {
        string email = getInput("  New Email: ");
        string phone = getInput("  New Phone: ");
        
        if (auth->validateEmail(email)) {currentUser->updateContact(email, phone);
            printSuccess("Contact information updated successfully!");
        } else {
            printError("Invalid email format!");
        }
    }
    
    pressEnterToContinue();
}

void userInterface(LibraryManager* library, AuthManager* auth) {
    User* currentUser = auth->getCurrentUser();
    
    while (true) {
        displayUserMenu(currentUser);
        int choice = getIntInput("  Enter choice: ");
        
        switch (choice) {
            case 1:
                userBrowseBooks(library);
                break;
            
            case 2:
                userSearchBooks(library);
                break;
            
            case 3:
                userMyBooks(library);
                break;
            
            case 4:
                userMyProfile(library, auth);
                break;
            
            case 5:
                printHeader("💾 SAVING DATA");
                library->saveAllData();
                pressEnterToContinue();
                break;
            
            case 6:
                printInfo("Logging out...");
                auth->logout();
                return;
            
            default:
                printError("Invalid choice!");
                pressEnterToContinue();
        }
    }
}
// ============ MAIN FUNCTION ============

int main() {
    // Initialize managers
    LibraryManager* library = LibraryManager::getInstance();
    AuthManager* auth = AuthManager::getInstance();
    
    // Link auth manager to library
    library->setAuthManager(auth);
    auth->setUserMap(library->getUserMap());  // FIX: Properly link UserHashMap
    
    printHeader("📖 LIBRARY MANAGEMENT SYSTEM 📖");
    cout << "  Initializing system..." << endl;
    
    // Try to load existing data
    bool dataLoaded = library->loadAllData();
    
    if (!dataLoaded) {
        printInfo("No existing data found. Initializing with sample data...");
        library->initializeSampleData();
        
        // CRITICAL FIX: Save data immediately after initialization
        cout << endl << "  Saving initial data to files..." << endl;
        library->saveAllData();
        
        printSuccess("Sample data created and saved successfully!");
    } else {
        printSuccess("Existing data loaded successfully!");
    }
    
    printInfo("Total Books: " + to_string(library->getTotalBooks()));
    printInfo("Total Users: " + to_string(library->getTotalUsers()));
    
    pressEnterToContinue();
    
    // Main login loop
    while (true) {
        displayLoginScreen();
        int choice = getIntInput("  Enter choice: ");
        
        switch (choice) {
            case 1: // Admin Login
                if (handleAdminLogin(auth)) {
                    adminInterface(library, auth);
                    // Auto-save after admin session
                    cout << endl << "  Auto-saving data..." << endl;
                    library->saveAllData();
                }
                break;
            
            case 2: // User Login
                if (handleUserLogin(auth)) {
                    userInterface(library, auth);
                    // Auto-save after user session
                    cout << endl << "  Auto-saving data..." << endl;
                    library->saveAllData();
                }
                break;
            
            case 3: // Register
                handleUserRegistration(auth);
                // Save immediately after registration
                cout << endl << "  Saving registration data..." << endl;
                library->saveAllData();
                break;
            
            case 4: // Exit
                printHeader("👋 GOODBYE");
                cout << endl;
                cout << "  Saving all data..." << endl;
                library->saveAllData();
                printSuccess("Data saved successfully!");
                cout << endl;
                cout << "  Thank you for using Library Management System!" << endl;
                cout << "  Have a great day! 📚" << endl;
                cout << endl;
                printDoubleLine();
                return 0;
            
            default:
                printError("Invalid choice! Please try again.");
                pressEnterToContinue();
        }
    }
    
    return 0;
}