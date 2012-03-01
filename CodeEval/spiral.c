#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

// Global debug turn on/off
int debug = 0;

// Error Codes 
typedef enum
{
    EC_OK = 0,
    EC_ERROR,
    EC_ERROR_NO_MEMORY
} e_status;
    
typedef struct 
{
    unsigned char value[11];
} t_input_store;

// #define s
#define TRUE    1
#define FALSE   0

#define DIRECTION_EAST   0
#define DIRECTION_SOUTH  1
#define DIRECTION_WEST   2
#define DIRECTION_NORTH  3

#define MEMBER(arr, row, col, max_col)  arr[(row)*(max_col)+(col)].value

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

// Get String : input line **, the function will advance the pointer and return value in the buffer
e_status get_next_string(char ** line, char * buf, unsigned buf_len, unsigned char skip_specials)
{
    int used = 0;

    if (!buf_len)
    {
        return EC_ERROR;
    }
    
    if (line_skip_specials(line, skip_specials) != EC_OK)
    {
        return EC_ERROR;
    }

    // process the number 
    while (buf_len > 1 && ((**line) != 0) && (isalpha((**line)) || isdigit((**line))))
    {
        *buf = (**line);
        (*line)++;
        buf++;
        buf_len--;
        used++;
    }

    *buf = 0;
    dbg_printf(0, "STRING: %s\n", buf);
    return used ? EC_OK : EC_ERROR;
}

// Print spiral
void print_spiral(t_input_store * arr, unsigned max_row, unsigned max_col)
{
    int start_row = 0, start_col = 0, end_row = max_row - 1, end_col = max_col - 1, direction = 0;
    int num_elements = 0;
    int cntr;

    do 
    {
        num_elements = 0;
        
        if (direction == DIRECTION_EAST)
        {
            // Go east and remove topmost remaining row
            for (cntr = start_col; cntr <= end_col; cntr++)
            {
                printf("%s ", MEMBER(arr, start_row, cntr, max_col));
                num_elements++;
            }
            start_row++;
            direction = DIRECTION_SOUTH;
        }
        else if (direction == DIRECTION_SOUTH)
        {
            // Go south and remove rightmost remaining col
            for (cntr = start_row; cntr <= end_row; cntr++)
            {
                printf("%s ", MEMBER(arr, cntr, end_col, max_col));
                num_elements++;
            }
            end_col--;
            direction = DIRECTION_WEST;
        }
        else if (direction == DIRECTION_WEST)
        {
            // Go west and remove bottommost row
            for (cntr = end_col; cntr >= start_col; cntr--)
            {
                printf("%s ", MEMBER(arr, end_row, cntr, max_col));
                num_elements++;
            }
            end_row--;
            direction = DIRECTION_NORTH;
        }
        else if (direction == DIRECTION_NORTH)
        {
            // Go north and remove leftmost col
            for (cntr = end_row; cntr >= start_row; cntr--)
            {
                printf("%s ", MEMBER(arr, cntr, start_col, max_col));
                num_elements++;
            }
            start_col++;
            direction = DIRECTION_EAST;
        }
    }
    while (num_elements > 0);
    
    printf("\n");
}

e_status process_line(char * line)
{
    unsigned max_row = 0, max_col = 0;
    t_input_store * arr_p = 0;
    unsigned row, col;
    char * line_p = line;
    
    get_next_number(&line_p, &max_row, TRUE);
    get_next_number(&line_p, &max_col, TRUE);
    
    dbg_printf(0, "Rows/cols : %u/%u\n", max_row, max_col);

    arr_p = (t_input_store *) malloc (sizeof(t_input_store) * max_row * max_col);
    if (!arr_p)
    {
        printf("No memory\n");
        return EC_ERROR_NO_MEMORY;
    }

    // Get the array filled up
    for (row = 0; row < max_row; row++)
    {
        for (col = 0; col < max_col; col++)
        {
            if (get_next_string(&line_p, MEMBER(arr_p, row, col, max_col), sizeof(t_input_store), TRUE) != EC_OK)
                return EC_ERROR;
        }
    }

    // Print Spiral
    print_spiral(arr_p, max_row, max_col);

    // Free array 
    free(arr_p);
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
        process_line(line);
    }

    // Paranoid check
    if (ferror(f)) {
        perror("I/O Error");
    }

    return 0;
}
