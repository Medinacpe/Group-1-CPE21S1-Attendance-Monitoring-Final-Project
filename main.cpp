#include "system.h"

Student* head = NULL;
Student* tail = NULL;
Record* firstRecord = NULL;
Record* lastRecord = NULL;
QueueNode* front = NULL;
QueueNode* rear = NULL;
int nextId = 1;
int studentCount = 0;

string trim(string text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

bool parseInt(string text, int& value) {
    stringstream input(text);
    char extra;
    if (!(input >> value)) return false;
    return !(input >> extra);
}

string getText(string prompt) {
    string text;
    while (true) {
        cout << prompt;
        if (!getline(cin, text)) return "";
        text = trim(text);
        if (!text.empty()) return text;
        cout << "Please enter a value.\n";
    }
}

int getNumber(string prompt, int low, int high) {
    string text;
    int value;
    while (true) {
        cout << prompt;
        if (!getline(cin, text)) return low;
        if (parseInt(text, value) && value >= low && value <= high) return value;
        cout << "Invalid input. Enter " << low << " to " << high << ".\n";
    }
}

string csvText(string text) {
    string result = "\"";
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '"') result += '"';
        result += text[i];
    }
    return result + "\"";
}

bool splitCsv(string line, string fields[], int expected) {
    int count = 0;
    string field;
    bool quoted = false, closed = false;
    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (quoted) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"';
                    i++;
                } else { quoted = false; closed = true; }
            } else field += c;
        } else if (c == ',') {
            if (count >= expected) return false;
            fields[count++] = field;
            field = "";
            closed = false;
        } else if (c == '"') {
            if (!field.empty() || closed) return false;
            quoted = true;
        } else {
            if (closed) return false;
            field += c;
        }
    }
    if (quoted || count != expected - 1) return false;
    fields[count] = field;
    return true;
}

bool saveData() {
    ofstream file("master.tmp");
    if (!file) { cout << "Cannot open save file. Changes remain in memory.\n"; return false; }
    file << "type,id,name,date,time,status\n";
    Student* student = head;
    while (student != NULL) {
        file << "S," << student->id << ',' << csvText(student->name) << ",,,\n";
        student = student->next;
    }
    Record* record = firstRecord;
    while (record != NULL) {
        student = findId(record->id);
        file << "A," << record->id << ',' << csvText(student->name) << ','
             << record->date << ',' << record->time << ',' << record->status << '\n';
        record = record->next;
    }
    file.close();
    if (!file) { cout << "Saving failed. Original file was kept.\n"; return false; }
    ifstream old("master.csv");
    bool exists = old.good();
    old.close();
    if (exists) {
        remove("master.bak");
        if (rename("master.csv", "master.bak") != 0) {
            cout << "Cannot replace master.csv. Close it in other programs.\n";
            return false;
        }
    }
    if (rename("master.tmp", "master.csv") != 0) {
        if (exists) rename("master.bak", "master.csv");
        cout << "Saving failed. Check folder permissions.\n";
        return false;
    }
    cout << "Saved to master.csv.\n";
    return true;
}

bool loadData() {
    ifstream file("master.csv");
    if (!file) {
        ifstream backup("master.bak");
        if (backup.good()) {
            cout << "master.csv is missing but master.bak exists. Restore the backup first.\n";
            return false;
        }
        cout << "Starting a new class.\n";
        return true;
    }
    string line, fields[6];
    if (!getline(file, line) || trim(line) != "type,id,name,date,time,status") return false;
    bool recordsStarted = false;
    while (getline(file, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (!splitCsv(line, fields, 6)) return false;
        int id, status;
        if (!parseInt(fields[1], id) || id < 1 || id > 1000000000) return false;
        if (fields[0] == "S") {
            if (recordsStarted || trim(fields[2]).empty() || findId(id) != NULL || findName(fields[2]) != NULL) return false;
            if (!fields[3].empty() || !fields[4].empty() || !fields[5].empty()) return false;
            addStudent(id, fields[2]);
        } else if (fields[0] == "A") {
            recordsStarted = true;
            if (findId(id) == NULL || !validDate(fields[3]) || !validTime(fields[4])) return false;
            if (!parseInt(fields[5], status) || status < 0 || status > 2) return false;
            if (findRecord(id, fields[3], fields[4]) != NULL) return false;
            addRecord(id, fields[3], fields[4], status);
        } else return false;
    }
    return !file.bad();
}

void clearData() {
    while (!isEmpty()) dequeue();
    while (firstRecord != NULL) removeRecord(firstRecord);
    while (head != NULL) {
        Student* next = head->next;
        delete head;
        head = next;
    }
    tail = NULL;
    studentCount = 0;
    nextId = 1;
}

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));
    if (argc > 1 && string(argv[1]) == "--benchmark") {
        runBenchmark();
        return 0;
    }
    cout << "STUDENT ATTENDANCE MONITORING SYSTEM\n";
    if (!loadData()) {
        cout << "Cannot load saved data safely. Check master.csv; it has not been overwritten.\n";
        clearData();
        return 1;
    }
    while (cin) {
        cout << "\n1. Check attendance\n2. Manage students\n3. View / correct attendance history\n4. Search student\n5. Generate groups\n6. Save\n0. Save and exit\n";
        int choice = getNumber("Choice: ", 0, 6);
        if (!cin || choice == 0) {
            if (saveData() || !cin) break;
            cout << "Save failed. You can retry or choose to exit without saving.\n";
            if (getNumber("1 = Exit without saving, 0 = Return: ", 0, 1) == 1) break;
        } else if (choice == 1) checkAttendance();
        else if (choice == 2) manageStudents();
        else if (choice == 3) historyMenu();
        else if (choice == 4) searchMenu();
        else if (choice == 5) groupStudents();
        else saveData();
    }
    clearData();
    cout << "Program ended.\n";
    return 0;
}
