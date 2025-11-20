// routes/books.js
const express = require('express');
const router = express.Router();
const library = require('../backend/libraryManager');
const { authenticateToken, requireAdmin } = require('../middleware/auth');

// Get all books (public or filtered)
router.get('/', (req, res) => {
    const { available } = req.query;

    if (available === 'true') {
        const books = library.getAvailableBooks();
        res.json({ success: true, books });
    } else {
        const books = library.getAllBooks();
        res.json({ success: true, books });
    }
});

// Get book by ISBN
router.get('/:isbn', (req, res) => {
    const book = library.getBookByISBN(req.params.isbn);

    if (book) {
        res.json({ success: true, book });
    } else {
        res.status(404).json({ success: false, message: 'Book not found' });
    }
});

// Add new book (admin only)
router.post('/', authenticateToken, requireAdmin, (req, res) => {
    const { isbn, title, author, quantity } = req.body;

    if (!isbn || !title || !author || !quantity) {
        return res.status(400).json({ success: false, message: 'All fields are required' });
    }

    const result = library.addBook(isbn, title, author, quantity);

    if (result.success) {
        res.json(result);
    } else {
        res.status(400).json(result);
    }
});

// Update book details (admin only)
router.put('/:isbn', authenticateToken, requireAdmin, (req, res) => {
    const { title, author } = req.body;

    if (!title || !author) {
        return res.status(400).json({ success: false, message: 'Title and author are required' });
    }

    const result = library.updateBookDetails(req.params.isbn, title, author);

    if (result.success) {
        res.json(result);
    } else {
        res.status(400).json(result);
    }
});

// Update book quantity (admin only)
router.put('/:isbn/quantity', authenticateToken, requireAdmin, (req, res) => {
    const { quantity } = req.body;

    if (quantity === undefined) {
        return res.status(400).json({ success: false, message: 'Quantity is required' });
    }

    const result = library.updateBookQuantity(req.params.isbn, quantity);

    if (result.success) {
        res.json(result);
    } else {
        res.status(400).json(result);
    }
});

// Delete book (admin only)
router.delete('/:isbn', authenticateToken, requireAdmin, (req, res) => {
    const result = library.removeBook(req.params.isbn);

    if (result.success) {
        res.json(result);
    } else {
        res.status(400).json(result);
    }
});

// Borrow book (user only)
router.post('/:isbn/borrow', authenticateToken, (req, res) => {
    if (req.user.role !== 'user') {
        return res.status(403).json({ success: false, message: 'Only regular users can borrow books' });
    }

    const result = library.borrowBook(req.params.isbn, req.user.userID);

    if (result.success) {
        res.json(result);
    } else {
        res.status(400).json(result);
    }
});

// Return book (user only)
router.post('/:isbn/return', authenticateToken, (req, res) => {
    if (req.user.role !== 'user') {
        return res.status(403).json({ success: false, message: 'Only regular users can return books' });
    }

    const result = library.returnBook(req.params.isbn, req.user.userID);

    if (result.success) {
        res.json(result);
    } else {
        res.status(400).json(result);
    }
});

module.exports = router;
