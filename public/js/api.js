// public/js/api.js

// Dynamic API URL - works with both localhost and ngrok
const API_BASE_URL = window.location.origin + '/api';

// Get token from localStorage
function getToken() {
    return localStorage.getItem('token');
}

// Set token in localStorage
function setToken(token) {
    localStorage.setItem('token');
}

// Remove token from localStorage
function removeToken() {
    localStorage.removeItem('token');
    localStorage.removeItem('user');
}

// API request helper
async function apiRequest(endpoint, options = {}) {
    const token = getToken();

    const headers = {
        'Content-Type': 'application/json',
        ...options.headers
    };

    if (token) {
        headers['Authorization'] = `Bearer ${token}`;
    }

    try {
        const response = await fetch(`${API_BASE_URL}${endpoint}`, {
            ...options,
            headers
        });

        const data = await response.json();

        if (!response.ok) {
            throw new Error(data.message || 'Request failed');
        }

        return data;
    } catch (error) {
        console.error('API Error:', error);
        throw error;
    }
}

// ============ AUTH API ============

const AuthAPI = {
    loginAdmin: async (username, password) => {
        const data = await apiRequest('/auth/login/admin', {
            method: 'POST',
            body: JSON.stringify({ username, password })
        });
        if (data.success) {
            localStorage.setItem('token', data.token);
            localStorage.setItem('user', JSON.stringify(data.user));
        }
        return data;
    },

    loginUser: async (username, password) => {
        const data = await apiRequest('/auth/login/user', {
            method: 'POST',
            body: JSON.stringify({ username, password })
        });
        if (data.success) {
            localStorage.setItem('token', data.token);
            localStorage.setItem('user', JSON.stringify(data.user));
        }
        return data;
    },

    register: async (username, password, fullName, email, phone) => {
        return await apiRequest('/auth/register', {
            method: 'POST',
            body: JSON.stringify({ username, password, fullName, email, phone })
        });
    },

    checkUsername: async (username) => {
        return await apiRequest(`/auth/check-username/${username}`);
    },

    getCurrentUser: async () => {
        return await apiRequest('/auth/me');
    }
};

// ============ BOOKS API ============

const BooksAPI = {
    getAll: async (availableOnly = false) => {
        const query = availableOnly ? '?available=true' : '';
        return await apiRequest(`/books${query}`);
    },

    getByISBN: async (isbn) => {
        return await apiRequest(`/books/${isbn}`);
    },

    add: async (isbn, title, author, quantity) => {
        return await apiRequest('/books', {
            method: 'POST',
            body: JSON.stringify({ isbn, title, author, quantity })
        });
    },

    update: async (isbn, title, author) => {
        return await apiRequest(`/books/${isbn}`, {
            method: 'PUT',
            body: JSON.stringify({ title, author })
        });
    },

    updateQuantity: async (isbn, quantity) => {
        return await apiRequest(`/books/${isbn}/quantity`, {
            method: 'PUT',
            body: JSON.stringify({ quantity })
        });
    },

    delete: async (isbn) => {
        return await apiRequest(`/books/${isbn}`, {
            method: 'DELETE'
        });
    },

    borrow: async (isbn) => {
        return await apiRequest(`/books/${isbn}/borrow`, {
            method: 'POST'
        });
    },

    return: async (isbn) => {
        return await apiRequest(`/books/${isbn}/return`, {
            method: 'POST'
        });
    }
};

// ============ USERS API ============

const UsersAPI = {
    getAll: async () => {
        return await apiRequest('/users');
    },

    getByID: async (userID) => {
        return await apiRequest(`/users/${userID}`);
    },

    activate: async (userID) => {
        return await apiRequest(`/users/${userID}/activate`, {
            method: 'PUT'
        });
    },

    deactivate: async (userID) => {
        return await apiRequest(`/users/${userID}/deactivate`, {
            method: 'PUT'
        });
    },

    delete: async (userID) => {
        return await apiRequest(`/users/${userID}`, {
            method: 'DELETE'
        });
    },

    getMyBooks: async () => {
        return await apiRequest('/users/me/books');
    }
};

// ============ TRANSACTIONS API ============

const TransactionsAPI = {
    getAll: async () => {
        return await apiRequest('/transactions');
    },

    getByUser: async (userID) => {
        return await apiRequest(`/transactions/user/${userID}`);
    },

    getByBook: async (isbn) => {
        return await apiRequest(`/transactions/book/${isbn}`);
    },

    getRecent: async (count = 10) => {
        return await apiRequest(`/transactions/recent/${count}`);
    },

    getMy: async () => {
        return await apiRequest('/transactions/me');
    }
};

// ============ SEARCH API ============

const SearchAPI = {
    keyword: async (query) => {
        return await apiRequest(`/search?q=${encodeURIComponent(query)}`);
    },

    title: async (query) => {
        return await apiRequest(`/search/title?q=${encodeURIComponent(query)}`);
    },

    author: async (query) => {
        return await apiRequest(`/search/author?q=${encodeURIComponent(query)}`);
    }
};

// ============ STATISTICS API ============

const StatisticsAPI = {
    get: async () => {
        return await apiRequest('/statistics');
    }
};
