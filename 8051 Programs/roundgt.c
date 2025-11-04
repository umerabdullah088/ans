#include <stdio.h>
#include <stdbool.h>

struct Process {
    int pid;     // process ID
    int at;      // arrival time
    int bt;      // burst time
    int ct;      // completion time
    int tat;     // turnaround time
    int wt;      // waiting time
    int rem_bt;  // remaining burst time
    bool done;   // completion status
};

int main() {
    int n, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for Process %d: ", p[i].pid);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rem_bt = p[i].bt;
        p[i].done = false;
    }

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    int currentTime = 0, completed = 0;
    float avgTAT = 0, avgWT = 0;
    int queue[100]; // ready queue
    int front = 0, rear = 0;
    bool inQueue[n];

    for (int i = 0; i < n; i++) inQueue[i] = false;

    printf("\nScheduling Order (Gantt Chart Sequence):\n");

    // Start from first process by arrival time
    int minAT = 9999, start = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].at < minAT) {
            minAT = p[i].at;
            start = i;
        }
    }

    currentTime = p[start].at;
    queue[rear++] = start;
    inQueue[start] = true;

    while (completed < n) {
        if (front == rear) {
            currentTime++; // idle time if queue empty
            for (int i = 0; i < n; i++) {
                if (p[i].at <= currentTime && !p[i].done && !inQueue[i]) {
                    queue[rear++] = i;
                    inQueue[i] = true;
                }
            }
            continue;
        }

        int idx = queue[front++];
        printf(" P%d ", p[idx].pid);

        if (p[idx].rem_bt > tq) {
            currentTime += tq;
            p[idx].rem_bt -= tq;
        } else {
            currentTime += p[idx].rem_bt;
            p[idx].rem_bt = 0;
            p[idx].done = true;
            p[idx].ct = currentTime;
            completed++;
        }

        // Add new processes that have arrived by now
        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && !p[i].done && !inQueue[i]) {
                queue[rear++] = i;
                inQueue[i] = true;
            }
        }

        // Re-add current process if not finished
        if (!p[idx].done) {
            queue[rear++] = idx;
        } else {
            inQueue[idx] = false;
        }
    }

    printf("\n\n--------------------------------------------------------\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("--------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        avgTAT += p[i].tat;
        avgWT += p[i].wt;
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    avgTAT /= n;
    avgWT /= n;

    printf("--------------------------------------------------------\n");
    printf("Average Turnaround Time: %.2f\n", avgTAT);
    printf("Average Waiting Time: %.2f\n", avgWT);

    // Gantt Chart (simple order of completion)
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n; i++) printf("-------");
    printf("\n|");

    // Sort by CT to print in order of completion
    int tempCT[n];
    for (int i = 0; i < n; i++) tempCT[i] = p[i].ct;

    for (int k = 0; k < n; k++) {
        int minCT = 99999, idx = -1;
        for (int j = 0; j < n; j++) {
            if (tempCT[j] < minCT) {
                minCT = tempCT[j];
                idx = j;
            }
        }
        if (idx != -1) {
            printf("  P%d  |", p[idx].pid);
            tempCT[idx] = 99999;
        }
    }

    printf("\n ");
    for (int i = 0; i < n; i++) printf("-------");
    printf("\n");

    printf("0");
    int tempCT2[n];
    for (int i = 0; i < n; i++) tempCT2[i] = p[i].ct;
    for (int k = 0; k < n; k++) {
        int minCT = 99999, idx = -1;
        for (int j = 0; j < n; j++) {
            if (tempCT2[j] < minCT) {
                minCT = tempCT2[j];
                idx = j;
            }
        }
        if (idx != -1) {
            printf("     %d", tempCT2[idx]);
            tempCT2[idx] = 99999;
        }
    }
    printf("\n");

    return 0;
}
