// Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
using namespace std;

// ============ USER LIMITS ============
const int MAX_BORROW_LIMIT = 5;
const int MIN_PASSWORD_LENGTH = 6;

// ============ ADMIN CREDENTIALS ============
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "admin123";
const string ADMIN_ID = "ADMIN001";

// ============ FILE PATHS ============
const string DATA_DIR = "data/";
const string BOOKS_FILE = DATA_DIR + "books.txt";
const string USERS_FILE = DATA_DIR + "users.txt";
const string TRANSACTIONS_FILE = DATA_DIR + "transactions.txt";

// ============ HASH TABLE CONFIGURATION ============
const int INITIAL_HASH_TABLE_SIZE = 101;  // Prime number
const double MAX_LOAD_FACTOR = 0.75;

// ============ DELIMITERS ============
const char CSV_DELIMITER = ',';
const char LIST_DELIMITER = ';';

// ============ DISPLAY SETTINGS ============
const int BOOKS_PER_PAGE = 10;
const int RECENT_TRANSACTIONS_COUNT = 20;

#endif // CONFIG_H