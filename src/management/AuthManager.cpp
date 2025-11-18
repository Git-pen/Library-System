// management/AuthManager.cpp
#include "AuthManager.h"
#include <algorithm>

// Initialize static instance
AuthManager* AuthManager::instance = nullptr;

// ============ SINGLETON ============

AuthManager::AuthManager() : userMap(nullptr), currentUser(nullptr), currentRole(NONE) {}

AuthManager* AuthManager::getInstance() {
    if (instance == nullptr) {
        instance = new AuthManager();
    }
    return instance;
}

AuthManager::~AuthManager() {
    // Don't delete userMap (owned by LibraryManager)
    // Don't delete currentUser (owned by userMap)
}

void AuthManager::setUserMap(UserHashMap* map) {
    userMap = map;
}

// ============ AUTHENTICATION ============

bool AuthManager::loginAsAdmin(const string& username, const string& password) {
    if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
        currentRole = ADMIN;
        currentUser = nullptr;  // Admin doesn't have a User object
        return true;
    }
    return false;
}

bool AuthManager::loginAsUser(const string& username, const string& password) {
    if (userMap == nullptr) return false;
    
    User* user = userMap->searchByUsername(username);
    if (user == nullptr) {
        return false;  // User not found
    }
    
    if (!user->isActive()) {
        return false;  // Account inactive
    }
    
    if (!user->authenticate(password)) {
        return false;  // Wrong password
    }
    
    // Login successful
    currentUser = user;
    currentRole = USER;
    return true;
}

bool AuthManager::registerUser(const string& username, const string& password, 
                               const string& fullName, const string& email, 
                               const string& phone) {
    if (userMap == nullptr) return false;
    
    // Validate inputs
    if (!isUsernameAvailable(username)) {
        return false;  // Username taken
    }
    
    if (!validatePassword(password)) {
        return false;  // Password too weak
    }
    
    if (!validateEmail(email)) {
        return false;  // Invalid email
    }
    
    // Create new user
    User* newUser = new User(username, password, fullName, email, phone);
    userMap->insert(newUser);
    
    return true;
}

void AuthManager::logout() {
    currentUser = nullptr;
    currentRole = NONE;
}

// ============ AUTHORIZATION CHECKS ============

bool AuthManager::isAdmin() const {
    return currentRole == ADMIN;
}

bool AuthManager::isUser() const {
    return currentRole == USER;
}

bool AuthManager::isLoggedIn() const {
    return currentRole != NONE;
}

User* AuthManager::getCurrentUser() const {
    return currentUser;
}

string AuthManager::getCurrentUserID() const {
    if (currentRole == ADMIN) {
        return ADMIN_ID;
    } else if (currentRole == USER && currentUser != nullptr) {
        return currentUser->getUserID();
    }
    return "";
}

AuthManager::Role AuthManager::getCurrentRole() const {
    return currentRole;
}

// ============ VALIDATION ============

bool AuthManager::isUsernameAvailable(const string& username) const {
    if (userMap == nullptr) return false;
    
    // Check against admin username
    if (username == ADMIN_USERNAME) {
        return false;
    }
    
    return !userMap->existsUsername(username);
}

bool AuthManager::validatePassword(const string& password) const {
    return password.length() >= MIN_PASSWORD_LENGTH;
}

bool AuthManager::validateEmail(const string& email) const {
    // Simple email validation
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    
    return (atPos != string::npos && 
            dotPos != string::npos && 
            atPos > 0 && 
            dotPos > atPos + 1 && 
            dotPos < email.length() - 1);
}