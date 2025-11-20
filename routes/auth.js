// routes/auth.js
const express = require('express');
const router = express.Router();
const { loginAsAdmin, loginAsUser, registerUser, isUsernameAvailable } = require('../backend/authManager');
const { getUserByID } = require('../backend/libraryManager');
const { authenticateToken } = require('../middleware/auth');

// Admin login
router.post('/login/admin', (req, res) => {
    const { username, password } = req.body;

    if (!username || !password) {
        return res.status(400).json({ success: false, message: 'Username and password required' });
    }

    const result = loginAsAdmin(username, password);

    if (result.success) {
        res.json(result);
    } else {
        res.status(401).json(result);
    }
});

// User login
router.post('/login/user', (req, res) => {
    const { username, password } = req.body;

    if (!username || !password) {
        return res.status(400).json({ success: false, message: 'Username and password required' });
    }

    const result = loginAsUser(username, password);

    if (result.success) {
        res.json(result);
    } else {
        res.status(401).json(result);
    }
});

// Register new user
router.post('/register', (req, res) => {
    const { username, password, fullName, email, phone } = req.body;

    if (!username || !password || !fullName || !email || !phone) {
        return res.status(400).json({ success: false, message: 'All fields are required' });
    }

    const result = registerUser(username, password, fullName, email, phone);

    if (result.success) {
        res.json(result);
    } else {
        res.status(400).json(result);
    }
});

// Check username availability
router.get('/check-username/:username', (req, res) => {
    const { username } = req.params;
    const available = isUsernameAvailable(username);
    res.json({ available });
});

// Get current user info
router.get('/me', authenticateToken, (req, res) => {
    if (req.user.role === 'admin') {
        res.json({
            success: true,
            user: {
                userID: req.user.userID,
                username: req.user.username,
                fullName: 'Administrator',
                role: 'admin'
            }
        });
    } else {
        const user = getUserByID(req.user.userID);
        if (user) {
            res.json({
                success: true,
                user: {
                    userID: user.userID,
                    username: user.username,
                    fullName: user.fullName,
                    email: user.email,
                    phone: user.phone,
                    borrowedCount: user.borrowedISBNs.length,
                    role: 'user'
                }
            });
        } else {
            res.status(404).json({ success: false, message: 'User not found' });
        }
    }
});

module.exports = router;
