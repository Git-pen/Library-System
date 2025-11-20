// middleware/auth.js
const { verifyToken } = require('../backend/authManager');

// Verify JWT token middleware
function authenticateToken(req, res, next) {
    const authHeader = req.headers['authorization'];
    const token = authHeader && authHeader.split(' ')[1]; // Bearer TOKEN

    if (!token) {
        return res.status(401).json({ success: false, message: 'Access token required' });
    }

    const result = verifyToken(token);

    if (!result.success) {
        return res.status(403).json({ success: false, message: result.message });
    }

    req.user = result.data;
    next();
}

// Check if user is admin
function requireAdmin(req, res, next) {
    if (req.user.role !== 'admin') {
        return res.status(403).json({ success: false, message: 'Admin access required' });
    }
    next();
}

// Check if user is regular user (not admin)
function requireUser(req, res, next) {
    if (req.user.role !== 'user') {
        return res.status(403).json({ success: false, message: 'User access required' });
    }
    next();
}

module.exports = {
    authenticateToken,
    requireAdmin,
    requireUser
};
