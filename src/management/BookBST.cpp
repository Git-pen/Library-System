// management/BookBST.cpp
#include "BookBST.h"

// ============ CONSTRUCTOR & DESTRUCTOR ============

BookBST::BookBST() : root(nullptr), nodeCount(0) {}

BookBST::~BookBST() {
    clear();
}

void BookBST::clear() {
    destroy(root);
    root = nullptr;
    nodeCount = 0;
}

void BookBST::destroy(BookNode* node) {
    if (node != nullptr) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

// ============ UTILITY METHODS ============

int BookBST::getCount() const {
    return nodeCount;
}

bool BookBST::isEmpty() const {
    return root == nullptr;
}

// ============ HEIGHT & BALANCE ============

int BookBST::getHeight(BookNode* node) {
    return (node == nullptr) ? 0 : node->height;
}

int BookBST::getBalance(BookNode* node) {
    return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
}

// ============ AVL ROTATIONS ============

BookBST::BookNode* BookBST::rotateRight(BookNode* y) {
    BookNode* x = y->left;
    BookNode* T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    
    return x;
}

BookBST::BookNode* BookBST::rotateLeft(BookNode* x) {
    BookNode* y = x->right;
    BookNode* T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    
    return y;
}

// ============ INSERT ============

void BookBST::insert(const Book& book) {
    root = insert(root, book);
}

BookBST::BookNode* BookBST::insert(BookNode* node, const Book& book) {
    // Standard BST insertion
    if (node == nullptr) {
        nodeCount++;
        return new BookNode(book);
    }
    
    if (book.getISBN() < node->data.getISBN()) {
        node->left = insert(node->left, book);
    } else if (book.getISBN() > node->data.getISBN()) {
        node->right = insert(node->right, book);
    } else {
        // Duplicate ISBN - update existing book
        node->data = book;
        return node;
    }
    
    // Update height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    
    // Get balance factor
    int balance = getBalance(node);
    
    // Left-Left Case
    if (balance > 1 && book.getISBN() < node->left->data.getISBN()) {
        return rotateRight(node);
    }
    
    // Right-Right Case
    if (balance < -1 && book.getISBN() > node->right->data.getISBN()) {
        return rotateLeft(node);
    }
    
    // Left-Right Case
    if (balance > 1 && book.getISBN() > node->left->data.getISBN()) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Right-Left Case
    if (balance < -1 && book.getISBN() < node->right->data.getISBN()) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

// ============ SEARCH ============

Book* BookBST::search(const string& isbn) {
    BookNode* node = search(root, isbn);
    return (node != nullptr) ? &(node->data) : nullptr;
}

BookBST::BookNode* BookBST::search(BookNode* node, const string& isbn) {
    if (node == nullptr || node->data.getISBN() == isbn) {
        return node;
    }
    
    if (isbn < node->data.getISBN()) {
        return search(node->left, isbn);
    } else {
        return search(node->right, isbn);
    }
}

// ============ DELETE ============

bool BookBST::remove(const string& isbn) {
    if (search(isbn) == nullptr) {
        return false;  // Book not found
    }
    root = deleteNode(root, isbn);
    nodeCount--;
    return true;
}

BookBST::BookNode* BookBST::deleteNode(BookNode* node, const string& isbn) {
    if (node == nullptr) {
        return node;
    }
    
    // Standard BST deletion
    if (isbn < node->data.getISBN()) {
        node->left = deleteNode(node->left, isbn);
    } else if (isbn > node->data.getISBN()) {
        node->right = deleteNode(node->right, isbn);
    } else {
        // Node to be deleted found
        
        // Case 1: No child or one child
        if (node->left == nullptr) {
            BookNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            BookNode* temp = node->left;
            delete node;
            return temp;
        }
        
        // Case 2: Two children
        BookNode* temp = findMin(node->right);
        node->data = temp->data;
        node->right = deleteNode(node->right, temp->data.getISBN());
    }
    
    if (node == nullptr) {
        return node;
    }
    
    // Update height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    
    // Get balance factor
    int balance = getBalance(node);
    
    // Left-Left Case
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }
    
    // Left-Right Case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Right-Right Case
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }
    
    // Right-Left Case
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

BookBST::BookNode* BookBST::findMin(BookNode* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// ============ TRAVERSAL ============

vector<Book*> BookBST::getAllBooksSorted() {
    vector<Book*> result;
    inorderTraversal(root, result);
    return result;
}

void BookBST::inorderTraversal(BookNode* node, vector<Book*>& result) {
    if (node != nullptr) {
        inorderTraversal(node->left, result);
        result.push_back(&(node->data));  // Return pointer to book in node
        inorderTraversal(node->right, result);
    }
}