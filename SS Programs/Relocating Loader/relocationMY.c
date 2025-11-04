#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char add[6], length[10], input[10], bitmask[12], relocbit;
    int start, len, i, address, opcode, addr, actualadd;
    FILE *fp1, *fp2;

    printf("Enter the actual starting address: ");
    scanf("%d", &start);

    fp1 = fopen("relinput.dat", "r");
    fp2 = fopen("reloutput.dat", "w");

    if (fp1 == NULL || fp2 == NULL)
    {
        printf("Error opening file.\n");
        return 1; // exit if file not found
    }

    fscanf(fp1, "%s", input);

    while (strcmp(input, "E") != 0) // until End record
    {
        if (strcmp(input, "H") == 0) // Header record
        {
            fscanf(fp1, "%s", add);
            fscanf(fp1, "%s", length);
            fscanf(fp1, "%s", input); // next record (should be T)
        }

        if (strcmp(input, "T") == 0) // Text record
        {
            fscanf(fp1, "%d", &address);
            fscanf(fp1, "%s", bitmask);

            address += start; // relocate starting address
            len = strlen(bitmask);

            for (i = 0; i < len; i++)
            {
                fscanf(fp1, "%d", &opcode);
                fscanf(fp1, "%d", &addr);

                relocbit = bitmask[i];

                if (relocbit == '0')
                    actualadd = addr; // no relocation
                else
                    actualadd = addr + start; // relocation needed

                fprintf(fp2, "%d\t%d\t%d\n", address, opcode, actualadd);

                address += 3; // each instruction assumed 3 bytes
            }

            fscanf(fp1, "%s", input); // read next record
        }
    }

    fclose(fp1);
    fclose(fp2);

    printf("Relocation finished. Check reloutput.dat\n");
    return 0;
}
