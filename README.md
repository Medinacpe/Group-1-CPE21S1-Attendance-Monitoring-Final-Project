# Student Attendance Monitoring System

A basic C++ console project for one a single class. The program is split into exactly six `.cpp` files and one shared `system.h`. It makes use of different functions, structs, raw pointers, arrays, loops, as well as standard file streams. No external libraries, GUI, database, STL containers, or object-oriented class hierarchy are implemented within it.



## Source file layout

| File | Responsibility |
|---|---|
| `main.cpp` | Main Menu, Shared Global Definitions, Input Helpers, CSV loading/saving, Cleanup |
| `students.cpp` | Student Management, Doubly Linked Student List, Linear Search |
| `attendance.cpp` | Linked-list Queue, Session date/time Validation, Attendance Checking |
| `history.cpp` | Doubly Linked Attendance Records, History, Status Corrections |
| `search.cpp` | Name Comparison, Merge Sort, Binary Search, Timing Benchmark |
| `grouping.cpp` | Alphabetical and Randomized Grouping |
| `system.h` | Shared Structs, Extern Declarations, Function Declarations, Include Guard |

Global variables are defined once in main.cpp; the header's extern declarations let the other files access those same variables. Every source file includes 'system.h' where Only main.cpp defines main().



## Build and run

Windows with g++ (MinGW):

```
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp students.cpp attendance.cpp history.cpp search.cpp grouping.cpp -o attendance.exe
.\attendance.exe
```

The program is easily compiled and built into an .exe file by clicking `01_BuildToEXE.bat` if g++ is already on PATH.



## Example Use-Case (Demonstration):

1. Choose 2 (Manage students), then 5 (Import names).
2. Enter `sample_names.txt` (or any named file with student names you wish to import).
3. Choose 2, then 2 to display the alphabetical roster.
4. Choose 1. Enter a date like `2026-11-06` and a session time such as `14:30`.
5. Mark each student: 2 = Present, 1 = Late, 0 = Absent. Enter 3 to stop the checking early.
6. Repeat the same for the date and time to resume. Existing entries are also skipped; unmarked students are NOT assumed absent.
7. Choose 3 to view a student's history by ID, then optionally correct a status.
8. Choose 4 to search an exact full name. ASCII letter case is ignored.
9. Choose 5 to generate alphabetical round-robin or random groups.
10. Choose 0 to save and exit. Restart to verify the records remain.

Use names consistently as `Surname, First Name`. The implemented alphabetical sorting compares the whole entered name. Use a distinguishing middle name/initial for students who share a name. IDs are also assigned automatically. Names imported from text must occupy one line each; blank and duplicate lines are skipped. Names may also contain commas and quotation marks.



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
| Persistence | CSV loading and saving through file streams |

The program maintains both previous and next pointers in its doubly linked lists. It traverses forward when displaying history; history appears in recording order, not necessarily date order. Moreover, grouping includes the whole current class and is displayed only, not ACTUALLY saved. Group sizes differ by at most one. Random grouping uses basic `rand()`.

A session is identified by the exact date and time entered. Multiple sessions on one day are supported. Changing the entered time means that it's a different session. Deleting a student requires confirmation and deletes all their attendance history as well. Additionally, renaming preserves their ID and records.



## Saved files

`master.csv` is created after changes and contains both student rows (`S`) and attendance rows (`A`), with columns:

```
type,id,name,date,time,status
```

For the explanation of the 'S' and 'A' designations, student rows are just basically preserved students with no attendance yet. Attendance rows include the ID, name, date, time, and numerical status where CSV names are escaped so commas and quotes round trip correctly. Addtionally, when viewing in a spreadsheet, filter the type column to A to only see the attendance records.

`master.tmp` is the temporary save file. `master.bak` holds the previous successful file when it is replaced. A malformed master file stops startup to avoid overwriting it. If only the backup remains, restore it as master.csv before starting. Keep these files together and close the CSV in Excel before saving from the program. Since this is a single-user, don't run the program in two instances in the same data folder. A failed save is reported for a failsafe; use option 6 to retry. Don't exit without saving if you need to retain pending changes.



## Experimental timing

The project also includes a separate benchmarking .bat, `02_BuildBenchmarking.bat` file that uses the compiled .exe file to run an experimental, benchamarking analysis. It does not load or modify class attendance data.

Windows:

```
g++ -std=c++11 -O2 main.cpp students.cpp attendance.cpp history.cpp search.cpp grouping.cpp -o attendance.exe
.\attendance.exe --benchmark
```

This generates `benchmark_results.csv` in the same folder when run. It tests n = 100, 1,000, 10,000, and 100,000 fictional students with each size using five runs. Additionally, each search run repeats the same unsuccessful search 100 times and reports average milliseconds per search. Linear search traverses the entire linked list; binary search uses an already sorted array. Merge sort is timed separately on a reverse alphabetical input that is reset before each run. Input generation, output, array preparation, and sorting are outside the search timers.



## Complexity notes for explaining the implementation

Assuming bounded-length names, a single enqueue/dequeue is O(1); processing n queued students is O(n). Merge sort is O(n log n) time and O(n) extra space. Binary search itself is O(log n), but the interactive search first constructs and sorts its array, so that complete menu operation includes O(n log n) preparation. Linear student lookup is O(n).

Attendance duplicate checks scan r existing attendance records for each student, so preparing the queue can take O(nr), in addition to sorting. Saving also looks up each attendance record's student by ID, costing O(nr) plus writing. This intendedly designed simple implementation should not be described as O(1) for the whole attendance feature. History scanning is O(r). Unlinking a known node is O(1); finding it is not. Student deletion also scans the attendance list. Import duplicate checking can be quadratic as the roster grows. Actual name comparisons also depend on name length.



## Verification performed

Compiled using C++11 and developed within VS Code among 6 group members with warning flags. Scripted checks have covered import, duplicate rejection, invalid menu/status/date/time input, alphabetical queue order, stopping/resuming a session, exact name search, history correction, rename persistence, quoted CSV round-trips, both grouping modes, student/history deletion, restart, malformed-file protection, and end-of-input handling. The benchmark was run separately.
