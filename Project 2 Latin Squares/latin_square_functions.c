/******************************************************************************
 * @file: latin_square_functions.c
 *
 * WISC NETID: sseah
 * CANVAS USERNAME: charmaine
 * WISC ID NUMBER: 9080857817
 * OTHER COMMENTS FOR THE GRADER (OPTIONAL)
 *
 * @creator: sseah@wisc.edu
 * @modified: SUBMISSION DATE
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "latin_square_functions.h"

// Set this to 1 to enable dbgprintf statements, make sure to set it back to 0
// before submitting!
#define DEBUG 0
#define dbgprintf(...)       \
    if (DEBUG)               \
    {                        \
        printf(__VA_ARGS__); \
    }
#define dbgprint_latin_square(n, LS) \
    if (DEBUG)                       \
    {                                \
        Print_Latin_Square(n, LS);   \
    }

/******************************************************************************
 * Data Types and Structures
 *****************************************************************************/

// ADD ANY ADDITIONAL DATA TYPES OR STRUCTSvHERE

/******************************************************************************
 * Globals
 *****************************************************************************/
extern void Print_Latin_Square(const size_t n, char **latin_square);

typedef struct Character
{
    char character;
    int count;
} CHAR;

/******************************************************************************
 * Helper functions
 *****************************************************************************/

/******************************************************************************
 * Verificaiton functions
 *****************************************************************************/

/* 
 * This function takes the name of the file containing the latin square
 * and reads in the data to the the latin_square parameter.  
 *
 * There are many approaches that will work to read in the latin square data.
 * In any approach you choose, you will need to do at least the following:
 *     1) open the file 
 *     2) read in the text from the file
 *     3) figure out the dimensions of the latin square (n)
 *     4) reserve memory for the latin_square. This requires 2 steps
 *         4a) reserve an array of pointers to the rows
 *         4b) reserve an array of characters for each row
 *     5) fill in the latin_square data structure 
 *     6) close the file
 *
 * @param filename The name of the file to read in
 * @param latin_square_in A pointer to the latin square variable in main
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 */
void Read_Latin_Square_File(const char *filename,
                            char ***latin_square_in,
                            size_t *n)
{
    // open file
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return;
    }
    // read in text from the file
    // fileData is temp storage for our file data
    // max dimension : 1000 x 1000
    // max bytes needed in malloc: (1000 x 1000) + 1000 (for new line character after each row)
    char *fileData;
    int maxElements = (1000 * 1000) + 1000;
    int maxBytes = maxElements * sizeof(char);
    fileData = malloc(maxBytes);
    fread(fileData, sizeof(char), maxBytes, fp);

    // debugging
    // int index = 0;
    // printf("test print\n");
    // while (fileData[index] != '\0')
    // {
    //     printf("%c", fileData[index++]);
    // }
    ///////////////////////////////

    // find dimension of square
    int dimension = 0;
    for (int i = 0; i < maxElements; i++)
    {
        if (*(fileData + i) == '\n')
        {
            break;
        }
        dimension++;
    }
    // set dimension
    *n = dimension;
    //printf("dim: %d\n", dimension); // debugging

    // init pointer to pointer array and reserve memory for it
    // reserve memory for 3 rows (pointers)
    *latin_square_in = malloc(dimension * sizeof(char *));
    for (int i = 0; i < dimension; i++)
    {
        // reserve memory for each col (# of chars per col)
        *(*latin_square_in + i) = malloc(dimension * sizeof(char));
    }
    // fill pointer to pointer array with values read into fileData
    int fileDataLength = dimension * dimension + dimension;
    int row = 0;
    int col = 0;
    for (int i = 0; i < fileDataLength; i++)
    {
        char curr = *(fileData + i);
        if (curr == '\0')
        {
            break;
        }
        else if (curr == '\n')
        {
            row++;
            col = 0;
            continue;
        }
        //printf("curr: %c\n", curr); //debugging
        *(*(*latin_square_in + row) + col) = curr;
        col++;
    }
    fclose(fp);
    free(fileData);
}

/* 
 * This function checks to see that exactly n symbols are used and that 
 * each symbol is used exactly n times.
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
int Verify_Alphabet(const size_t n, char **latin_square)
{
    if (latin_square == NULL)
    {
        printf("Verify_Alphabet - latin_square is NULL\n");
        return 0;
    }
    /* BEGIN MODIFYING CODE HERE */

    // create an array where arr[0] is ascii 33 and arr[93] is ascii 126
    int rangeChars = 126 - 33 + 1;
    int *pChars = malloc(rangeChars * sizeof(int));
    // init count of each ascii character to 0
    for (int i = 0; i < rangeChars; i++)
    {
        *(pChars + i) = 0;
    }

    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            int curr = *(*(latin_square + row) + col);
            int indexOfArray = curr - 33;  // index representing curr character in pChars array
            *(pChars + indexOfArray) += 1; // increment count of character
        }
    }
    // check that only n elements have its count > 0 and all have count of n
    int elementsWithNCounts = 0;
    for (int i = 0; i < rangeChars; i++)
    {
        int count = *(pChars + i);
        if (count > 0 && count == n)
        {
            elementsWithNCounts++;
        }
        else if (count != 0)
        {
            free(pChars);
            return 0; // a character does not have n counts
        }
    }
    free(pChars);
    //printf("elements with n counts: %d\n", elementsWithNCounts); // debugging
    if (elementsWithNCounts != n)
    {
        return 0;
    }

    /* END MODIFYING CODE HERE */
    return 1;
}

/* 
 * This function verifies that no symbol is used twice in a row or column.
 * It prints an error message alerting the user which rows or columns have 
 * duplicate symbols. This means that you will have at most n row error prints
 * and n column error prints.
 * 
 * Note: Test all rows first then test all columns.
 * 
 * Error messages have been included for you. Do not change the format out the 
 * print statements, as our grading scripts will use exact string matching. You
 * change the variable passed as the formatting argument to printf.
 * 
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
int Verify_Rows_and_Columns(const size_t n, char **latin_square)
{
    if (latin_square == NULL)
    {
        printf("Verify_Rows_and_Columns - latin_square is NULL\n");
        return 0;
    }
    /* BEGIN MODIFYING CODE HERE */
    int boolDuplicateFlag = 0;
    // check for duplicate in rows
    for (int row = 0; row < n; row++)
    {
        // create an array where arr[0] is ascii 33 and arr[93] is ascii 126
        int rangeChars = 126 - 33 + 1;
        int *pChars = malloc(rangeChars * sizeof(int));
        // init count of each ascii character to 0
        for (int i = 0; i < rangeChars; i++)
        {
            *(pChars + i) = 0;
        }

        for (int col = 0; col < n; col++)
        {
            int curr = *(*(latin_square + row) + col);
            int indexOfArray = curr - 33; // index representing curr character in pChars array
            if (*(pChars + indexOfArray) == 1)
            {
                boolDuplicateFlag = 1;
                printf("Error in row %d\n", row);
                break;
            }
            else
            {
                *(pChars + indexOfArray) += 1; // increment count of character
            }
        }
        free(pChars);
    }

    // check for duplicates in columns
    for (int col = 0; col < n; col++)
    {
        // create an array where arr[0] is ascii 33 and arr[93] is ascii 126
        int rangeChars = 126 - 33 + 1;
        int *pChars2 = malloc(rangeChars * sizeof(int));
        // init count of each ascii character to 0
        for (int i = 0; i < rangeChars; i++)
        {
            *(pChars2 + i) = 0;
        }

        for (int row = 0; row < n; row++)
        {
            int curr = *(*(latin_square + row) + col);
            int indexOfArray = curr - 33; // index representing curr character in pChars array
            if (*(pChars2 + indexOfArray) == 1)
            {
                boolDuplicateFlag = 1;
                printf("Error in column %d\n", col);
                break;
            }
            else
            {
                *(pChars2 + indexOfArray) += 1; // increment count of character
            }
        }
        free(pChars2);
    }
    if (boolDuplicateFlag)
    {
        return 0;
    }
    // printf("Error in row %d\n", i);
    // printf("Error in column %d\n", i);
    /* END MODIFYING CODE HERE */
    return 1;
}

/* 
 * This function calls free to allow all memory used by the latin_square 
 * verification program to be reclaimed.
 *
 * Note: you will have n+1 calls to free
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 */
void Free_Memory(const size_t n, char **latin_square)
{
    /* BEGIN MODIFYING CODE HERE */
    for (int row = 0; row < n; row++)
    {
        free(*(latin_square + row));
    }
    // free pointer to pointer
    free(latin_square);
    /* END MODIFYING CODE HERE */
}
