/*

Enter the number of processes: 5
Enter the number of resource types: 3

Enter Allocation Matrix:
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2

Enter Max Matrix:
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3

Enter Available Resources: 3 3 2



*/

#include <stdio.h>

int main()
{
    int n, m;
    printf("Enter the no. of process: ");
    scanf("%d", &n);
    printf("Enter the no. of resources: ");
    scanf("%d", &m);

    int alloc[n][m], max[n][m], need[n][m];
    int avail[m], safeSeq[n], finished[n];

    ///================== get the allocation matrix
    printf("Enter the allocation matrix: \n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            scanf("%d", &alloc[i][j]);
        }
        printf("\n");
    }
    // ===================get the max matrix

    printf("Enter the maximum matrix: \n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            scanf("%d", &max[i][j]);
        }
        printf("\n");
    }

    //============== read the available matrix
    printf("Available resources: ");
    for (int i = 0; i < m; i++)
    {
        scanf("%d", &avail[i]);
    }

    // ===================calculate the need matrix

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    for (int i = 0; i < n; i++)
    {
        finished[i] = 0;
    }
    int count = 0;
    // banker s algo ====================
    while (count < n)
    {
        int found = 0;
        for (int i = 0; i < n; i++)
        {
            if (finished[i] == 0)
            {
                int flag = 0;
                for (int j = 0; j < m; j++)
                {
                    if (need[i][j] > avail[j])
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                { // can proceed
                    for (int k = 0; k < m; k++)
                        avail[k] += alloc[i][k];
                    finished[i] = 1;
                    safeSeq[count++] = i;
                    found = 1;
                }
            }
        }
        if (found == 0)
        {
            printf("not a safe state found ...");
            return 0;
        }
    }

    printf("\nSystem is in a SAFE state.\nSafe sequence: ");
    for (int i = 0; i < n; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    return 0;
}
