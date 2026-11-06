#include "system.h"

void groupStudents() {
    // Checks if students are available
    if (studentCount == 0) { cout << "Add students first.\n"; return; }

    // Get the number of groups
    int groups = getNumber("Number of groups: ", 1, studentCount);
    if (!cin) return;

    // Select grouping mode
    int mode = getNumber("1 = Alphabetical round-robin, 2 = Random: ", 1, 2);
    if (!cin) return;

    // Get sorted student list
    Student** list = sortedStudents();
    if (mode == 2) {
        for (int i = studentCount - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            Student* temp = list[i];
            list[i] = list[j];
            list[j] = temp;
        }
    }

    // Display students in each group
    for (int group = 0; group < groups; group++) {
        cout << "\nGROUP " << group + 1 << '\n';    

        // Assign students using round-robin
        for (int i = group; i < studentCount; i += groups) 
            cout << list[i]->id << " - " << list[i]->name << '\n';
    }

    // Free allocated memory
    delete[] list;
}
