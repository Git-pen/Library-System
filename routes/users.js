// routes/users.js
const express = require('express');
const router = express.Router();
const library = require('../backend/libraryManager');
const { authenticateToken, requireAdmin } = require('../middleware/auth');

// Get all users (admin only)
router.get('/', authenticateToken, requireAdmin, (req, res) => {
    const users = library.getAllUsers();

    // Remove password hashes from response
    const sanitizedUsers = users.map(user => ({
        userID: user.userID,
        username: user.username,
        fullName: user.fullName,
        email: user.email,
        phone: user.phone,
        borrowedCount: user.borrowedISBNs.length,
        isActive: user.isActive
    }));

    res.json({ success: true, users: sanitizedUsers });
});

// Get user details (admin only)
router.get('/:userID', authenticateToken, requireAdmin, (req, res) => {
    const user = library.getUserByID(req.params.userID);

    if (!user) {
        return res.status(404).json({ success: false, message: 'User not found' });
    }

    const borrowedBooks = library.getUserBorrowedBooks(req.params.userID);

    res.json({
        success: true,
        user: {
            userID: user.userID,
            username: user.username,
            fullName: user.fullName,
            email: user.email,
            phone: user.phone,
            borrowedCount: user.borrowedISBNs.length,
            isActive: user.isActive,
            borrowedBooks
        }
    });
});

// Activate user (admin only)
router.put('/:userID/activate', authenticateToken, requireAdmin, (req, res) => {
    const result = library.activateUser(req.params.userID);

    if (result.success) {
        res.json(result);
    } else {
        res.status(400).json(result);
    }
});

// Deactivate user (admin only)
router.put('/:userID/deactivate', authenticateToken, requireAdmin, (req, res) => {
    const result = library.deactivateUser(req.params.userID);

    if (result.success) {
        res.json(result);
    } else {
        res.status(400).json(result);
    }
});

// Delete user (admin only)
router.delete('/:userID', authenticateToken, requireAdmin, (req, res) => {
    const result = library.removeUser(req.params.userID);

    if (result.success) {
        res.json(result);
    } else {
        res.status(400).json(result);
    }
});

// Get current user's borrowed books
router.get('/me/books', authenticateToken, (req, res) => {
    if (req.user.role !== 'user') {
        return res.status(403).json({ success: false, message: 'Only regular users have borrowed books' });
    }

    const books = library.getUserBorrowedBooks(req.user.userID);
    res.json({ success: true, books });
});

module.exports = router;
