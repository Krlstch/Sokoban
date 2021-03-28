#include <stdio.h>
#include <stdlib.h>

void load(char*** array, int* row_count, int* col_count)
{
    scanf("%d %d\n", row_count, col_count);
    *array = (char**)calloc(*row_count, sizeof(char*));
    for(int i = 0; i < *row_count; i++)
    {
        (*array)[i] = (char*)calloc(*col_count, sizeof(char));
        for(int j = 0; j < *col_count; j++)
            (*array)[i][j] = getchar();
        getchar();
    }
}

int main()
{
    char** array;
    int row_count;
    int col_count;

    load(&array, &row_count, &col_count);
    for(int i = 0; i < row_count; i++)
    {
        for(int j = 0; j < col_count; j++)
            printf("%c", array[i][j]);
        printf("\n");
    }
}