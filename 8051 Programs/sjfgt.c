#include <stdio.h>
#include <stdbool.h>

struct Process {
    int pid;    // process ID
    int at;     // arrival time
    int bt;     // burst time
    int ct;     // completion time
    int tat;    // turnaround time
    int wt;     // waiting time
    bool done;  // whether process is completed
};

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for Process %d: ", p[i].pid);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].done = false;
    }

    int completed = 0, currentTime = 0;
    float avgTAT = 0, avgWT = 0;

    printf("\nScheduling Order (Gantt Chart Sequence):\n");

    // SJF scheduling logic
    while (completed < n) {
        int idx = -1;
        int minBT = 99999;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && !p[i].done) {
                if (p[i].bt < minBT) {
                    minBT = p[i].bt;
                    idx = i;
                }
                // If two have same burst time, choose one with earlier arrival
                else if (p[i].bt == minBT && p[i].at < p[idx].at) {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            currentTime++; // no process arrived yet
            continue;
        }

        // Execute the selected process
        printf(" P%d ", p[idx].pid);
        currentTime += p[idx].bt;
        p[idx].ct = currentTime;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].done = true;
        completed++;
    }

    // Print table
    printf("\n\n--------------------------------------------------------\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("--------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        avgTAT += p[i].tat;
        avgWT += p[i].wt;
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    avgTAT /= n;
    avgWT /= n;

    printf("--------------------------------------------------------\n");
    printf("Average Turnaround Time: %.2f\n", avgTAT);
    printf("Average Waiting Time: %.2f\n", avgWT);

    // Gantt Chart
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n; i++) printf("-------");
    printf("\n|");

    // Recreate Gantt Chart by order of completion
    for (int i = 0; i < n; i++) {
        int minCT = 99999, idx = -1;
        for (int j = 0; j < n; j++) {
            if (p[j].ct < minCT && p[j].ct != -1) {
                minCT = p[j].ct;
                idx = j;
            }
        }
    }

    // Reprint in the correct completion order
    // (We’ll print in order of CT ascending)
    for (int time = 1; time <= n; time++) {
        int minCT = 99999, idx = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].ct < minCT) {
                minCT = p[i].ct;
                idx = i;
            }
        }
        printf("  P%d  |", p[idx].pid);
        p[idx].ct = 99999; // mark as printed
    }

    printf("\n ");
    for (int i = 0; i < n; i++) printf("-------");
    printf("\n");

    // Reprint completion times under chart
    int minCT = 99999, idx = -1;
    int temp[n];
    for (int i = 0; i < n; i++) temp[i] = p[i].ct; // store before marking

    printf("0");
    for (int time = 1; time <= n; time++) {
        int min = 99999, pos = -1;
        for (int j = 0; j < n; j++) {
            if (temp[j] < min) {
                min = temp[j];
                pos = j;
            }
        }
        if (pos != -1) {
            printf("     %d", temp[pos]);
            temp[pos] = 99999;
        }
    }
    printf("\n");

    return 0;
}
