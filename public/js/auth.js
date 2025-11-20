// public/js/auth.js

// Check if user is logged in
function isLoggedIn() {
    return localStorage.getItem('token') !== null;
}

// Get current user from localStorage
function getCurrentUser() {
    const userStr = localStorage.getItem('user');
    return userStr ? JSON.parse(userStr) : null;
}

// Check if current user is admin
function isAdmin() {
    const user = getCurrentUser();
    return user && user.role === 'admin';
}

// Check if current user is regular user
function isRegularUser() {
    const user = getCurrentUser();
    return user && user.role === 'user';
}

// Logout
function logout() {
    localStorage.removeItem('token');
    localStorage.removeItem('user');
    window.location.href = '/index.html';
}

// Require authentication - redirect to login if not logged in
function requireAuth() {
    if (!isLoggedIn()) {
        window.location.href = '/index.html';
        return false;
    }
    return true;
}

// Require admin - redirect if not admin
function requireAdmin() {
    if (!requireAuth()) return false;

    if (!isAdmin()) {
        showToast('Access denied. Admin privileges required.', 'error');
        window.location.href = '/index.html';
        return false;
    }
    return true;
}

// Require regular user - redirect if not regular user
function requireUser() {
    if (!requireAuth()) return false;

    if (!isRegularUser()) {
        showToast('Access denied. User privileges required.', 'error');
        window.location.href = '/index.html';
        return false;
    }
    return true;
}

// Initialize auth on page load
function initAuth() {
    // If already logged in, redirect to appropriate dashboard
    if (isLoggedIn()) {
        const user = getCurrentUser();
        if (user.role === 'admin') {
            window.location.href = '/admin-dashboard.html';
        } else {
            window.location.href = '/user-dashboard.html';
        }
    }
}
