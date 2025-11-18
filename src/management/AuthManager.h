// management/AuthManager.h
#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include "../entities/User.h"
#include "../Config.h"
#include "UserHashMap.h"
#include <string>
using namespace std;

class AuthManager {
public:
    enum Role {
        NONE,
        ADMIN,
        USER
    };

private:
    static AuthManager* instance;
    UserHashMap* userMap;
    User* currentUser;
    Role currentRole;
    
    // Private constructor (Singleton)
    AuthManager();

public:
    static AuthManager* getInstance();
    ~AuthManager();
    
    void setUserMap(UserHashMap* map);
    
    // Authentication
    bool loginAsAdmin(const string& username, const string& password);
    bool loginAsUser(const string& username, const string& password);
    bool registerUser(const string& username, const string& password, 
                     const string& fullName, const string& email, const string& phone);
    void logout();
    
    // Authorization checks
    bool isAdmin() const;
    bool isUser() const;
    bool isLoggedIn() const;
    User* getCurrentUser() const;
    string getCurrentUserID() const;
    Role getCurrentRole() const;
    
    // Validation
    bool isUsernameAvailable(const string& username) const;
    bool validatePassword(const string& password) const;
    bool validateEmail(const string& email) const;
};

#endif // AUTHMANAGER_H