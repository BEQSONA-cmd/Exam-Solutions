#include <stdio.h>
#include <math.h>
#include <float.h>

int n = 0;

float x[11];
float y[11];
int used[11];
float dist[11][11];

float best = FLT_MAX;

void tsp(int cur, int cnt, float len) 
{
    if (len >= best) 
        return;
    if (cnt == n) 
    {
        len += dist[cur][0];
        if (len < best) 
            best = len;
        return;
    }
    for (int i = 1; i < n; i++) 
    {
        if (!used[i]) 
        {
            used[i] = 1;
            tsp(i, cnt + 1, len + dist[cur][i]);
            used[i] = 0;
        }
    }
}

float distance(float x, float y) 
{
    return sqrtf(x * x + y * y);
}

int main(void) 
{
    while (fscanf(stdin, "%f, %f", &x[n], &y[n]) == 2) 
        n++;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dist[i][j] = distance(x[i] - x[j], y[i] - y[j]);
    
    tsp(0, 1, 0);

    fprintf(stdout, "%.2f\n", best);

    return 0;
}

// compile:
// cc tsp.c -lm

// run:
// ./a.out < 4.txt

// expected output:
// % 4.00
