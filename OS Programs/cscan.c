#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, i, j, temp, head, size, dir, seek = 0;
    int arr[20];

    printf("Enter number of requests: ");
    scanf("%d", &n);

    printf("Enter request sequence: ");
    for(i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter total disk size: ");
    scanf("%d", &size);

    printf("Enter direction (1 = high, 0 = low): ");
    scanf("%d", &dir);

    // Sort the requests
    for(i = 0; i < n - 1; i++)
        for(j = i + 1; j < n; j++)
            if(arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }

    int pos = 0;
    for(i = 0; i < n; i++) {
        if(arr[i] > head) {
            pos = i;
            break;
        }
    }

    printf("\nSeek Sequence:\n");

    // Moving to higher cylinders
    if(dir == 1) {
        for(i = pos; i < n; i++) {
            printf("%d ", arr[i]);
            seek += abs(arr[i] - head);
            head = arr[i];
        }
        // Jump from end to start
        seek += (size - 1 - head);
        seek += (size - 1);
        head = 0;
        for(i = 0; i < pos; i++) {
            printf("%d ", arr[i]);
            seek += abs(arr[i] - head);
            head = arr[i];
        }
    }
    // Moving to lower cylinders
    else {
        for(i = pos - 1; i >= 0; i--) {
            printf("%d ", arr[i]);
            seek += abs(arr[i] - head);
            head = arr[i];
        }
        // Jump from start to end
        seek += head;
        seek += (size - 1);
        head = size - 1;
        for(i = n - 1; i >= pos; i--) {
            printf("%d ", arr[i]);
            seek += abs(arr[i] - head);
            head = arr[i];
        }
    }

    printf("\nTotal Seek Time = %d\n", seek);
    return 0;
}
