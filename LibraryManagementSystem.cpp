#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

struct Book {
    string id;
    string title;
    string author;
    bool isBorrowed;

    void display() const {
        cout << left << setw(10) << id
             << setw(30) << title
             << setw(20) << author
             << setw(10) << (isBorrowed ? "Yes" : "No") << endl;
    }
};

vector<Book> books;

// Utility Functions
void loadBooksFromFile() {
    books.clear();
    ifstream file("books.txt");
    if (!file) return;
    Book b;
    string borrowStatus;
    while (getline(file, b.id, '*') &&
           getline(file, b.title, '*') &&
           getline(file, b.author, '*') &&
           getline(file, borrowStatus)) {
        b.isBorrowed = (borrowStatus == "1");
        books.push_back(b);
    }
    file.close();
}

void saveBooksToFile() {
    ofstream file("books.txt");
    for (const auto& b : books) {
        file << b.id << "*"
             << b.title << "*"
             << b.author << "*"
             << (b.isBorrowed ? "1" : "0") << endl;
    }
    file.close();
}

Book* findBookByID(const string& id) {
    for (auto& book : books) {
        if (book.id == id) return &book;
    }
    return nullptr;
}

void showAllBooks() {
    cout << "\n=== Book List ===\n";
    cout << left << setw(10) << "ID" << setw(30) << "Title"
         << setw(20) << "Author" << setw(10) << "Borrowed" << endl;
    cout << string(70, '-') << endl;
    for (const auto& book : books)
        book.display();
}

// Core Functionalities
void addBook() {
    Book b;
    cout << "Enter Book ID: ";
    cin >> ws;
    getline(cin, b.id);
    if (findBookByID(b.id)) {
        cout << "Book with this ID already exists.\n";
        return;
    }
    cout << "Enter Book Title: ";
    getline(cin, b.title);
    cout << "Enter Author Name: ";
    getline(cin, b.author);
    b.isBorrowed = false;
    books.push_back(b);
    saveBooksToFile();
    cout << "Book added successfully!\n";
}

void updateBook() {
    string id;
    cout << "Enter Book ID to update: ";
    cin >> ws;
    getline(cin, id);
    Book* b = findBookByID(id);
    if (!b) {
        cout << "Book not found.\n";
        return;
    }
    cout << "Enter new Title: ";
    getline(cin, b->title);
    cout << "Enter new Author: ";
    getline(cin, b->author);
    saveBooksToFile();
    cout << "Book updated!\n";
}

void deleteBook() {
    string id;
    cout << "Enter Book ID to delete: ";
    cin >> ws;
    getline(cin, id);
    auto it = remove_if(books.begin(), books.end(), [&](Book& b) { return b.id == id; });
    if (it != books.end()) {
        books.erase(it, books.end());
        saveBooksToFile();
        cout << "Book deleted.\n";
    } else {
        cout << "Book not found.\n";
    }
}

void searchBook() {
    string keyword;
    cout << "Enter keyword (ID/Title/Author): ";
    cin >> ws;
    getline(cin, keyword);
    cout << "\n=== Search Results ===\n";
    bool found = false;
    for (const auto& book : books) {
        if (book.id.find(keyword) != string::npos ||
            book.title.find(keyword) != string::npos ||
            book.author.find(keyword) != string::npos) {
            book.display();
            found = true;
        }
    }
    if (!found) cout << "No books matched your search.\n";
}

void borrowBook() {
    string id;
    cout << "Enter Book ID to borrow: ";
    cin >> ws;
    getline(cin, id);
    Book* b = findBookByID(id);
    if (!b) {
        cout << "Book not found.\n";
        return;
    }
    if (b->isBorrowed) {
        cout << "Book is already borrowed.\n";
    } else {
        b->isBorrowed = true;
        saveBooksToFile();
        cout << "Book borrowed successfully!\n";
    }
}

void returnBook() {
    string id;
    cout << "Enter Book ID to return: ";
    cin >> ws;
    getline(cin, id);
    Book* b = findBookByID(id);
    if (!b) {
        cout << "Book not found.\n";
        return;
    }
    if (!b->isBorrowed) {
        cout << "Book is not marked as borrowed.\n";
    } else {
        b->isBorrowed = false;
        saveBooksToFile();
        cout << "Book returned successfully!\n";
    }
}

void statsDashboard() {
    int total = books.size();
    int borrowed = count_if(books.begin(), books.end(), [](Book& b){ return b.isBorrowed; });
    int available = total - borrowed;
    cout << "\n=== Statistics Dashboard ===\n";
    cout << "Total Books   : " << total << endl;
    cout << "Borrowed Books: " << borrowed << endl;
    cout << "Available     : " << available << endl;
}

bool adminLogin() {
    string user, pass;
    cout << "\n--- Admin Login ---\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;
    return (user == "admin" && pass == "admin123");
}

void adminMenu() {
    int choice;
    do {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Book\n2. Update Book\n3. Delete Book\n4. View Books\n5. Search\n6. Stats\n7. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addBook(); break;
            case 2: updateBook(); break;
            case 3: deleteBook(); break;
            case 4: showAllBooks(); break;
            case 5: searchBook(); break;
            case 6: statsDashboard(); break;
            case 7: cout << "Logging out...\n"; break;
            default: cout << "Invalid option.\n";
        }
    } while (choice != 7);
}

void guestMenu() {
    int choice;
    do {
        cout << "\n--- Guest Menu ---\n";
        cout << "1. View Books\n2. Search\n3. Borrow Book\n4. Return Book\n5. Stats\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice) {
            case 1: showAllBooks(); break;
            case 2: searchBook(); break;
            case 3: borrowBook(); break;
            case 4: returnBook(); break;
            case 5: statsDashboard(); break;
            case 6: cout << "Goodbye!\n"; break;
            default: cout << "Invalid option.\n";
        }
    } while (choice != 6);
}

int main() {
    loadBooksFromFile();
    int mode;
    cout << "=== Library Management System ===\n";
    cout << "1. Admin Login\n2. Guest Access\nChoose mode: ";
    cin >> mode;
    if (mode == 1) {
        if (adminLogin()) adminMenu();
        else cout << "Wrong credentials!\n";
    } else if (mode == 2) {
        guestMenu();
    } else {
        cout << "Invalid selection.\n";
    }
    return 0;
}
