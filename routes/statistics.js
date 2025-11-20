// routes/statistics.js
const express = require('express');
const router = express.Router();
const library = require('../backend/libraryManager');
const { authenticateToken, requireAdmin } = require('../middleware/auth');

// Get system statistics (admin only)
router.get('/', authenticateToken, requireAdmin, (req, res) => {
    const stats = library.getStatistics();
    res.json({ success: true, statistics: stats });
});

module.exports = router;
