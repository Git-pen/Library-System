// backend/dataManager.js
const fs = require('fs');
const path = require('path');

// File paths matching C++ Config.h
const DATA_DIR = path.join(__dirname, '../src/data');
const BOOKS_FILE = path.join(DATA_DIR, 'books.txt');
const USERS_FILE = path.join(DATA_DIR, 'users.txt');
const TRANSACTIONS_FILE = path.join(DATA_DIR, 'transactions.txt');

// Delimiters matching C++ Config.h
const CSV_DELIMITER = ',';
const LIST_DELIMITER = ';';

// Ensure data directory exists
function ensureDataDirectory() {
    if (!fs.existsSync(DATA_DIR)) {
        fs.mkdirSync(DATA_DIR, { recursive: true });
    }
}

// ============ BOOKS ============

function loadBooks() {
    ensureDataDirectory();
    if (!fs.existsSync(BOOKS_FILE)) {
        return [];
    }
    
    const content = fs.readFileSync(BOOKS_FILE, 'utf-8');
    const lines = content.trim().split('\n').filter(line => line.trim());
    
    return lines.map(line => {
        const parts = line.split(CSV_DELIMITER);
        if (parts.length >= 4) {
            return {
                isbn: parts[0].trim(),
                title: parts[1].trim(),
                author: parts[2].trim(),
                quantity: parseInt(parts[3]) || 0,
                availableCopies: parseInt(parts[4]) || parseInt(parts[3]) || 0
            };
        }
        return null;
    }).filter(book => book !== null);
}

function saveBooks(books) {
    ensureDataDirectory();
    const lines = books.map(book => 
        `${book.isbn}${CSV_DELIMITER}${book.title}${CSV_DELIMITER}${book.author}${CSV_DELIMITER}${book.quantity}${CSV_DELIMITER}${book.availableCopies}`
    );
    fs.writeFileSync(BOOKS_FILE, lines.join('\n') + '\n', 'utf-8');
}

// ============ USERS ============

function loadUsers() {
    ensureDataDirectory();
    if (!fs.existsSync(USERS_FILE)) {
        return [];
    }
    
    const content = fs.readFileSync(USERS_FILE, 'utf-8');
    const lines = content.trim().split('\n').filter(line => line.trim());
    
    return lines.map(line => {
        const parts = line.split(CSV_DELIMITER);
        if (parts.length >= 7) {
            return {
                userID: parts[0].trim(),
                username: parts[1].trim(),
                passwordHash: parts[2].trim(),
                fullName: parts[3].trim(),
                email: parts[4].trim(),
                phone: parts[5].trim(),
                borrowedISBNs: parts[6] ? parts[6].trim().split(LIST_DELIMITER).filter(isbn => isbn) : [],
                isActive: parts[7] ? parts[7].trim() === '1' : true
            };
        }
        return null;
    }).filter(user => user !== null);
}

function saveUsers(users) {
    ensureDataDirectory();
    const lines = users.map(user => 
        `${user.userID}${CSV_DELIMITER}${user.username}${CSV_DELIMITER}${user.passwordHash}${CSV_DELIMITER}${user.fullName}${CSV_DELIMITER}${user.email}${CSV_DELIMITER}${user.phone}${CSV_DELIMITER}${user.borrowedISBNs.join(LIST_DELIMITER)}${CSV_DELIMITER}${user.isActive ? '1' : '0'}`
    );
    fs.writeFileSync(USERS_FILE, lines.join('\n') + '\n', 'utf-8');
}

// ============ TRANSACTIONS ============

function loadTransactions() {
    ensureDataDirectory();
    if (!fs.existsSync(TRANSACTIONS_FILE)) {
        return [];
    }
    
    const content = fs.readFileSync(TRANSACTIONS_FILE, 'utf-8');
    const lines = content.trim().split('\n').filter(line => line.trim());
    
    return lines.map(line => {
        const parts = line.split(CSV_DELIMITER);
        if (parts.length >= 6) {
            return {
                transactionID: parts[0].trim(),
                type: parts[1].trim(),
                userID: parts[2].trim(),
                userName: parts[3].trim(),
                isbn: parts[4].trim(),
                bookTitle: parts[5].trim(),
                timestamp: parts[6] ? parts[6].trim() : new Date().toISOString()
            };
        }
        return null;
    }).filter(trans => trans !== null);
}

function saveTransactions(transactions) {
    ensureDataDirectory();
    const lines = transactions.map(trans => 
        `${trans.transactionID}${CSV_DELIMITER}${trans.type}${CSV_DELIMITER}${trans.userID}${CSV_DELIMITER}${trans.userName}${CSV_DELIMITER}${trans.isbn}${CSV_DELIMITER}${trans.bookTitle}${CSV_DELIMITER}${trans.timestamp}`
    );
    fs.writeFileSync(TRANSACTIONS_FILE, lines.join('\n') + '\n', 'utf-8');
}

// ============ UTILITY ============

function generateUserID() {
    const users = loadUsers();
    const maxID = users.reduce((max, user) => {
        const num = parseInt(user.userID.replace('USR', ''));
        return num > max ? num : max;
    }, 0);
    return `USR${String(maxID + 1).padStart(4, '0')}`;
}

function generateTransactionID() {
    const transactions = loadTransactions();
    const maxID = transactions.reduce((max, trans) => {
        const num = parseInt(trans.transactionID.replace('TXN', ''));
        return num > max ? num : max;
    }, 0);
    return `TXN${String(maxID + 1).padStart(6, '0')}`;
}

module.exports = {
    loadBooks,
    saveBooks,
    loadUsers,
    saveUsers,
    loadTransactions,
    saveTransactions,
    generateUserID,
    generateTransactionID
};
