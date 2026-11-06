# Student Attendance Monitoring System

This project is just a basic C++ console-based project for attendance monitoring in a single class. The program is split into exactly six '.cpp' files where all of them use a single shared 'system.h' making use of different functions, structs, points, arrays, loops, and file streams. Additionally, external libraries, databases, GUI, OOP, and etc. are out of this project's scope and are not implemented along within it.



## Source file layout

| File | Responsibility |
|---|---|
| `main.cpp` | Includes Main Menu, Cleaning, Global Definitions, Input Helpers |
| `students.cpp` | Student Management, Doubly Linked Student List, Linear Search |
| `attendance.cpp` | Linked list Queue, Session date/time Validation, Attendance Checking |
| `history.cpp` | Doubly Linked Attendance Records, History, Status Corrections |
| `search.cpp` | Name Comparison, Merge Sort, Binary Search, Timing Benchmark |
| `grouping.cpp` | Alphabetical and Randomized Grouping |
| `system.h` | Shared Structs, Extern and Function Declarations, Include Guard |

Every source file includes 'system.h' where Only main.cpp defines main().



## Build and run

Windows with g++ (MinGW):

```
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp students.cpp attendance.cpp history.cpp search.cpp grouping.cpp -o attendance.exe
.\attendance.exe
```

The program is easily compiled and built into an .exe file by clicking `01_BuildToEXE.bat`.



## Example Use-Case (Demonstration):

1. First, pick 2 (Manage students), then 5 (Import names) to properly start with the program.
2. Input `sample_names.txt` (or any named file with student names you wish to import).
3. Choose 2, then 2 to display the imported student names in their order (alphabetical).
4. Pick 1 to start recording attendance, entering a date like `2026-11-06` and a time for the session such as `14:30`.
5. As the attendance checking starts, you may start marking them. 2 = Present, 1 = Late, 0 = Absent. Enter 3 to stop attendance checking early.
6. Repeat the same for the date and time to resume (if exited early) where existing entries are now skipped; unmarked students are NOT assumed absent.
7. Pick 3 to view a student's history by ID (you are able to rectify any student's status within this menu).
8. Choose 4 to search an exact full name.
9. Open 5 to generate alphabetical round robin or random groups.
10. When done, simply pick 0 to save and exit where you can restart to verify the records remain.

The preferred naming convention for the program follows a 'Lastname, First Name' basis as the implemented alphabetical sorting compares the entire entered name per line. Additionally, if students share a name, the user may differentiate between them by changing their middle name or initial. IDs are assigned automatically and blank or duplicate lines within the list of names to be imported are skipped. Moroever, names can have comas and quotations marks.



## Features and data structures

| Feature | Implementation |
|---|---|
| Student management | Doubly linked student list; linear search by ID/name |
| Attendance checking | Alphabetically ordered linked-list FIFO queue |
| Attendance storage | Doubly linked attendance-record list |
| Name sorting | Manually implemented recursive merge sort on a temporary array of student pointers |
| Name search | Manually implemented iterative binary search on the sorted pointer array |
| History and correction | Linear scan of attendance records; exact ID/date/time lookup |
| Grouping | Round-robin assignment; optional simple random shuffle first |
| Persistence | Where the main CSV file is loaded and saved through file streams |

The program maintains both previous and next pointers in its doubly linked lists. It traverses forward when displaying history; history appears in recording order, not necessarily date order. Moreover, grouping includes the whole current class and is displayed only, not ACTUALLY saved. Group sizes differ by at most one. Random grouping uses basic `rand()`.

It counts as a session by how the exact date and time is entered by the user, where multiple sessions on one day are possible and are actually supported. Additionally, changing the entered time means that it's a different session where deleting a student requires confirmation and deletes all their attendance history as well. Additionally, renaming students should preserve their ID and records.



## Saved files

The file, `master.csv` is created after saving in which it contains both the student rows (`S`) and attendance rows (`A`), with columns:

```
[ type, id, name, date, time, status ]
```

For the explanation of the 'S' and 'A' designations, student rows are just basically preserved students with no attendance yet. The attendance rows should include the name, date, time, ID, as well as the numerical status where CSV names are escaped so commas and quotes are able to round trip correctly. Additionally, when viewing in a spreadsheet, you can just filter the type column to A to only see the attendance records.

Additional files may be created, such as with `master.tmp` which is a temporary save file. `master.bak` is basically a previous state where the file was successfully saved, backed up.



## Experimental timing

The project also includes a separate benchmarking .bat, `02_BuildBenchmarking.bat` file that uses the compiled .exe file to run an experimental, benchamarking analysis where it does NOT load or modify the class attendance data in anyway.

Windows:

```
g++ -std=c++11 -O2 main.cpp students.cpp attendance.cpp history.cpp search.cpp grouping.cpp -o attendance.exe
.\attendance.exe --benchmark
```

The generated benchmarking file, `benchmark_results.csv` is created in the same folder where you can run it, and it tests n = 100, 1,000, 10,000, and 100,000 of fictional students with each size using five runs. Additionally, linear search would traverse the whole linked list where binary search would use an already sorted array. Furthermore, merge sort is would be timed in a seperate manner where it has an alphabetical input that is reversed and is reset before each of the different n runs.



## Complexity notes for explaining the implementation

A single enqueue/dequeue is O(1) where processing n queued students is O(n). Merge sort should have a time complexity of O(n log n) where binary search only has O(log n). The interactive search first makes and sorts its array so that complete menu operation includes the O(n log n) preparation. Lastly, linear student lookup is O(n). Attendance duplicate checks scan r existing attendance records for each student, so preparing the queue can take O(nr), in addition to sorting. Saving also looks up each attendance record's student by ID, costing O(nr) plus writing. This intendedly designed simple implementation should not be described as O(1) for the whole attendance feature. History scanning is O(r). Unlinking a known node is O(1); finding it is not. Student deletion also scans the attendance list. Import duplicate checking can be quadratic as the roster grows. Actual name comparisons also depend on name length.



## Verification performed

Compiled using C++11 and developed within VS Code among 6 group members with warning flags. Scripted checks have covered import, duplicate rejection, invalid menu/status/date/time input, alphabetical queue order, stopping/resuming a session, exact name search, history correction, rename persistence, quoted CSV round-trips, both grouping modes, student/history deletion, restart, malformed-file protection, and end-of-input handling. The benchmark was run separately.



## Team Members - CPE21S1 / Group 1
---
* Alvarez, Joshua Caleb P 
* Baltazar, Kenji
* Formilleza, Huone Anthony
* Garcia, Vince Joseph R.
* Medina, Jomz Ryle E.
* Regala, Joaquin Luiz B.
