// server.js
const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3004;

// Middleware
app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Serve static files from public directory
app.use(express.static(path.join(__dirname, 'public')));

// API Routes
app.use('/api/auth', require('./routes/auth'));
app.use('/api/books', require('./routes/books'));
app.use('/api/users', require('./routes/users'));
app.use('/api/transactions', require('./routes/transactions'));
app.use('/api/search', require('./routes/search'));
app.use('/api/statistics', require('./routes/statistics'));

// Root route - serve index.html
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Health check
app.get('/api/health', (req, res) => {
    res.json({ status: 'ok', message: 'Library Management System API is running' });
});

// 404 handler
app.use((req, res) => {
    res.status(404).json({ success: false, message: 'Endpoint not found' });
});

// Error handler
app.use((err, req, res, next) => {
    console.error(err.stack);
    res.status(500).json({ success: false, message: 'Internal server error' });
});

// Start server
app.listen(PORT, () => {
    console.log(`
╔════════════════════════════════════════════════════════════╗
║                                                            ║
║     📚 LIBRARY MANAGEMENT SYSTEM - WEB SERVER 📚          ║
║                                                            ║
║     Server running on: http://localhost:${PORT}            ║
║                                                            ║
║     API Endpoints:                                         ║
║     • Authentication: /api/auth                            ║
║     • Books:          /api/books                           ║
║     • Users:          /api/users                           ║
║     • Transactions:   /api/transactions                    ║
║     • Search:         /api/search                          ║
║     • Statistics:     /api/statistics                      ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝
    `);
});
