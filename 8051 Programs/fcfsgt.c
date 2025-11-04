#include <stdio.h>

struct Process {
    int pid;    // process ID
    int at;     // arrival time
    int bt;     // burst time
    int ct;     // completion time
    int tat;    // turnaround time
    int wt;     // waiting time
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
    }

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].at > p[j].at) {
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < p[i].at)
            currentTime = p[i].at; // CPU idle until process arrives
        p[i].ct = currentTime + p[i].bt;
        currentTime = p[i].ct;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }

    printf("\n--------------------------------------------------------\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("--------------------------------------------------------\n");

    // Calculate averages
    float avgTAT = 0, avgWT = 0;
    for (int i = 0; i < n; i++) {
        avgTAT += p[i].tat;
        avgWT += p[i].wt;
    }
    avgTAT /= n;
    avgWT /= n;

    printf("Average Turnaround Time: %.2f\n", avgTAT);
    printf("Average Waiting Time: %.2f\n", avgWT);

    // Print Gantt Chart
    printf("\nGantt Chart:\n");
    printf(" ");
    for (int i = 0; i < n; i++) {
        printf("-------");
    }
    printf("\n|");
    for (int i = 0; i < n; i++) {
        printf("  P%d  |", p[i].pid);
    }
    printf("\n ");
    for (int i = 0; i < n; i++) {
        printf("-------");
    }
    printf("\n");

    // Time line
    printf("%d", (p[0].at < 0) ? 0 : p[0].at);
    for (int i = 0; i < n; i++) {
        printf("     %d", p[i].ct);
    }
    printf("\n");

    return 0;
}
