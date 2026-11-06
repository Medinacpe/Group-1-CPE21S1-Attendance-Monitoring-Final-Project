#include "system.h"

// This checks if the date follows the right format and if it is a valid date in the calendar.
bool validDate(string date){
    if(date.size() != 10 || date[4] != '-' || date[7] != '-'){
        return false;
    }
    for(int i = 0; i < 10; i++){
        if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9')){
            return false;
        }
    }
    int y = atoi(date.substr(0, 4).c_str());
    int m = atoi(date.substr(5, 2).c_str());
    int d = atoi(date.substr(8, 2).c_str());
    int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(y < 1900 || m < 1 || m > 12){
        return false;
    }
    if(y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)){
        days[1] = 29;
    }
    return d >= 1 && d <= days[m - 1];
}

bool validTime(string time){
    if(time.size() != 5 || time[2] != ':'){
        return false;
    }
    for(int i = 0; i < 5; i++){
        if(i != 2 && (time[i] < '0' || time[i] > '9')){
            return false;
        }
    }
    return atoi(time.substr(0, 2).c_str()) < 24 && atoi(time.substr(3, 2).c_str()) < 60;
}

void enqueue(Student* student){
    QueueNode* item = new QueueNode;
    item->student = student;
    item->next = NULL;
    if(rear == NULL){
        front = item;
    }
    else{
        rear->next = item;
    }
    rear = item;
}

bool isEmpty(){
    return front == NULL;
}

Student* peek(){
    if(isEmpty()){
        return NULL;
    }
    return front->student;
}

void dequeue(){
    if(isEmpty()){
        return;
    }
    QueueNode* item = front;
    front = front->next;
    if(front == NULL){
        rear = NULL;
    }
    delete item;
}

void checkAttendance(){
    if(studentCount == 0){
        cout << "Add students first.\n";
        return;
    }
    string date, time;
    do{
        date = getText("Session date (YYYY-MM-DD): ");
        if(!cin){
            return;
        }
        if(!validDate(date)){
            cout << "Invalid date.\n";
        }
    } while (!validDate(date));
    do{
        time = getText("Session time (HH:MM, 24-hour): ");
        if(!cin){
            return;
        }
        if(!validTime(time)){
            cout << "Invalid time.\n";
        }
    }while(!validTime(time));
    Student** list = sortedStudents();
    int skipped = 0;
    
    //Only add the student's that do not have attendance record for just this session in time
    for(int i = 0; i < studentCount; i++){
        if(findRecord(list[i]->id, date, time) == NULL){
            enqueue(list[i]);
        }
        else{
            skipped++;
        }
    }
    delete[] list;
    cout << "Existing entries skipped: " << skipped
         << "\n2 = Present | 1 = Late | 0 = Absent | 3 = Stop\n";
    while(!isEmpty()){
        Student* student = peek();
        cout << "\n" << student->id << " - " << student->name << '\n';
        int status = getNumber("Status: ", 0, 3);
        if(!cin || status == 3){
            break;
        }
        addRecord(student->id, date, time, status);
        dequeue();
    }
    while(!isEmpty()){
        dequeue();
    }
    saveData();
    cout << "Unmarked students remain unrecorded. Repeat this session to continue.\n";
}
