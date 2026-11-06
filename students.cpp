#include "system.h"

Student* findId(int id) {
    Student* current = head;
    while (current != NULL) {
        if (current->id == id) return current;
        current = current->next;
    }
    return NULL;
}

Student* findName(string name) {
    Student* current = head;
    while (current != NULL) {
        if (nameKey(current->name) == nameKey(name)) return current;
        current = current->next;
    }
    return NULL;
}

void addStudent(int id, string name) {
    Student* item = new Student;
    item->id = id;
    item->name = name;
    item->prev = tail;
    item->next = NULL;
    if (tail == NULL) head = item;
    else tail->next = item;
    tail = item;
    studentCount++;
    if (id >= nextId) nextId = id + 1;
}

void showStudents() {
    if (studentCount == 0) { cout << "No students yet.\n"; return; }
    Student** list = sortedStudents();
    cout << "\nID       NAME\n";
    for (int i = 0; i < studentCount; i++) cout << left << setw(9) << list[i]->id << list[i]->name << '\n';
    delete[] list;
}

void manageStudents() {
    cout << "\n1. Add student\n2. Display students\n3. Edit name\n4. Delete student\n5. Import names\n0. Back\n";
    int choice = getNumber("Choice: ", 0, 5);
    if (!cin || choice == 0) return;
    if (choice == 2) { showStudents(); return; }
    if (choice == 1) {
        string name = getText("Full name (Surname, First Name): ");
        if (!cin) return;
        if (findName(name) != NULL) { cout << "That name already exists.\n"; return; }
        addStudent(nextId, name);
    } else if (choice == 5) {
        string path = getText("Text file path (one full name per line): ");
        if (!cin) return;
        ifstream file(path.c_str());
        if (!file) { cout << "File not found.\n"; return; }
        string name;
        int added = 0, skipped = 0;
        while (getline(file, name)) {
            name = trim(name);
            if (name.empty() || findName(name) != NULL) { skipped++; continue; }
            addStudent(nextId, name);
            added++;
        }
        cout << "Added: " << added << " | Skipped blank/duplicate names: " << skipped << '\n';
    } else {
        showStudents();
        if (studentCount == 0) return;
        int id = getNumber("Student ID (0 to cancel): ", 0, 1000000000);
        if (!cin || id == 0) return;
        Student* student = findId(id);
        if (student == NULL) { cout << "Student not found.\n"; return; }
        if (choice == 3) {
            string name = getText("New full name: ");
            if (!cin) return;
            Student* duplicate = findName(name);
            if (duplicate != NULL && duplicate != student) { cout << "That name already exists.\n"; return; }
            student->name = name;
        } else {
            cout << "Delete " << student->name << " and ALL their attendance history?\n";
            if (getNumber("1 = Yes, 0 = Cancel: ", 0, 1) != 1 || !cin) return;
            Record* current = firstRecord;
            while (current != NULL) {
                Record* next = current->next;
                if (current->id == id) removeRecord(current);
                current = next;
            }
            if (student->prev != NULL) student->prev->next = student->next;
            else head = student->next;
            if (student->next != NULL) student->next->prev = student->prev;
            else tail = student->prev;
            delete student;
            studentCount--;
        }
    }
    saveData();
}
