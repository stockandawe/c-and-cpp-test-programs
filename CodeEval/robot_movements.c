#include <stdio.h>

// Global debug turn on/off
int debug = 0;

#define MAX_DIM  4
#define MIN_DIM  0

// Path Logs/Print
unsigned char path[MAX_DIM*MAX_DIM] = {};
unsigned char path_index = 0;

// Track Visits
unsigned char matrix[MAX_DIM][MAX_DIM] = { };

// Count unique paths found 
unsigned path_discovered = 0;

void dbg_print_path(void)
{
    if (debug)
    {
        int cntr = 0;
        printf("Path Length: %02d  ", path_index + 1);
        for (cntr = 0; cntr < path_index; cntr++)
        {
            printf("[%u][%u] ->", (path[cntr] & 0xF0) >> 4, path[cntr] & 0xF);
        }
        printf("[%u][%u]\n", MAX_DIM, MAX_DIM);
    }
}

void log_path(unsigned x, unsigned y)
{
    path[path_index++] = ((x & 0xF) << 4) | (y & 0xF);
}

void pop_path(void)
{
    path[path_index--] = 0;
}
    
void traverse(unsigned x, unsigned y)
{
    if ((x == (MAX_DIM - 1)) && (y == (MAX_DIM - 1)))
    {
        dbg_print_path();
        path_discovered++;
        return;
    }
    
    if (matrix[x][y] != 0)
    {
        // Revisiting - Does not count 
        return;
    }
    
    // Mark My position
    matrix[x][y] = 1;

    // Log Path
    log_path(x, y);

    // Go East 
    if (y < (MAX_DIM - 1))
    {
        traverse(x, y + 1);
    }

    // Go South
    if (x < (MAX_DIM - 1))
    {
        traverse(x + 1, y);
    }
    
    // Go West
    if (y > MIN_DIM)
    {
        traverse(x, y - 1);
    }

    // Go North
    if (x > MIN_DIM)
    {
        traverse(x - 1, y);
    }
    
    // Leaving to browse a different router - Clear My position
    matrix[x][y] = 0;

    // Pop Path
    pop_path();
}

int main()
{
    traverse(0, 0);
    printf("%u\n", path_discovered);
    return 0;
}









