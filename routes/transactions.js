// routes/transactions.js
const express = require('express');
const router = express.Router();
const library = require('../backend/libraryManager');
const { authenticateToken, requireAdmin } = require('../middleware/auth');

// Get all transactions (admin only)
router.get('/', authenticateToken, requireAdmin, (req, res) => {
    const transactions = library.getAllTransactions();
    res.json({ success: true, transactions });
});

// Get user transactions (admin or own user)
router.get('/user/:userID', authenticateToken, (req, res) => {
    // Users can only view their own transactions
    if (req.user.role !== 'admin' && req.user.userID !== req.params.userID) {
        return res.status(403).json({ success: false, message: 'Access denied' });
    }

    const transactions = library.getUserTransactions(req.params.userID);
    res.json({ success: true, transactions });
});

// Get book transactions (admin only)
router.get('/book/:isbn', authenticateToken, requireAdmin, (req, res) => {
    const transactions = library.getBookTransactions(req.params.isbn);
    res.json({ success: true, transactions });
});

// Get recent transactions
router.get('/recent/:count?', authenticateToken, (req, res) => {
    const count = parseInt(req.params.count) || 10;
    const transactions = library.getRecentTransactions(count);
    res.json({ success: true, transactions });
});

// Get current user's transactions
router.get('/me', authenticateToken, (req, res) => {
    if (req.user.role !== 'user') {
        return res.status(403).json({ success: false, message: 'Only regular users have transactions' });
    }

    const transactions = library.getUserTransactions(req.user.userID);
    res.json({ success: true, transactions });
});

module.exports = router;
