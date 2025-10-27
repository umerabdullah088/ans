#include <stdio.h>

int findLFU(int freq[], int time[], int n) {
    int i, minFreq = freq[0], pos = 0;
    for(i = 1; i < n; ++i) {
        if(freq[i] < minFreq) {
            minFreq = freq[i];
            pos = i;
        } else if(freq[i] == minFreq && time[i] < time[pos]) {
            // Tie-breaker: replace the oldest page
            pos = i;
        }
    }
    return pos;
}

int main() {
    int frames[10], pages[30], freq[10], time[10];
    int totalFrames, totalPages, counter = 0, faults = 0;
    int i, j, flag1, flag2, pos;

    printf("Enter number of frames: ");
    scanf("%d", &totalFrames);

    printf("Enter number of pages: ");
    scanf("%d", &totalPages);

    printf("Enter page reference string: ");
    for(i = 0; i < totalPages; ++i)
        scanf("%d", &pages[i]);

    for(i = 0; i < totalFrames; ++i) {
        frames[i] = -1;
        freq[i] = 0;
        time[i] = 0;
    }

    printf("\nPage\tFrames\t\tFault\n");

    for(i = 0; i < totalPages; ++i) {
        flag1 = flag2 = 0;

        // Check if page already exists in frames
        for(j = 0; j < totalFrames; ++j) {
            if(frames[j] == pages[i]) {
                freq[j]++;       // Increase frequency
                counter++;
                time[j] = counter; // Update recent time
                flag1 = flag2 = 1;
                break;
            }
        }

        // If not found, load it
        if(flag1 == 0) {
            for(j = 0; j < totalFrames; ++j) {
                if(frames[j] == -1) {
                    counter++;
                    frames[j] = pages[i];
                    freq[j] = 1;
                    time[j] = counter;
                    faults++;
                    flag2 = 1;
                    break;
                }
            }
        }

        // If all frames are full, replace LFU
        if(flag2 == 0) {
            pos = findLFU(freq, time, totalFrames);
            counter++;
            frames[pos] = pages[i];
            freq[pos] = 1;     // Reset frequency for new page
            time[pos] = counter;
            faults++;
        }

        printf("%d\t", pages[i]);
        for(j = 0; j < totalFrames; ++j) {
            if(frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
        printf("\t\t%s\n", flag1 == 0 ? "Yes" : "No");
    }

    printf("\nTotal Page Faults = %d\n", faults);
    return 0;
}
