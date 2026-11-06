// System Header file (keep adding functions here)

#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cstdio>
using namespace std;

// doubly linked; list student per node having id, name, and prev as well as next values
struct Student {
    int id;
    string name;
    Student* prev;
    Student* next;
};

// one attendance entry linked to the student by ID where 2 = present, 1 = late, & 0 = absent.
struct Record {
    int id;
    string date;
    string time;
    int status;
    Record* prev;
    Record* next;
};

// one waiting student in attendance queue. Student points to existing student, next points to next queue node
struct QueueNode {
    Student* student;
    QueueNode* next;
};

// needed so that we declare shared variables without another copy. Actual definitions are in main.cpp 
extern Student* head;
extern Student* tail;
// first and last attendance recs
extern Record* firstRecord;
extern Record* lastRecord;
// first and last nodes in attendance queue
extern QueueNode* front;
extern QueueNode* rear;
// next student ID to assign and the current number of students
extern int nextId;
extern int studentCount;


// FUHCTION DECLARATIONS (function definitions are in .cpp files)
string trim(string text);
string nameKey(string text);
bool parseInt(string text, int& value);
string getText(string prompt);
int getNumber(string prompt, int low, int high);
bool validDate(string date);
bool validTime(string time);
Student* findId(int id);
Student* findName(string name);
void addStudent(int id, string name);
void addRecord(int id, string date, string time, int status);
Record* findRecord(int id, string date, string time);
string statusText(int status);
void merge(Student* list[], Student* temp[], int left, int middle, int right);
void mergeSort(Student* list[], Student* temp[], int left, int right);
Student** sortedStudents();
int binarySearch(Student* list[], int count, string name);
void enqueue(Student* student);
bool isEmpty();
Student* peek();
void dequeue();
string csvText(string text);
bool splitCsv(string line, string fields[], int expected);
bool saveData();
bool loadData();
void showStudents();
void removeRecord(Record* item);
void manageStudents();
void checkAttendance();
void showHistory(int id);
void historyMenu();
void searchMenu();
void groupStudents();
void clearData();
// benchmark function to run separate test and write mmeasureents to CSV
void runBenchmark();

#endif
