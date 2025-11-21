# Library Management System - Web Application

A modern web-based Library Management System with Node.js backend and vanilla HTML/CSS/JavaScript frontend, integrated with the existing C++ console application through shared data files.

## Features

### User Features
- 📝 **User Registration** - Create new accounts with validation
- 🔐 **User Login** - Secure authentication with JWT tokens
- 📚 **Browse Books** - View all available books in the library
- 🔍 **Search Books** - Search by title, author, or keyword
- 📖 **Borrow Books** - Borrow available books (max 5 per user)
- 📥 **Return Books** - Return borrowed books
- 📊 **My Books** - View currently borrowed books and transaction history
- 📈 **Dashboard** - Personal dashboard with stats and recent activity

### Admin Features
- 👨‍💼 **Admin Login** - Separate admin authentication
- 📚 **Book Management** - Add, edit, update quantity, and delete books
- 👥 **User Management** - View, activate, deactivate, and remove users
- 📋 **Transaction Management** - View all transactions with filtering
- 📊 **Statistics Dashboard** - System overview with key metrics
- 🔍 **Advanced Search** - Search and filter across all entities

## Technology Stack

### Backend
- **Node.js** - Runtime environment
- **Express.js** - Web framework
- **JWT** - Authentication tokens
- **bcryptjs** - Password hashing
- **CORS** - Cross-origin resource sharing

### Frontend
- **HTML5** - Structure
- **CSS3** - Styling with modern design system
- **Vanilla JavaScript** - No frameworks, pure JS
- **Google Fonts (Inter)** - Typography

### Data Storage
- **File-based storage** - Shared with C++ application
- **CSV format** - Compatible with C++ backend
- **Data files**: `books.txt`, `users.txt`, `transactions.txt`

## Installation

### Prerequisites
- Node.js (v14 or higher)
- npm (comes with Node.js)

### Setup Steps

1. **Navigate to project directory**
   ```powershell
   cd e:\your-folder\Library-System
   ```

2. **Install dependencies**
   ```powershell
   npm install
   ```

3. **Start the server**
   ```powershell
   npm start
   ```

4. **Access the application**
   - Open your browser and go to: `http://localhost:3003
     `

## Usage

### First Time Setup

1. **Admin Login**
   - Username: `admin`
   - Password: `admin123`

2. **User Registration**
   - Click "Register New Account" on the homepage
   - Fill in all required fields
   - Login with your new credentials

### User Workflow

1. **Login** as a regular user
2. **Browse Books** to see available books
3. **Search Books** to find specific titles
4. **Borrow Books** by clicking the "Borrow" button
5. **View My Books** to see borrowed books
6. **Return Books** when finished

### Admin Workflow

1. **Login** as admin
2. **Manage Books** - Add new books, update details, adjust quantities
3. **Manage Users** - View user details, activate/deactivate accounts
4. **View Transactions** - Monitor all library activity
5. **Check Statistics** - View system overview

## Project Structure

```
Library-System/
├── backend/
│   ├── authManager.js       # Authentication logic
│   ├── dataManager.js       # File I/O operations
│   └── libraryManager.js    # Business logic
├── middleware/
│   └── auth.js              # JWT verification
├── routes/
│   ├── auth.js              # Authentication routes
│   ├── books.js             # Book management routes
│   ├── users.js             # User management routes
│   ├── transactions.js      # Transaction routes
│   ├── search.js            # Search routes
│   └── statistics.js        # Statistics routes
├── public/
│   ├── css/
│   │   ├── style.css        # Main styles
│   │   └── components.css   # Component styles
│   ├── js/
│   │   ├── api.js           # API client
│   │   ├── auth.js          # Auth utilities
│   │   └── utils.js         # Helper functions
│   ├── index.html           # Landing page
│   ├── login.html           # Login page
│   ├── register.html        # Registration page
│   ├── user-dashboard.html  # User dashboard
│   ├── browse-books.html    # Browse books
│   ├── search-books.html    # Search books
│   ├── my-books.html        # My borrowed books
│   ├── admin-dashboard.html # Admin dashboard
│   ├── admin-books.html     # Book management
│   ├── admin-users.html     # User management
│   └── admin-transactions.html # Transaction history
├── src/                     # C++ source files
│   ├── data/                # Shared data files
│   │   ├── books.txt
│   │   ├── users.txt
│   │   └── transactions.txt
│   └── ...
├── server.js                # Express server
├── package.json             # Dependencies
└── README_WEB.md            # This file
```

## API Endpoints

### Authentication
- `POST /api/auth/login/admin` - Admin login
- `POST /api/auth/login/user` - User login
- `POST /api/auth/register` - Register new user
- `GET /api/auth/me` - Get current user info

### Books
- `GET /api/books` - Get all books
- `GET /api/books/:isbn` - Get book by ISBN
- `POST /api/books` - Add new book (admin)
- `PUT /api/books/:isbn` - Update book (admin)
- `DELETE /api/books/:isbn` - Delete book (admin)
- `POST /api/books/:isbn/borrow` - Borrow book (user)
- `POST /api/books/:isbn/return` - Return book (user)

### Users
- `GET /api/users` - Get all users (admin)
- `GET /api/users/:id` - Get user details (admin)
- `PUT /api/users/:id/activate` - Activate user (admin)
- `PUT /api/users/:id/deactivate` - Deactivate user (admin)
- `DELETE /api/users/:id` - Delete user (admin)
- `GET /api/users/me/books` - Get my borrowed books

### Transactions
- `GET /api/transactions` - Get all transactions (admin)
- `GET /api/transactions/user/:userId` - Get user transactions
- `GET /api/transactions/book/:isbn` - Get book transactions
- `GET /api/transactions/recent/:count` - Get recent transactions
- `GET /api/transactions/me` - Get my transactions

### Search
- `GET /api/search?q=keyword` - Search by keyword
- `GET /api/search/title?q=title` - Search by title
- `GET /api/search/author?q=author` - Search by author

### Statistics
- `GET /api/statistics` - Get system statistics (admin)

## Data Format

The application uses CSV files compatible with the C++ application:

### books.txt
```
ISBN,Title,Author,Quantity,AvailableCopies
```

### users.txt
```
UserID,Username,PasswordHash,FullName,Email,Phone,BorrowedISBNs,IsActive
```

### transactions.txt
```
TransactionID,Type,UserID,UserName,ISBN,BookTitle,Timestamp
```

## Configuration

### Default Admin Credentials
- Username: `admin`
- Password: `admin123`

### Limits
- Maximum books per user: 5
- Minimum password length: 6 characters

### Server Port
- Default: 3000
- Change in `server.js` or set `PORT` environment variable

## Development

### Running in Development Mode
```powershell
npm run dev
```
This uses nodemon for auto-restart on file changes.

### Running the C++ Console App
The C++ console application can still be used:
```powershell
cd src
.\lms_app.exe
```

Both applications share the same data files, so changes in one are reflected in the other (may require server restart for web app to see C++ changes).

## Troubleshooting

### Port Already in Use
If port 3000 is already in use, either:
1. Stop the other application using port 3000
2. Change the port in `server.js`

### Cannot Find Module
Run `npm install` to install all dependencies.

### Data Not Loading
Ensure the `src/data/` directory exists and has the correct permissions.

### CORS Errors
The server is configured with CORS enabled. If you still see errors, check your browser console.

## Security Notes

- Passwords are hashed using bcrypt
- JWT tokens expire after 24 hours
- Admin credentials should be changed in production
- In production, use environment variables for sensitive data

## Future Enhancements

- Email notifications for due dates
- Book reservations
- Fine calculation for late returns
- Book categories and tags
- Advanced reporting
- Export data to PDF/Excel
- Book cover images
- User profile pictures

## License

This project is part of an academic assignment.

## Support

For issues or questions, please contact the development team.

---

**Enjoy your modern Library Management System! 📚**
