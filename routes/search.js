// routes/search.js
const express = require('express');
const router = express.Router();
const library = require('../backend/libraryManager');

// Search by keyword
router.get('/', (req, res) => {
    const { q } = req.query;

    if (!q) {
        return res.status(400).json({ success: false, message: 'Search query required' });
    }

    const results = library.searchBooksByKeyword(q);
    res.json({ success: true, results });
});

// Search by title
router.get('/title', (req, res) => {
    const { q } = req.query;

    if (!q) {
        return res.status(400).json({ success: false, message: 'Search query required' });
    }

    const results = library.searchBooksByTitle(q);
    res.json({ success: true, results });
});

// Search by author
router.get('/author', (req, res) => {
    const { q } = req.query;

    if (!q) {
        return res.status(400).json({ success: false, message: 'Search query required' });
    }

    const results = library.searchBooksByAuthor(q);
    res.json({ success: true, results });
});

module.exports = router;
