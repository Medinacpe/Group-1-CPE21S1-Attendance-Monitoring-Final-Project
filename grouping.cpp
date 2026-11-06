#include "system.h"

void groupStudents() {
    if (studentCount == 0) { cout << "Add students first.\n"; return; }
    int groups = getNumber("Number of groups: ", 1, studentCount);
    if (!cin) return;
    int mode = getNumber("1 = Alphabetical round-robin, 2 = Random: ", 1, 2);
    if (!cin) return;
    Student** list = sortedStudents();
    if (mode == 2) {
        for (int i = studentCount - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            Student* temp = list[i];
            list[i] = list[j];
            list[j] = temp;
        }
    }
    for (int group = 0; group < groups; group++) {
        cout << "\nGROUP " << group + 1 << '\n';
        for (int i = group; i < studentCount; i += groups) cout << list[i]->id << " - " << list[i]->name << '\n';
    }
    delete[] list;
}
