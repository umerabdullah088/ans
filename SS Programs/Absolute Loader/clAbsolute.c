#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char record_type;
    char progname[20];
    int start_addr, prog_len, text_addr, opcode, operand;
    FILE *fin = fopen("inputabs.dat", "r");

    if (!fin)
    {
        printf("Error: Cannot open inputabs.dat\n");
        return 1;
    }

    printf("---- ABSOLUTE LOADER ----\n\n");

    // Read header record
    fscanf(fin, " %c", &record_type);
    if (record_type == 'H')
    {
        fscanf(fin, "%s %x %x", progname, &start_addr, &prog_len);
        printf("Program Name: %s\n", progname);
        printf("Starting Address: %04X\n", start_addr);
        printf("Program Length: %04X\n\n", prog_len);
        printf("Address\tObject Code\n");
        printf("-------\t-----------\n");
    }

    // Read next record type
    while (fscanf(fin, " %c", &record_type) == 1)
    {
        // Check for End record
        if (record_type == 'E')
        {
            int exec_addr;
            if (fscanf(fin, "%x", &exec_addr) == 1)
            {
                printf("\nFirst Execution Address: %04X\n", exec_addr);
            }
            break;
        }

        // Process Text record
        if (record_type == 'T')
        {
            fscanf(fin, "%x", &text_addr);

            // Read pairs of opcode and operand
            while (1)
            {
                // Save current position
                long pos = ftell(fin);
                char next_char;

                // Peek at next non-whitespace character
                if (fscanf(fin, " %c", &next_char) == 1)
                {
                    // If it's a record type (T or E), restore position and break
                    if (next_char == 'T' || next_char == 'E')
                    {
                        fseek(fin, pos, SEEK_SET);
                        break;
                    }
                    // Otherwise, it's hex data, restore and read it
                    fseek(fin, pos, SEEK_SET);
                }

                int res = fscanf(fin, "%x%x", &opcode, &operand);
                if (res != 2) // if no more opcode/operand found
                    break;

                printf("%04X\t%02X%04X\n", text_addr, opcode, operand);
                text_addr += 3; // each instruction = 3 bytes
            }
        }
    }

    fclose(fin);
    printf("\n---- PROGRAM LOADED SUCCESSFULLY ----\n");
    return 0;
}

/*
 * Sample input file format (inputabs.dat):
 *
 * H COPY 1000 003F
 * T 1000 14 1033 48 2039 00 1036
 * T 1009 28 1030 30 1015 48 2061
 * E 1000
 *
 * Where:
 * H = Header record (Program name, Start address in hex, Length in hex)
 * T = Text record (Start address in hex, then opcode-operand pairs in hex)
 * E = End record (First execution address in hex)
 */
