#include "system.h"

void addRecord(int id, string date, string time, int status) {
    Record* item = new Record;
    item->id = id;
    item->date = date;
    item->time = time;
    item->status = status;
    item->prev = lastRecord;
    item->next = NULL;
    if (lastRecord == NULL) firstRecord = item;
    else lastRecord->next = item;
    lastRecord = item;
}

Record* findRecord(int id, string date, string time) {
    Record* current = firstRecord;
    while (current != NULL) {
        if (current->id == id && current->date == date && current->time == time) return current;
        current = current->next;
    }
    return NULL;
}

string statusText(int status) {
    if (status == 2) return "Present";
    if (status == 1) return "Late";
    return "Absent";
}

void removeRecord(Record* item) {
    if (item->prev != NULL) item->prev->next = item->next;
    else firstRecord = item->next;
    if (item->next != NULL) item->next->prev = item->prev;
    else lastRecord = item->prev;
    delete item;
}

void showHistory(int id) {
    int counts[3] = {0,0,0};
    Record* current = firstRecord;
    cout << "\nDATE        TIME    STATUS\n";
    while (current != NULL) {
        if (current->id == id) {
            cout << current->date << "  " << current->time << "   " << statusText(current->status) << '\n';
            counts[current->status]++;
        }
        current = current->next;
    }
    cout << "Present: " << counts[2] << " | Late: " << counts[1] << " | Absent: " << counts[0] << '\n';
    if (counts[0] + counts[1] + counts[2] == 0) cout << "No attendance records.\n";
}

void historyMenu() {
    showStudents();
    if (studentCount == 0) return;
    int id = getNumber("Student ID (0 to cancel): ", 0, 1000000000);
    if (!cin || id == 0) return;
    Student* student = findId(id);
    if (student == NULL) { cout << "Student not found.\n"; return; }
    cout << student->name << '\n';
    showHistory(id);
    if (getNumber("1 = Correct an entry, 0 = Back: ", 0, 1) == 0 || !cin) return;
    string date = getText("Entry date (YYYY-MM-DD): ");
    if (!cin) return;
    string time = getText("Entry time (HH:MM): ");
    if (!cin) return;
    Record* record = findRecord(id, date, time);
    if (record == NULL) { cout << "Attendance entry not found.\n"; return; }
    int status = getNumber("New status (2 = Present, 1 = Late, 0 = Absent): ", 0, 2);
    if (!cin) return;
    record->status = status;
    saveData();
}
