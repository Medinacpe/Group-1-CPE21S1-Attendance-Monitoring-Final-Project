#include "system.h"

string nameKey(string text) {
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') text[i] += 'a' - 'A';
    }
    return text;
}

void merge(Student* list[], Student* temp[], int left, int middle, int right) {
    int i = left, j = middle + 1, k = left;
    while (i <= middle && j <= right) {
        if (nameKey(list[i]->name) <= nameKey(list[j]->name)) temp[k++] = list[i++];
        else temp[k++] = list[j++];
    }
    while (i <= middle) temp[k++] = list[i++];
    while (j <= right) temp[k++] = list[j++];
    for (int p = left; p <= right; p++) list[p] = temp[p];
}

void mergeSort(Student* list[], Student* temp[], int left, int right) {
    if (left >= right) return;
    int middle = left + (right - left) / 2;
    mergeSort(list, temp, left, middle);
    mergeSort(list, temp, middle + 1, right);
    merge(list, temp, left, middle, right);
}

Student** sortedStudents() {
    Student** list = new Student*[studentCount];
    Student** temp = new Student*[studentCount];
    Student* current = head;
    for (int i = 0; i < studentCount; i++) {
        list[i] = current;
        current = current->next;
    }
    mergeSort(list, temp, 0, studentCount - 1);
    delete[] temp;
    return list;
}

int binarySearch(Student* list[], int count, string name) {
    int low = 0, high = count - 1;
    string target = nameKey(name);
    while (low <= high) {
        int middle = low + (high - low) / 2;
        string key = nameKey(list[middle]->name);
        if (key == target) return middle;
        if (key < target) low = middle + 1;
        else high = middle - 1;
    }
    return -1;
}

void searchMenu() {
    if (studentCount == 0) { cout << "No students yet.\n"; return; }
    string name = getText("Exact full name to search (case-insensitive): ");
    if (!cin) return;
    Student** list = sortedStudents();
    int index = binarySearch(list, studentCount, name);
    if (index == -1) cout << "Student not found.\n";
    else {
        cout << "Found: " << list[index]->id << " - " << list[index]->name << '\n';
        showHistory(list[index]->id);
    }
    delete[] list;
}

void runBenchmark() {
    int sizes[4] = {100, 1000, 10000, 100000};
    ofstream output("benchmark_results.csv");
    if (!output) { cout << "Cannot create benchmark output.\n"; return; }
    output << "n,linear_search_ms,binary_search_ms,merge_sort_ms,repetitions,searches_per_run\n";
    volatile int found = 0;
    for (int s = 0; s < 4; s++) {
        int n = sizes[s];
        for (int i = 1; i <= n; i++) {
            stringstream name;
            name << "Student " << setfill('0') << setw(6) << i;
            addStudent(i, name.str());
        }
        Student** list = sortedStudents();
        Student** temp = new Student*[n];
        double linearTotal = 0, binaryTotal = 0, sortTotal = 0;
        string target = "ZZZ Missing Student";
        for (int run = 0; run < 5; run++) {
            chrono::steady_clock::time_point start = chrono::steady_clock::now();
            for (int k = 0; k < 100; k++) found = (findName(target) != NULL);
            chrono::steady_clock::time_point end = chrono::steady_clock::now();
            linearTotal += chrono::duration<double, milli>(end - start).count() / 100;
            start = chrono::steady_clock::now();
            for (int k = 0; k < 100; k++) found = binarySearch(list, n, target);
            end = chrono::steady_clock::now();
            binaryTotal += chrono::duration<double, milli>(end - start).count() / 100;
            for (int i = 0; i < n / 2; i++) {
                Student* item = list[i];
                list[i] = list[n - 1 - i];
                list[n - 1 - i] = item;
            }
            start = chrono::steady_clock::now();
            mergeSort(list, temp, 0, n - 1);
            end = chrono::steady_clock::now();
            sortTotal += chrono::duration<double, milli>(end - start).count();
        }
        output << n << ',' << fixed << setprecision(9) << linearTotal / 5 << ',' << binaryTotal / 5 << ',' << sortTotal / 5 << ",5,100\n";
        cout << "Finished n = " << n << '\n';
        delete[] temp;
        delete[] list;
        clearData();
    }
    output.close();
    if (!output) cout << "Benchmark output could not be saved.\n";
    else cout << "Saved benchmark_results.csv. Check value: " << found << '\n';
}
