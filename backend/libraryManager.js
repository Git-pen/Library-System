// backend/libraryManager.js
const { loadBooks, saveBooks, loadUsers, saveUsers, loadTransactions, saveTransactions, generateTransactionID } = require('./dataManager');

const MAX_BORROW_LIMIT = 5;

// ============ BOOK OPERATIONS ============

function getAllBooks() {
    return loadBooks();
}

function getAvailableBooks() {
    const books = loadBooks();
    return books.filter(book => book.availableCopies > 0);
}

function getBookByISBN(isbn) {
    const books = loadBooks();
    return books.find(book => book.isbn === isbn);
}

function addBook(isbn, title, author, quantity) {
    const books = loadBooks();

    // Check if ISBN already exists
    if (books.find(book => book.isbn === isbn)) {
        return { success: false, message: 'Book with this ISBN already exists' };
    }

    const newBook = {
        isbn,
        title,
        author,
        quantity: parseInt(quantity),
        availableCopies: parseInt(quantity)
    };

    books.push(newBook);
    saveBooks(books);

    return { success: true, message: 'Book added successfully', book: newBook };
}

function removeBook(isbn) {
    const books = loadBooks();
    const index = books.findIndex(book => book.isbn === isbn);

    if (index === -1) {
        return { success: false, message: 'Book not found' };
    }

    // Check if book is borrowed
    if (books[index].availableCopies < books[index].quantity) {
        return { success: false, message: 'Cannot remove book that is currently borrowed' };
    }

    books.splice(index, 1);
    saveBooks(books);

    return { success: true, message: 'Book removed successfully' };
}

function updateBookDetails(isbn, newTitle, newAuthor) {
    const books = loadBooks();
    const book = books.find(b => b.isbn === isbn);

    if (!book) {
        return { success: false, message: 'Book not found' };
    }

    book.title = newTitle;
    book.author = newAuthor;
    saveBooks(books);

    return { success: true, message: 'Book updated successfully', book };
}

function updateBookQuantity(isbn, newQuantity) {
    const books = loadBooks();
    const book = books.find(b => b.isbn === isbn);

    if (!book) {
        return { success: false, message: 'Book not found' };
    }

    const borrowed = book.quantity - book.availableCopies;

    if (newQuantity < borrowed) {
        return { success: false, message: `Cannot set quantity below borrowed count (${borrowed})` };
    }

    book.availableCopies = newQuantity - borrowed;
    book.quantity = newQuantity;
    saveBooks(books);

    return { success: true, message: 'Quantity updated successfully', book };
}

// ============ SEARCH OPERATIONS ============

function searchBooksByTitle(title) {
    const books = loadBooks();
    const searchTerm = title.toLowerCase();
    return books.filter(book => book.title.toLowerCase().includes(searchTerm));
}

function searchBooksByAuthor(author) {
    const books = loadBooks();
    const searchTerm = author.toLowerCase();
    return books.filter(book => book.author.toLowerCase().includes(searchTerm));
}

function searchBooksByKeyword(keyword) {
    const books = loadBooks();
    const searchTerm = keyword.toLowerCase();
    return books.filter(book =>
        book.title.toLowerCase().includes(searchTerm) ||
        book.author.toLowerCase().includes(searchTerm) ||
        book.isbn.toLowerCase().includes(searchTerm)
    );
}

// ============ USER OPERATIONS ============

function getAllUsers() {
    return loadUsers();
}

function getUserByID(userID) {
    const users = loadUsers();
    return users.find(user => user.userID === userID);
}

function removeUser(userID) {
    const users = loadUsers();
    const index = users.findIndex(user => user.userID === userID);

    if (index === -1) {
        return { success: false, message: 'User not found' };
    }

    // Check if user has borrowed books
    if (users[index].borrowedISBNs.length > 0) {
        return { success: false, message: 'Cannot remove user with borrowed books' };
    }

    users.splice(index, 1);
    saveUsers(users);

    return { success: true, message: 'User removed successfully' };
}

function deactivateUser(userID) {
    const users = loadUsers();
    const user = users.find(u => u.userID === userID);

    if (!user) {
        return { success: false, message: 'User not found' };
    }

    user.isActive = false;
    saveUsers(users);

    return { success: true, message: 'User deactivated successfully' };
}

function activateUser(userID) {
    const users = loadUsers();
    const user = users.find(u => u.userID === userID);

    if (!user) {
        return { success: false, message: 'User not found' };
    }

    user.isActive = true;
    saveUsers(users);

    return { success: true, message: 'User activated successfully' };
}

function getActiveUsersCount() {
    const users = loadUsers();
    return users.filter(user => user.isActive).length;
}

// ============ BORROW/RETURN OPERATIONS ============

function borrowBook(isbn, userID) {
    const books = loadBooks();
    const users = loadUsers();

    const book = books.find(b => b.isbn === isbn);
    const user = users.find(u => u.userID === userID);

    if (!book) {
        return { success: false, message: 'Book not found' };
    }

    if (!user) {
        return { success: false, message: 'User not found' };
    }

    if (!user.isActive) {
        return { success: false, message: 'User account is inactive' };
    }

    if (book.availableCopies <= 0) {
        return { success: false, message: 'Book is not available' };
    }

    if (user.borrowedISBNs.length >= MAX_BORROW_LIMIT) {
        return { success: false, message: `Maximum borrow limit (${MAX_BORROW_LIMIT}) reached` };
    }

    if (user.borrowedISBNs.includes(isbn)) {
        return { success: false, message: 'You have already borrowed this book' };
    }

    // Update book availability
    book.availableCopies--;
    saveBooks(books);

    // Update user borrowed books
    user.borrowedISBNs.push(isbn);
    saveUsers(users);

    // Create transaction
    const transaction = {
        transactionID: generateTransactionID(),
        type: 'BORROW',
        userID: user.userID,
        userName: user.fullName,
        isbn: book.isbn,
        bookTitle: book.title,
        timestamp: new Date().toISOString()
    };

    const transactions = loadTransactions();
    transactions.push(transaction);
    saveTransactions(transactions);

    return { success: true, message: 'Book borrowed successfully', transaction };
}

function returnBook(isbn, userID) {
    const books = loadBooks();
    const users = loadUsers();

    const book = books.find(b => b.isbn === isbn);
    const user = users.find(u => u.userID === userID);

    if (!book) {
        return { success: false, message: 'Book not found' };
    }

    if (!user) {
        return { success: false, message: 'User not found' };
    }

    if (!user.borrowedISBNs.includes(isbn)) {
        return { success: false, message: 'You have not borrowed this book' };
    }

    // Update book availability
    book.availableCopies++;
    saveBooks(books);

    // Update user borrowed books
    user.borrowedISBNs = user.borrowedISBNs.filter(i => i !== isbn);
    saveUsers(users);

    // Create transaction
    const transaction = {
        transactionID: generateTransactionID(),
        type: 'RETURN',
        userID: user.userID,
        userName: user.fullName,
        isbn: book.isbn,
        bookTitle: book.title,
        timestamp: new Date().toISOString()
    };

    const transactions = loadTransactions();
    transactions.push(transaction);
    saveTransactions(transactions);

    return { success: true, message: 'Book returned successfully', transaction };
}

function getUserBorrowedBooks(userID) {
    const users = loadUsers();
    const books = loadBooks();

    const user = users.find(u => u.userID === userID);

    if (!user) {
        return [];
    }

    return user.borrowedISBNs.map(isbn => books.find(b => b.isbn === isbn)).filter(b => b);
}

// ============ TRANSACTION OPERATIONS ============

function getAllTransactions() {
    return loadTransactions();
}

function getUserTransactions(userID) {
    const transactions = loadTransactions();
    return transactions.filter(trans => trans.userID === userID);
}

function getBookTransactions(isbn) {
    const transactions = loadTransactions();
    return transactions.filter(trans => trans.isbn === isbn);
}

function getRecentTransactions(count) {
    const transactions = loadTransactions();
    return transactions.slice(-count).reverse();
}

// ============ STATISTICS ============

function getStatistics() {
    const books = loadBooks();
    const users = loadUsers();
    const transactions = loadTransactions();

    return {
        totalBooks: books.length,
        totalCopies: books.reduce((sum, book) => sum + book.quantity, 0),
        availableCopies: books.reduce((sum, book) => sum + book.availableCopies, 0),
        borrowedCopies: books.reduce((sum, book) => sum + (book.quantity - book.availableCopies), 0),
        totalUsers: users.length,
        activeUsers: users.filter(u => u.isActive).length,
        totalTransactions: transactions.length
    };
}

module.exports = {
    getAllBooks,
    getAvailableBooks,
    getBookByISBN,
    addBook,
    removeBook,
    updateBookDetails,
    updateBookQuantity,
    searchBooksByTitle,
    searchBooksByAuthor,
    searchBooksByKeyword,
    getAllUsers,
    getUserByID,
    removeUser,
    deactivateUser,
    activateUser,
    getActiveUsersCount,
    borrowBook,
    returnBook,
    getUserBorrowedBooks,
    getAllTransactions,
    getUserTransactions,
    getBookTransactions,
    getRecentTransactions,
    getStatistics
};
