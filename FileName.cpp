#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <iomanip> 
#include <Windows.h>
using namespace std;



class Member {
public:
    int id;
    string name;
    string email;
    Member(int id, string name, string email) : id(id), name(name), email(email) {}
    virtual string get_details() = 0; 

    virtual ~Member() = default;
};

class Professor : public Member {
public:
    string department;
    Professor(int id, string name, string email, string department) : Member(id, name, email), department(department) {}
    string get_details() override {
        return "ID: " + to_string(id) + " | Name: " + name + " | Email: " + email + " | Department: " + department;
    }
};

class Employee : public Member {
public:
    string position;
    Employee(int id, string name, string email, string position) : Member(id, name, email), position(position) {}
    string get_details() override {
        return "ID: " + to_string(id) + " | Name: " + name + " | Email: " + email + " | Position: " + position;
    }
};

class Student : public Member {
public:
    string major;
    Student(int id, string name, string email, string major) : Member(id, name, email), major(major) {}
    string get_details() override {
        return "ID: " + to_string(id) + " | Name: " + name + " | Email: " + email + " | Major: " + major;
    }
};

class Admin : public Member {
public:
    Admin(int id, string name, string email) : Member(id, name, email) {}
    string get_details() override {
        return "ID: " + to_string(id) + " | Name: " + name + " | Email: " + email + " | Role: Admin";
    }
};

class Book {
public:
    string isbn;
    string title;
    string author;
    string publisher;
    int year;
    int borrowed_by;
    time_t return_date;
    Book(string isbn, string title, string author, string publisher, int year) : isbn(isbn), title(title), author(author), publisher(publisher), year(year), borrowed_by(0), return_date(0) {}
};

Member* search_member_by_id(const vector<Member*>& members, int id) {
    for (auto member : members) {
        if (member->id == id) {
            return member;
        }
    }
    return nullptr;
}

vector<Member*> search_member_by_name(const vector<Member*>& members, const string& name) {
    vector<Member*> result;
    for (auto member : members) {
        if (member->name.find(name) != string::npos) {
            result.push_back(member);
        }
    }
    return result;
}

Book* search_book_by_isbn(const vector<Book*>& books, const string& isbn) {
    for (auto book : books) {
        if (book->isbn == isbn) {
            return book;
        }
    }
    return nullptr;
}

vector<Book*> search_book_by_title(const vector<Book*>& books, const string& title) {
    vector<Book*> result;
    for (auto book : books) {
        if (book->title.find(title) != string::npos) {
            result.push_back(book);
        }
    }
    return result;
}

class Library {
private:
    vector<Member*> members;
    vector<Book*> books;

public:
    ~Library() {
        for (auto member : members) {
            delete member;
        }
        for (auto book : books) {
            delete book;
        }
    }

    void add_member(Member* member) {
        members.push_back(member);
    }

    void edit_member(int id, string name, string email) {
        for (auto member : members) {
            if (member->id == id) {
                member->name = name;
                member->email = email;
            }
        }
    }
    Member* search_member_by_id(int id) {
        return ::search_member_by_id(members, id);
    }

    vector<Member*> search_member_by_name(const string& name) {
        return ::search_member_by_name(members, name);
    }

    
    Book* search_book_by_isbn(const string& isbn) {
        return ::search_book_by_isbn(books, isbn);
    }

    vector<Book*> search_book_by_title(const string& title) {
        return ::search_book_by_title(books, title);
    }

    void delete_member(int id) {
        members.erase(remove_if(members.begin(), members.end(), [id](Member* member) { return member->id == id; }), members.end());
    }

    void display_members() {
        for (auto member : members) {
            cout << member->get_details() << endl;
        }
    }


    void add_book(Book* book) {
        books.push_back(book);
    }

    void edit_book(string isbn, string title, string author, string publisher, int year) {
        for (auto book : books) {
            if (book->isbn == isbn) {
                book->title = title;
                book->author = author;
                book->publisher = publisher;
                book->year = year;
            }
        }
    }

    void delete_book(string isbn) {
        books.erase(remove_if(books.begin(), books.end(), [isbn](Book* book) { return book->isbn == isbn; }), books.end());
    }

    void display_books() {
        for (auto book : books) {
            cout << "ISBN: " << book->isbn << " | Title: " << book->title << " | Author: " << book->author << " | Publisher: " << book->publisher << " | Year: " << book->year << endl;
        }
    }

    void borrow_book(string isbn, int member_id) {
        for (auto book : books) {
            if (book->isbn == isbn && book->borrowed_by == 0) {
                book->borrowed_by = member_id;
                book->return_date = time(0) + 604800; 
                return; 
            }
        }
    }



    void return_book(string isbn, int member_id) {
        for (auto book : books) {
            if (book->isbn == isbn && book->borrowed_by == member_id) {
                book->borrowed_by = 0;
                book->return_date = 0;
            }
        }
    }

    void display_borrowed_books(int member_id) {
        for (auto book : books) {
            if (book->borrowed_by == member_id) {
                cout << "ISBN: " << book->isbn << " | Title: " << book->title << " | Author: " << book->author << " | Return Date: " << ctime(&book->return_date) << endl;
            }
        }
    }

    void check_overdue_books(time_t current_time) {
        for (auto book : books) {
            if (book->borrowed_by != 0 && book->return_date < current_time) {
                cout << "ISBN: " << book->isbn << " | Title: " << book->title << " | Author: " << book->author << " | Borrowed By: " << book->borrowed_by << " | Return Date: " << ctime(&book->return_date) << endl;
            }
        }
    }

    void save_data(const string& member_filename, const string& book_filename) {
        ofstream member_file(member_filename, ios::out);
        if (!member_file) {
            cerr << "Error opening " << member_filename << " for writing.\n";
            return;
        }

        for (auto member : members) {
            if (Professor* professor = dynamic_cast<Professor*>(member)) {
                member_file << "Professor," << professor->id << "," << professor->name << "," << professor->email << "," << professor->department << endl;
            }
            else if (Employee* employee = dynamic_cast<Employee*>(member)) {
                member_file << "Employee," << employee->id << "," << employee->name << "," << employee->email << "," << employee->position << endl;
            }
            else if (Student* student = dynamic_cast<Student*>(member)) {
                member_file << "Student," << student->id << "," << student->name << "," << student->email << "," << student->major << endl;
            }
            else if (Admin* admin = dynamic_cast<Admin*>(member)) {
                member_file << "Admin," << admin->id << "," << admin->name << "," << admin->email << endl;
            }
        }
        member_file.close();

        ofstream book_file(book_filename, ios::out);
        if (!book_file) {
            cerr << "Error opening " << book_filename << " for writing.\n";
            return;
        }

        for (auto book : books) {
            book_file << book->isbn << "," << book->title << "," << book->author << "," << book->publisher << "," << book->year << "," << book->borrowed_by << "," << book->return_date << endl;
        }
        book_file.close();
    }



    void load_data(const string& member_filename, const string& book_filename) {
        ifstream member_file(member_filename, ios::in);
        if (!member_file) {
            cerr << "Error opening " << member_filename << " for reading.\n";
            return;
        }

        string line, type, name, email, department, position, major;
        int id;
        while (getline(member_file, line)) {
            stringstream ss(line);
            getline(ss, type, ',');
            ss >> id;
            ss.ignore();
            getline(ss, name, ',');
            getline(ss, email, ',');

            if (type == "Professor") {
                getline(ss, department, ',');
                add_member(new Professor(id, name, email, department));
            }
            else if (type == "Employee") {
                getline(ss, position, ',');
                add_member(new Employee(id, name, email, position));
            }
            else if (type == "Student") {
                getline(ss, major, ',');
                add_member(new Student(id, name, email, major));
            }
            else if (type == "Admin") {
                add_member(new Admin(id, name, email));
            }
        }
        member_file.close();

        ifstream book_file(book_filename, ios::in);
        if (!book_file) {
            cerr << "Error opening " << book_filename << " for reading.\n";
            return;
        }

        string isbn, title, author, publisher, borrowed_by_str, return_date_str;
        int year, borrowed_by;
        time_t return_date;
        while (getline(book_file, line)) {
            stringstream ss(line);
            getline(ss, isbn, ',');
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, publisher, ',');
            ss >> year;
            ss.ignore();
            ss >> borrowed_by;
            ss.ignore();
            ss >> return_date;
            Book* book = new Book(isbn, title, author, publisher, year);
            book->borrowed_by = borrowed_by;
            book->return_date = return_date;
            add_book(book);
        }
        book_file.close();
    }




};

void clearScreen() {
    system("cls");
}

void display_admin_info(Member* admin) {
    cout << "Welcome, Admin!\n";
    cout << "Your information:\n";
    cout << admin->get_details() << endl;
    cout << "----------------------------------\n";
}

void mainMenu();
void memberMenu(Library& library);
void bookMenu(Library& library);
void addMemberMenu(Library& library);
void editMemberMenu(Library& library);
void deleteMemberMenu(Library& library);
void displayMembersMenu(Library& library);
void addBookMenu(Library& library);
void editBookMenu(Library& library);
void deleteBookMenu(Library& library);
void displayBooksMenu(Library& library);
void borrowBookMenu(Library& library);
void returnBookMenu(Library& library);
void displayBorrowedBooksMenu(Library& library);
void checkOverdueBooksMenu(Library& library);

void mainMenu();
void memberMenu(Library& library);
void bookMenu(Library& library);
void admin_login(Library& library);
void admin_menu(Library& library, Member* admin);

void mainMenu() {
    Library library;
    library.load_data("members.txt", "books.txt");

    int choice;
    do {
        clearScreen();
        cout << "Main Menu:\n";
        cout << "1. Member Management\n";
        cout << "2. Book Management\n";
        cout << "3. Admin Menu\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            memberMenu(library);
            break;
        case 2:
            bookMenu(library);
            break;
        case 3:
            admin_login(library); 
            break;
        case 0:
            library.save_data("members.txt", "books.txt");
            break;
        default:
            cout << "Invalid choice! Try again.\n";
            break;
        }
    } while (choice != 0);
}

void memberMenu(Library& library) {
    int choice;
    do {
        clearScreen();
        Sleep(500);
        cout << "Member Management Menu:\n";
        cout << "1. Add Member\n";
        cout << "2. Edit Member\n";
        cout << "3. Delete Member\n";
        cout << "4. Display Members\n";
        cout << "5. Search Member by ID\n";
        cout << "6. Search Member by Name\n";
        cout << "0. Back to Main Menu\n";
        Sleep(500);
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addMemberMenu(library);
            break;
        case 2:
            editMemberMenu(library);
            break;
        case 3:
            deleteMemberMenu(library);
            break;
        case 4:
            displayMembersMenu(library);
            break;
        case 5: {
            int id;
            cout << "Enter member ID: ";
            cin >> id;
            Member* member = library.search_member_by_id(id);
            if (member) {
                cout << member->get_details() << endl;
            }
            else {
                cout << "Member not found!" << endl;
            }
            system("pause");
            break;
        }
        case 6: {
            string name;
            cout << "Enter member name: ";
            cin.ignore();
            getline(cin, name);
            vector<Member*> members = library.search_member_by_name(name);
            if (!members.empty()) {
                for (auto member : members) {
                    cout << member->get_details() << endl;
                }
            }
            else {
                cout << "No members found!" << endl;
            }
            system("pause");
            break;
        }
        case 0:
            return;
        default:
            cout << "Invalid choice! Try again.\n";
            break;
        }
    } while (true);
}


void bookMenu(Library& library) {
    int choice;
    do {
        clearScreen();
        cout << "Book Management Menu:\n";
        cout << "1. Add Book\n";
        cout << "2. Edit Book\n";
        cout << "3. Delete Book\n";
        cout << "4. Display Books\n";
        cout << "5. Borrow Book\n";
        cout << "6. Return Book\n";
        cout << "7. Display Borrowed Books\n";
        cout << "8. Check Overdue Books\n";
        cout << "9. Search Book by ISBN\n";
        cout << "10. Search Book by Title\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addBookMenu(library);
            break;
        case 2:
            editBookMenu(library);
            break;
        case 3:
            deleteBookMenu(library);
            break;
        case 4:
            displayBooksMenu(library);
            break;
        case 5:
            borrowBookMenu(library);
            break;
        case 6:
            returnBookMenu(library);
            break;
        case 7:
            displayBorrowedBooksMenu(library);
            break;
        case 8:
            checkOverdueBooksMenu(library);
            break;
        case 9: {
            string isbn;
            cout << "Enter book ISBN: ";
            cin >> ws;
            getline(cin, isbn);
            Book* book = library.search_book_by_isbn(isbn);
            if (book) {
                cout << "ISBN: " << book->isbn << " | Title: " << book->title << " | Author: " << book->author << " | Publisher: " << book->publisher << " | Year: " << book->year << endl;
            }
            else {
                cout << "Book not found!" << endl;
            }
            system("pause");
            break;
        }
        case 10: {
            string title;
            cout << "Enter book title: ";
            cin.ignore();
            getline(cin, title);
            vector<Book*> books = library.search_book_by_title(title);
            if (!books.empty()) {
                for (auto book : books) {
                    cout << "ISBN: " << book->isbn << " | Title: " << book->title << " | Author: " << book->author << " | Publisher: " << book->publisher << " | Year: " << book->year << endl;
                }
            }
            else {
                cout << "No books found!" << endl;
            }
            system("pause");
            break;
        }
        case 0:
            return;
        default:
            cout << "Invalid choice! Try again.\n";
            break;
        }
    } while (true);
}


void addMemberMenu(Library& library) {
    int id;
    string name, email, type, department, position, major;

    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter name: ";
    cin >> ws;
    getline(cin, name);
    cout << "Enter email: ";
    getline(cin, email);
    cout << "Enter type (Professor/Employee/Student): ";
    getline(cin, type);

    if (type == "Professor") {
        cout << "Enter department: ";
        getline(cin, department);
        library.add_member(new Professor(id, name, email, department));
    }
    else if (type == "Employee") {
        cout << "Enter position: ";
        getline(cin, position);
        library.add_member(new Employee(id, name, email, position));
    }
    else if (type == "Student") {
        cout << "Enter major: ";
        getline(cin, major);
        library.add_member(new Student(id, name, email, major));
    }
    else {
        cout << "Invalid member type!\n";
    }
}

void editMemberMenu(Library& library) {
    int id;
    string name, email;

    cout << "Enter ID of the member to edit: ";
    cin >> id;
    cout << "Enter new name: ";
    cin >> ws;
    getline(cin, name);
    cout << "Enter new email: ";
    getline(cin, email);

    library.edit_member(id, name, email);
}

void deleteMemberMenu(Library& library) {
    int id;

    cout << "Enter ID of the member to delete: ";
    cin >> id;

    library.delete_member(id);
}

void displayMembersMenu(Library& library) {
    clearScreen();
    display_admin_info(new Admin(0, "Admin", "admin@example.com"));
    library.display_members();
    system("pause");
}

void addBookMenu(Library& library) {
    string isbn, title, author, publisher;
    int year;

    cout << "Enter ISBN: ";
    cin >> ws;
    getline(cin, isbn);
    cout << "Enter title: ";
    getline(cin, title);
    cout << "Enter author: ";
    getline(cin, author);
    cout << "Enter publisher: ";
    getline(cin, publisher);
    cout << "Enter year: ";
    cin >> year;

    library.add_book(new Book(isbn, title, author, publisher, year));
}

void editBookMenu(Library& library) {
    string isbn, title, author, publisher;
    int year;

    cout << "Enter ISBN of the book to edit: ";
    cin >> ws;
    getline(cin, isbn);
    cout << "Enter new title: ";
    getline(cin, title);
    cout << "Enter new author: ";
    getline(cin, author);
    cout << "Enter new publisher: ";
    getline(cin, publisher);
    cout << "Enter new year: ";
    cin >> year;

    library.edit_book(isbn, title, author, publisher, year);
}

void deleteBookMenu(Library& library) {
    string isbn;

    cout << "Enter ISBN of the book to delete: ";
    cin >> ws;
    getline(cin, isbn);

    library.delete_book(isbn);
}

void displayBooksMenu(Library& library) {
    clearScreen();
    display_admin_info(new Admin(0, "Admin", "admin@example.com"));
    library.display_books();
    system("pause");
}

void borrowBookMenu(Library& library) {
    string isbn;
    int member_id;

    cout << "Enter ISBN of the book to borrow: ";
    cin >> ws;
    getline(cin, isbn);
    cout << "Enter member ID: ";
    cin >> member_id;

    library.borrow_book(isbn, member_id);
}

void returnBookMenu(Library& library) {
    string isbn;
    int member_id;

    cout << "Enter ISBN of the book to return: ";
    cin >> ws;
    getline(cin, isbn);
    cout << "Enter member ID: ";
    cin >> member_id;

    library.return_book(isbn, member_id);
}

void displayBorrowedBooksMenu(Library& library) {
    clearScreen();
    display_admin_info(new Admin(0, "Admin", "admin@example.com"));
    cout << "List of Borrowed Books:\n";
    int member_id = 0; 
    library.display_borrowed_books(member_id); 
    system("pause");
}

void checkOverdueBooksMenu(Library& library) {
    clearScreen();
    display_admin_info(new Admin(0, "Admin", "admin@example.com"));
    cout << "List of Overdue Books:\n";
    time_t current_time = time(0); 
    library.check_overdue_books(current_time); 
    system("pause");
}

void admin_login(Library& library) {
    int id;
    string password;

    clearScreen();
    cout << "Admin Login\n";
    cout << "Enter Admin ID: ";
    cin >> id;
    cout << "Enter Password: ";
    cin >> password;

  
    if (id == 0 && password == "admin") {
        Member* admin = new Admin(id, "Admin", "admin@example.com");
        admin_menu(library, admin);
        delete admin;
    }
    else {
        cout << "Invalid Admin ID or Password. Try again.\n";
        system("pause");
        admin_login(library);
    }
}

void admin_menu(Library& library, Member* admin) {
    int choice;
    do {
        clearScreen();
        display_admin_info(admin);
        cout << "Admin Menu:\n";
        cout << "1. Display Borrowed Books\n";
        cout << "2. Check Overdue Books\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            displayBorrowedBooksMenu(library); 
            break;
        case 2:
            checkOverdueBooksMenu(library); 
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice! Try again.\n";
            break;
        }
    } while (true);
}

int main() {
    Library library;
    library.load_data("members.txt", "books.txt");

    Sleep(1000);
    mainMenu();
    Sleep(1000);
    library.save_data("Members.txt", "Books.txt");

    return 0;
}