#include <stdio.h>
#include <stdlib.h>

int *set;
int *powerset;
int size;
int target;

void print(int size)
{
    for (int i = 0; i < size; i++)
    {
        if (i > 0)
            printf(" ");
        printf("%d", powerset[i]);
    }
    printf("\n");
}

void backtrack(int idx, int sub_size, int curr_sum)
{
    if (idx == size)
    {
        if (curr_sum == target)
            print(sub_size);
        return;
    }

    backtrack(idx + 1, sub_size, curr_sum);

    powerset[sub_size] = set[idx];
    backtrack(idx + 1, sub_size + 1, curr_sum + set[idx]);
}

int main(int ac, char **av)
{
    if (ac < 2)
        return (0);

    size = ac - 2;
    target = atoi(av[1]);

    set = malloc(size * sizeof(int));
    powerset = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++)
        set[i] = atoi(av[i + 2]);

    backtrack(0, 0, 0);

    free(set);
    free(powerset);
    return (0);
}
