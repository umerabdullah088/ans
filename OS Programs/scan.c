#include <stdio.h>

int main() {
    int n, i, j, head, temp, size, dir, seek = 0;
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

    // Sort the request list
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

    // Head moving towards higher cylinder numbers
    if(dir == 1) {
        for(i = pos; i < n; i++) {
            printf("%d ", arr[i]);
            seek += abs(arr[i] - head);
            head = arr[i];
        }
        // Move to end
        seek += abs((size - 1) - head);
        head = size - 1;
        // Then reverse
        for(i = pos - 1; i >= 0; i--) {
            printf("%d ", arr[i]);
            seek += abs(arr[i] - head);
            head = arr[i];
        }
    }
    // Head moving towards lower cylinder numbers
    else {
        for(i = pos - 1; i >= 0; i--) {
            printf("%d ", arr[i]);
            seek += abs(arr[i] - head);
            head = arr[i];
        }
        // Move to 0
        seek += abs(head - 0);
        head = 0;
        // Then reverse
        for(i = pos; i < n; i++) {
            printf("%d ", arr[i]);
            seek += abs(arr[i] - head);
            head = arr[i];
        }
    }

    printf("\nTotal Seek Time = %d\n", seek);
    return 0;
}
