#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
using namespace std;

class Student {
private:
    int rollNo;
    string name;
    string course;
    float marks;

public:
    Student() {}
    Student(int r, string n, string c, float m)
        : rollNo(r), name(n), course(c), marks(m) {}

    int getRollNo() const { return rollNo; }
    string getName() const { return name; }
    string getCourse() const { return course; }
    float getMarks() const { return marks; }

    void setRollNo(int r) { rollNo = r; }
    void setName(string n) { name = n; }
    void setCourse(string c) { course = c; }
    void setMarks(float m) { marks = m; }

    void display() const {
        cout << left << setw(10) << rollNo
             << setw(20) << name
             << setw(15) << course
             << setw(10) << marks << endl;
    }

    void writeToFile(ofstream &out) const {
        out << rollNo << "," << name << "," << course << "," << marks << "\n";
    }

    static Student readFromLine(const string &line) {
        Student s;
        size_t pos = 0, start = 0;
        vector<string> tokens;

        while ((pos = line.find(',', start)) != string::npos) {
            tokens.push_back(line.substr(start, pos - start));
            start = pos + 1;
        }
        tokens.push_back(line.substr(start));

        if (tokens.size() == 4) {
            s.rollNo = stoi(tokens[0]);
            s.name = tokens[1];
            s.course = tokens[2];
            s.marks = stof(tokens[3]);
        }
        return s;
    }
};

class StudentManager {
private:
    vector<Student> students;
    const string filename = "students.txt";

public:
    StudentManager() {
        loadFromFile();
    }

    void loadFromFile() {
        students.clear();
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            if (!line.empty())
                students.push_back(Student::readFromLine(line));
        }
        file.close();
    }

    void saveToFile() const {
        ofstream file(filename);
        for (auto &s : students) {
            s.writeToFile(file);
        }
        file.close();
    }

    void addStudent() {
        int roll;
        string name, course;
        float marks;
        cout << "Enter Roll No: ";
        cin >> roll;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Course: ";
        getline(cin, course);
        cout << "Enter Marks: ";
        cin >> marks;

        students.push_back(Student(roll, name, course, marks));
        saveToFile();
        cout << "\n✅ Student Added Successfully!\n";
    }

    void displayAll() const {
        cout << left << setw(10) << "RollNo"
             << setw(20) << "Name"
             << setw(15) << "Course"
             << setw(10) << "Marks" << endl;
        cout << string(55, '-') << endl;
        for (auto &s : students)
            s.display();
    }

    void searchStudent() const {
        int roll;
        cout << "Enter Roll No to Search: ";
        cin >> roll;
        bool found = false;
        for (auto &s : students) {
            if (s.getRollNo() == roll) {
                cout << "\nStudent Found:\n";
                s.display();
                found = true;
                break;
            }
        }
        if (!found)
            cout << "❌ Student Not Found!\n";
    }

    void deleteStudent() {
        int roll;
        cout << "Enter Roll No to Delete: ";
        cin >> roll;
        bool found = false;

        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getRollNo() == roll) {
                students.erase(it);
                found = true;
                break;
            }
        }

        if (found) {
            saveToFile();
            cout << "✅ Student Deleted Successfully!\n";
        } else {
            cout << "❌ Student Not Found!\n";
        }
    }

    void updateStudent() {
        int roll;
        cout << "Enter Roll No to Update: ";
        cin >> roll;
        bool found = false;

        for (auto &s : students) {
            if (s.getRollNo() == roll) {
                found = true;
                string name, course;
                float marks;
                cin.ignore();
                cout << "Enter New Name: ";
                getline(cin, name);
                cout << "Enter New Course: ";
                getline(cin, course);
                cout << "Enter New Marks: ";
                cin >> marks;

                s.setName(name);
                s.setCourse(course);
                s.setMarks(marks);
                saveToFile();

                cout << "✅ Student Updated Successfully!\n";
                break;
            }
        }

        if (!found)
            cout << "❌ Student Not Found!\n";
    }
};

int main() {
    StudentManager manager;
    int choice;

    do {
        cout << "\n========= STUDENT MANAGEMENT SYSTEM =========\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            manager.addStudent();
            break;
        case 2:
            manager.displayAll();
            break;
        case 3:
            manager.searchStudent();
            break;
        case 4:
            manager.updateStudent();
            break;
        case 5:
            manager.deleteStudent();
            break;
        case 6:
            cout << "Exiting... Goodbye!\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
