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

struct Student {
    int id;
    string name;
    Student* prev;
    Student* next;
};

struct Record {
    int id;
    string date;
    string time;
    int status;
    Record* prev;
    Record* next;
};

struct QueueNode {
    Student* student;
    QueueNode* next;
};

extern Student* head;
extern Student* tail;
extern Record* firstRecord;
extern Record* lastRecord;
extern QueueNode* front;
extern QueueNode* rear;
extern int nextId;
extern int studentCount;

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
void runBenchmark();

#endif
