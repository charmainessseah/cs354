#include <stdio.h>
#include <stdlib.h>
#define MAX 20
int main()
{
    FILE *fp;
    fp = fopen("./tests/Latin_4x4.txt", "r");
    char *fileData;
    fileData = malloc(MAX * sizeof(char));
    fread(fileData, sizeof(char), 1000, fp);
    //printf("%s", fileData);
    int index = 0;
    while (fileData[index] != '\0')
    {
        printf("%c", fileData[index++]);
    }

    int dimension = 0;
    for (int i = 0; i < MAX; i++)
    {
        if (*(fileData + i) == '\n')
        {
            break;
        }
        dimension++;
    }
    printf("dimension: %d\n", dimension);

    char **ptp;
    ptp = malloc(dimension * sizeof(char *));
    for (int i = 0; i < dimension; i++)
    {
        *(ptp + i) = malloc(dimension * sizeof(char));
    }

    int row = 0;
    int col = 0;
    printf("test\n");
    while (row < dimension)
    {
        while (col <= dimension)
        {
            printf("%c", *(fileData + (5 * row) + col));
            col++;
        }
        row++;
        col = 0;
    }

    for (int i = 0; i <= dimension; i++)
    {
        for (int j = 0; j <= dimension; j++)
        {
            int memSpacesAway = (i * dimension) + j;
            char curr = *(fileData + memSpacesAway);
            // printf("memSpacesAway: %d\n", memSpacesAway);
            // printf("curr char: %c\n", curr);
            //*(*(ptp + i) + j)
        }
    }

    free(ptp);
    return 0;
}