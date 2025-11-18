#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

// Structure to store student information
struct Student
{
    int id;
    string name;
    int age;
    int room;
    double fees;
    bool paid;
};

// Function declarations
Student* createStudentList(int capacity);
void showMenu();
int getUserChoice();
void addStudentEntry(Student *students, int &studentCount);
void deleteStudentEntry(Student *students, int &studentCount);
void updateStudentEntry(Student *students, int studentCount);
void searchStudentById(Student *students, int studentCount);
void searchStudentByName(Student *students, int studentCount);
void displayAllStudents(Student *students, int studentCount);
int findStudentIndex(Student *students, int studentCount, int id);
void processFeePayment(Student *students, int studentCount);
double calculateTotalFees(Student *students, int studentCount);
double calculateCollectedFees(Student *students, int studentCount);
void showStudentInfo(const Student &s);
void saveStudentData(Student *students, int studentCount);
void loadStudentData(Student *students, int &studentCount);
void generateReportFile(Student *students, int studentCount);
void clearAllData(int &studentCount);
void pauseScreen();
void exitProgram(Student *students, int studentCount);
string toLowerCase(const string &s);

int main()
{
    int capacity = 100;
    int studentCount = 0;
    Student *hostelStudents = createStudentList(capacity);

    // Load data from previous sessions if available
    loadStudentData(hostelStudents, studentCount);

    while (true)
    {
        showMenu();
        int choice = getUserChoice();

        if (choice == 10)
        {
            exitProgram(hostelStudents, studentCount);
            break;
        }
        else if (choice == 1)
            addStudentEntry(hostelStudents, studentCount);
        else if (choice == 2)
            deleteStudentEntry(hostelStudents, studentCount);
        else if (choice == 3)
            updateStudentEntry(hostelStudents, studentCount);
        else if (choice == 4)
            searchStudentById(hostelStudents, studentCount);
        else if (choice == 5)
            searchStudentByName(hostelStudents, studentCount);
        else if (choice == 6)
            displayAllStudents(hostelStudents, studentCount);
        else if (choice == 7)
            processFeePayment(hostelStudents, studentCount);
        else if (choice == 8)
            generateReportFile(hostelStudents, studentCount);
        else if (choice == 9)
            clearAllData(studentCount);
        else
            cout << "Invalid choice.\n";

        pauseScreen();
    }

    return 0;
}

// dynamic memory allocation to student list
Student* createStudentList(int capacity)
{
    return new Student[capacity];
}

// menu options
void showMenu()
{
    cout << "\n===== HOSTEL MANAGEMENT SYSTEM =====\n";
    cout << "1. Add Student\n2. Delete Student\n3. Update Student\n4. Search by ID\n";
    cout << "5. Search by Name\n6. Display All Students\n7. Pay Fees\n8. Generate Report\n";
    cout << "9. Clear All Data\n10. Save & Exit\n";
}

// Get choice from user
int getUserChoice()
{
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore(); // clear newline before getline for names
    return choice;
}

// Addition of new students + duplicate check
void addStudentEntry(Student *students, int &studentCount)
{
    int id;
    cout << "Enter Student ID: ";
    cin >> id;

    // Check for duplicate ID
    for (int i = 0; i < studentCount; i++)
        if (students[i].id == id)
        {
            cout << "Student with this ID already exists!\n";
            cin.ignore();
            return;
        }

    cin.ignore(); // clear newline
    students[studentCount].id = id;

    cout << "Enter Name: ";
    getline(cin, students[studentCount].name);

    cout << "Enter Age: ";
    cin >> students[studentCount].age;
    cout << "Enter Room Number: ";
    cin >> students[studentCount].room;
    cout << "Enter Fees: ";
    cin >> students[studentCount].fees;
    cin.ignore();

    students[studentCount].paid = false;
    studentCount++;
    cout << "Student added successfully!\n";
}

// For deletion of student by ID
void deleteStudentEntry(Student *students, int &studentCount)
{
    int id;
    cout << "Enter Student ID to delete: ";
    cin >> id;
    int idx = findStudentIndex(students, studentCount, id);
    if (idx == -1) { cout << "Student not found.\n"; return; }

    for (int i = idx; i < studentCount - 1; i++)
        students[i] = students[i + 1];
    studentCount--;
    cout << "Student deleted successfully!\n";
}

// For updating room and fees for a student
void updateStudentEntry(Student *students, int studentCount)
{
    int id;
    cout << "Enter Student ID to update: ";
    cin >> id;
    int idx = findStudentIndex(students, studentCount, id);
    if (idx == -1) { cout << "Student not found.\n"; return; }

    cout << "Enter new Room Number: ";
    cin >> students[idx].room;
    cout << "Enter new Fees: ";
    cin >> students[idx].fees;
    cout << "Student updated successfully!\n";
}

// To search student by ID
void searchStudentById(Student *students, int studentCount)
{
    int id;
    cout << "Enter Student ID to search: ";
    cin >> id;
    int idx = findStudentIndex(students, studentCount, id);
    if (idx != -1)
        showStudentInfo(students[idx]);
    else
        cout << "Student not found.\n";
}

// To search student by Name i.e (case-insensitive)
void searchStudentByName(Student *students, int studentCount)
{
    string name;
    cout << "Enter Student Name to search: ";
    cin.ignore();
    getline(cin, name);
    bool found = false;
    string key = toLowerCase(name);
    for (int i = 0; i < studentCount; i++)
    {
        if (toLowerCase(students[i].name).find(key) != string::npos)
        {
            showStudentInfo(students[i]);
            found = true;
        }
    }
    if (!found) cout << "Student not found.\n";
}

// To display all students
void displayAllStudents(Student *students, int studentCount)
{
    if (studentCount == 0) { cout << "No students to display.\n"; return; }
    for (int i = 0; i < studentCount; i++)
        showStudentInfo(students[i]);
}

// To find student index by ID
int findStudentIndex(Student *students, int studentCount, int id)
{
    for (int i = 0; i < studentCount; i++)
        if (students[i].id == id) return i;
    return -1;
}

// Process fee payment
void processFeePayment(Student *students, int studentCount)
{
    int id;
    cout << "Enter Student ID to pay fees: ";
    cin >> id;
    int idx = findStudentIndex(students, studentCount, id);
    if (idx != -1)
    {
        students[idx].paid = true;
        cout << "Fees paid successfully!\n";
    }
    else
        cout << "Student not found.\n";
}

// Calculation of total fees
double calculateTotalFees(Student *students, int studentCount)
{
    double total = 0;
    for (int i = 0; i < studentCount; i++)
        total += students[i].fees;
    return total;
}

// Calculation of collected fees
double calculateCollectedFees(Student *students, int studentCount)
{
    double total = 0;
    for (int i = 0; i < studentCount; i++)
        if (students[i].paid) total += students[i].fees;
    return total;
}

// For displaying single student info
void showStudentInfo(const Student &s)
{
    cout << "\nID: " << s.id << "\nName: " << s.name << "\nAge: " << s.age
         << "\nRoom: " << s.room << "\nFees: " << s.fees << "\nPaid: ";
    if (s.paid)
        cout << "Yes\n";
    else
        cout << "No\n";
}

// For Saving data to text and binary files
void saveStudentData(Student *students, int studentCount)
{
    ofstream out("hostel.txt");
    out << studentCount << '\n';
    for (int i = 0; i < studentCount; i++)
    {
        out << students[i].id << endl;
        out << students[i].name << endl;
        out << students[i].age << endl;
        out << students[i].room << endl;
        out << students[i].fees << endl;
       
    	if(students[i].paid)
    	    out << 1 << '\n';
    	else 
    		out << 0 << '\n';
    }
    out.close();

    ofstream binFile("hostel.bin", ios::binary);
    binFile.write((char*)&studentCount, sizeof(studentCount));
    binFile.write((char*)students, sizeof(Student) * studentCount);
    binFile.close();
}

// To Load data from text file
void loadStudentData(Student *students, int &studentCount)
{
    studentCount = 0;
    ifstream in("hostel.txt");
    if (!in) return;

    in >> studentCount;
    in.ignore();

    for (int i = 0; i < studentCount; i++)
    {
        in >> students[i].id;
        in.ignore();
        getline(in, students[i].name);
        in >> students[i].age;
        in >> students[i].room;
        in >> students[i].fees;
        int paidInt;
        in >> paidInt;
        students[i].paid = (paidInt == 1);
        in.ignore();
    }
    in.close();
}

// Convert string to lowercase for case-insensitive search
string toLowerCase(const string &s)
{
    string r = s;
    for (int i = 0; i < (int)r.length(); i++)
        r[i] = tolower(r[i]);
    return r;
}

// To generate report file
void generateReportFile(Student *students, int studentCount)
{
    ofstream f("report.txt");
    f << "Total Students: " << studentCount << "\n";
    f << "Total Fees: " << calculateTotalFees(students, studentCount) << "\n";
    f << "Collected Fees: " << calculateCollectedFees(students, studentCount) << "\n";
    f.close();
}

// To clear all student data
void clearAllData(int &studentCount)
{
    studentCount = 0;
    cout << "All student data cleared.\n";
}

// Pause screen until ENTER pressed
void pauseScreen()
{
    cout << "Press ENTER to continue...";
    string dummy;
    getline(cin, dummy);
}

// To save data and exit
void exitProgram(Student *students, int studentCount)
{
    saveStudentData(students, studentCount);
    delete[] students;
    cout << "Data saved. Exiting program.\n";
}