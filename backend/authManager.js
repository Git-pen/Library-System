// backend/authManager.js
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');
const { loadUsers, saveUsers, generateUserID } = require('./dataManager');

// Config matching C++ Config.h
const ADMIN_USERNAME = 'admin';
const ADMIN_PASSWORD = 'admin123';
const ADMIN_ID = 'ADMIN001';
const MIN_PASSWORD_LENGTH = 6;
const JWT_SECRET = 'library-management-secret-key-2024'; // In production, use environment variable

// Admin login
function loginAsAdmin(username, password) {
    if (username === ADMIN_USERNAME && password === ADMIN_PASSWORD) {
        const token = jwt.sign(
            { userID: ADMIN_ID, username: ADMIN_USERNAME, role: 'admin' },
            JWT_SECRET,
            { expiresIn: '24h' }
        );
        return {
            success: true,
            token,
            user: {
                userID: ADMIN_ID,
                username: ADMIN_USERNAME,
                fullName: 'Administrator',
                role: 'admin'
            }
        };
    }
    return { success: false, message: 'Invalid admin credentials' };
}

// User login
function loginAsUser(username, password) {
    const users = loadUsers();
    const user = users.find(u => u.username === username);

    if (!user) {
        return { success: false, message: 'User not found' };
    }

    if (!user.isActive) {
        return { success: false, message: 'Account is inactive' };
    }

    // Compare password with hash
    const isValidPassword = bcrypt.compareSync(password, user.passwordHash);

    if (!isValidPassword) {
        return { success: false, message: 'Invalid password' };
    }

    const token = jwt.sign(
        { userID: user.userID, username: user.username, role: 'user' },
        JWT_SECRET,
        { expiresIn: '24h' }
    );

    return {
        success: true,
        token,
        user: {
            userID: user.userID,
            username: user.username,
            fullName: user.fullName,
            email: user.email,
            phone: user.phone,
            borrowedCount: user.borrowedISBNs.length,
            role: 'user'
        }
    };
}

// Register new user
function registerUser(username, password, fullName, email, phone) {
    const users = loadUsers();

    // Check if username exists
    if (users.find(u => u.username === username)) {
        return { success: false, message: 'Username already exists' };
    }

    // Validate password length
    if (password.length < MIN_PASSWORD_LENGTH) {
        return { success: false, message: `Password must be at least ${MIN_PASSWORD_LENGTH} characters` };
    }

    // Validate email format
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    if (!emailRegex.test(email)) {
        return { success: false, message: 'Invalid email format' };
    }

    // Hash password
    const passwordHash = bcrypt.hashSync(password, 10);

    // Create new user
    const newUser = {
        userID: generateUserID(),
        username,
        passwordHash,
        fullName,
        email,
        phone,
        borrowedISBNs: [],
        isActive: true
    };

    users.push(newUser);
    saveUsers(users);

    return {
        success: true,
        message: 'Registration successful',
        userID: newUser.userID
    };
}

// Verify JWT token
function verifyToken(token) {
    try {
        const decoded = jwt.verify(token, JWT_SECRET);
        return { success: true, data: decoded };
    } catch (error) {
        return { success: false, message: 'Invalid or expired token' };
    }
}

// Check if username is available
function isUsernameAvailable(username) {
    const users = loadUsers();
    return !users.find(u => u.username === username);
}

module.exports = {
    loginAsAdmin,
    loginAsUser,
    registerUser,
    verifyToken,
    isUsernameAvailable,
    JWT_SECRET
};
