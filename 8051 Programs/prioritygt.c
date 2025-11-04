#include <stdio.h>
#include <stdbool.h>

struct Process {
    int pid;    // process ID
    int at;     // arrival time
    int bt;     // burst time
    int pr;     // priority (smaller number = higher priority)
    int ct;     // completion time
    int tat;    // turnaround time
    int wt;     // waiting time
    bool done;  // completion status
};

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time, Burst Time and Priority for Process %d: ", p[i].pid);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].pr);
        p[i].done = false;
    }

    int completed = 0, currentTime = 0;
    float avgTAT = 0, avgWT = 0;

    printf("\nScheduling Order (Gantt Chart Sequence):\n");

    while (completed < n) {
        int idx = -1;
        int highestPriority = 99999;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && !p[i].done) {
                if (p[i].pr < highestPriority) {
                    highestPriority = p[i].pr;
                    idx = i;
                }
                // If same priority → choose one that arrived first
                else if (p[i].pr == highestPriority && p[i].at < p[idx].at) {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            currentTime++; // no process has arrived yet
            continue;
        }

        printf(" P%d ", p[idx].pid);
        currentTime += p[idx].bt;
        p[idx].ct = currentTime;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].done = true;
        completed++;
    }

    printf("\n\n---------------------------------------------------------------\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        avgTAT += p[i].tat;
        avgWT += p[i].wt;
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].tat, p[i].wt);
    }

    avgTAT /= n;
    avgWT /= n;

    printf("---------------------------------------------------------------\n");
    printf("Average Turnaround Time: %.2f\n", avgTAT);
    printf("Average Waiting Time: %.2f\n", avgWT);

    // Gantt Chart
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n; i++) printf("-------");
    printf("\n|");

    // Print in order of completion
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
