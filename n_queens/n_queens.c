#include <unistd.h>
#include <stdlib.h>

void putnbr(int n)
{
    char c;
    if (n > 9) putnbr(n / 10);
    c = '0' + n % 10;
    write(1, &c, 1);
}

void print(int *solution, int n)
{
    for (int i = 0; i < n; i++)
    {
        putnbr(solution[i]);
        write(1, " ", 1);
    }
    write(1, "\n", 1);
}

int is_safe(int *solution, int row, int col)
{
    for(int x = 0; x < col; x++)
    {
        int y = solution[x];
        if (y == row)
            return 0;
        if (y - x == row - col)
            return 0;
        if (y + x == row + col)
            return 0;
    }
    return 1;
}

void n_queens(int n, int col, int *solution)
{
    if (col == n)
    {
        print(solution, n);
        return;
    }

    for (int row = 0; row < n; row++)
    {
        if (is_safe(solution, row, col))
        {
            solution[col] = row;
            n_queens(n, col + 1, solution);
        }
    }
}

int main(int argc, char *argv[]) 
{
    if (argc < 2)
        return 1;

    int n = atoi(argv[1]);

    int solution[n];

    n_queens(n, 0, solution);

    return 0;
}