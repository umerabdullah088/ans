#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXSYM 1000
#define MAXLINE 256

typedef struct
{
    char label[32];
    unsigned int adder;
} Sym;

Sym symtab[MAXSYM];
int sym_count = 0;

const char *optab[] = {
    "LDA", "STA", "LDCH", "STCH", "ADD", "SUB", "MUL", "DIV",
    "COMP", "J", "JEQ", "JGT", "JLT", "JSUB", "RSUB", "TIX",
    "TD", "RD", "WD", "STL", "LDL", "LDX", "STX", "STB", "LDB"};

int optabLen = sizeof(optab) / sizeof(optab[0]);

int isComment(char *line)
{
    for (int i = 0; line[i]; i++)
    {
        if (line[0] == ' ' || line[0] == '\t')
            continue;
        return (line[0] == '.');
    }
    return 0;
}

void to_upcase(char *s)
{
    for (; *s; s++)
    {
        *s = toupper((unsigned char)*s);
    }
}
void trim_string(char *string)
{
    int n = strlen(string);
    if (n && string[n - 1] == '\n')
    {
        string[n - 1] = '\0';
    }
}
int isInOptab(char *string)
{
    for (int i = 0; i < optabLen; i++)
    {
        if (strcmp(optab[i], string) == 0)
            return 1;
    }
    return 0;
}

int add_symbol(const char *label, unsigned int addr)
{
    for (int i = 0; i < sym_count; i++)
        if (strcmp(symtab[i].label, label) == 0)
            return 0;
    strncpy(symtab[sym_count].label, label, sizeof(symtab[sym_count].label) - 1);
    symtab[sym_count].adder = addr;
    sym_count++;
    return 1;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("less no of cmdline arguements");
        return 0;
    }

    FILE *fin = fopen(argv[1], "r");
    if (!fin)
    {
        perror("error occured in reading the input.asm");
        return 0;
    }
    FILE *fint = fopen("intermediate.txt", "w");
    FILE *fsym = fopen("symtab.txt", "w");
    if (!fint || !fsym)
    {
        perror("mid way file occur there ");
        return 0;
    }

    int start_add = 0;
    int locctr = 0;
    char opcode[64], operand[128], line[MAXLINE], label[64];

    int first_line = 1;

    if (fgets(line, sizeof(line), fin) == NULL)
    {
        printf("empty input file there ");
        return 1;
    }

    trim_string(line);

    if (isComment(line))
    {
        fprintf(fint, "\t%s\t\n", line);
    }
    else
    {
        if (line[0] == ' ' || line[0] == '\t')
        {
            label[0] = 0;
            int n = sscanf(line, "%s %s", opcode, operand);

            if (n < 1)
            {
                printf("bad line over there less than 1 ");
                return 1;
            }
        }
        else
        {
            int n = sscanf(line, "%s %s %s", label, opcode, operand);
            if (n < 2)
            {
                printf("bad line less than 2");
                return 1;
            }
        }

        to_upcase(opcode);

        if (strcmp(opcode, "START") == 0)
        {
            start_add = (unsigned int)strtol(operand, NULL, 16);
            locctr = start_add;

            fprintf(fint, "%04X\t%s\n", locctr, line);
        }
        else
        {
            locctr = 0;
            fseek(fin, 0, SEEK_SET);
        }
    }

    while (fgets(line, sizeof(line), fin))
    {

        trim_string(line);
        if (isComment(line))
        {
            fprintf(fint, "\t%s\t", line);
            continue;
        }

        label[0] = operand[0] = opcode[0] = 0;
        if (line[0] == ' ' || line[0] == '\t')
        {
            int n = sscanf(line, "%s %s", opcode, operand);

            if (n < 1)
                printf("bad line over there less than 1 ");
        }
        else
        {
            int n = sscanf(line, "%s %s %s", label, opcode, operand);
            if (n < 2)
            {

                printf("bad line less than 2");
            }
        }

        to_upcase(opcode), to_upcase(label);
        if (label[0] != '\0')
        {
            if (!add_symbol(label, locctr))
                perror("cant add symbol");
        }

        if (isInOptab(opcode))
        {
            fprintf(fint, "%04X\t%s\n", locctr, line);
            locctr += 3;
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            fprintf(fint, "%04X\t%s\n", locctr, line);
            locctr += 3;
        }
        else if (strcmp(opcode, "RESW") == 0)
        {
            fprintf(fint, "%04X\t%s\n", locctr, line);
            locctr += 3 * atoi(operand);
        }
        else if (strcmp(opcode, "RESB") == 0)
        {
            fprintf(fint, "%04X\t%s\n", locctr, line);
            locctr += atoi(operand);
        }
        else if (strcmp(opcode, "BYTE") == 0)
        {
            int n;
            int bytes = 0;
            char *p = strchr(operand, '\'');
            if (p)
            {
                char *q = strrchr(operand, '\'');
                if (q && q > p)
                    n = q - p - 1;
                if (operand[0] == 'C')
                {
                    bytes = n;
                }
                else if (operand[0] == 'X')
                {
                    bytes = (n + 1) / 2;
                }
                else
                {
                    bytes = n;
                }
            }
            fprintf(fint, "%04X\t%s\n", locctr, line);
            locctr += bytes;
        }
        else if (strcmp(opcode, "END") == 0)
        {
            fprintf(fint, "%04X\t%s\n", locctr, line);
            break;
        }
        else if (strcmp(opcode, "START") == 0)
        {
            fprintf(fint, "%04X\t%s\n", locctr, line);
        }
        else
        {
            /* Unknown opcode: treat as error but still write */
            fprintf(fint, "%04X\t%s\n", locctr, line);
            fprintf(stderr, "Warning: Unknown opcode/directive '%s' (treated as 3 bytes).\n", opcode);
            locctr += 3; /* conservative assumption */
        }
    }

    unsigned int program_length = locctr - start_add;

    /* write symtab */
    for (int i = 0; i < sym_count; i++)
    {
        fprintf(fsym, "%-10s %04X\n", symtab[i].label, symtab[i].adder);
    }

    printf("PASS 1 complete.\n");
    printf("Intermediate file: intermediate.txt\n");
    printf("Symbol table: symtab.txt\n");
    printf("Program length: %04X (hex) = %u (dec)\n", program_length, program_length);

    fclose(fin);
    fclose(fint);
    fclose(fsym);
    return 0;
}
