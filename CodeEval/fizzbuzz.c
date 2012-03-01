#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// Global debug turn on/off
int debug = 0;

// Error Codes 
typedef enum
{
    EC_OK = 0,
    EC_ERROR,
} e_status;
    
// #define s
#define TRUE    1
#define FALSE   0

// :: Debug ::
void dbg_printf(unsigned force_debug, const char * fmt, ...)
{
    if (force_debug || debug)
    {
        va_list parms;
        va_start(parms, fmt);
        vprintf(fmt, parms);
        va_end(parms);
    }
}

// :: Parsing Functions ::
e_status line_skip_specials(char ** line, unsigned char skip_specials)
{
    if ((**line) == 0)
    {
        return EC_ERROR;
    }
    
    if (skip_specials)
    {
        // skip non-digits and non-alpha
        while (!isdigit((**line)) && !isalpha(**line))
        {
            if ((**line) == 0)
            {
                return EC_ERROR;
            }
            (*line)++;
        }
    }

    return EC_OK;
}

// Get Number : input line **, the function will advance the pointer and return value as contents of ret
e_status get_next_number(char ** line, unsigned * ret, unsigned char skip_specials)
{
    *ret = 0;
    
    if (line_skip_specials(line, skip_specials) != EC_OK)
    {
        return EC_ERROR;
    }

    // process the number 
    while (((**line) != 0) && isdigit((**line)))
    {
        (*ret) = (*ret) * 10 + ((**line) - '0');
        (*line)++;
    }

    dbg_printf(0, "NUMBER: %u\n", (*ret));
    return EC_OK;
}

e_status process_line(char * line)
{
    char * p_line = line;
    
    unsigned a = 0, b = 0, n = 0, cntr;
    
    if (get_next_number(&p_line, &a, TRUE) != EC_OK)
        return EC_ERROR;
    
    if (get_next_number(&p_line, &b, TRUE) != EC_OK)
        return EC_ERROR;
    
    if (get_next_number(&p_line, &n, TRUE) != EC_OK)
        return EC_ERROR;

    for (cntr = 1; cntr <= n; cntr++)
    {
        if ((cntr % a == 0) || (cntr % b == 0))
            printf("%s%s", (cntr % a == 0) ? "F" : "", (cntr % b == 0) ? "B " : " ");
        else 
            printf("%u ", cntr);
    }
    
    printf("\n");
    return EC_OK;
}

#define LINE_SIZE 1024
int main(int argc, char **argv)
{
    FILE *f;
    char line[LINE_SIZE], *p;

    // Open file passed as argument
    if (argc < 2 || !(f = fopen(argv[1], "r"))) {
        fprintf(stderr, "Unable to open file argument\n");
        return 1;
    }

    // Read lines from file
    while (fgets(line, LINE_SIZE, f)) {
        // You may want to remove the trailing '\n'
        if ((p = strchr(line, '\n'))) { *p = '\0'; }

        // Skip empty lines
        if (line[0] == '\0') { continue; }

        // Do something with the line
        if (process_line(line) != EC_OK) {
            printf("Invalid input: %s", line);
        }
    }

    // Paranoid check
    if (ferror(f)) {
        perror("I/O Error");
    }

    return 0;
}
