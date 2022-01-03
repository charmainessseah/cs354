#include "mem.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    Initialize_Memory_Allocator(1600);
    
    // init array of pointers
    int *arrOfPointers[5] = {0};
    int size = 0;

    for (int i = 0; i < 5; i++) {
        int randNum = rand() % 2;
        if (randNum) { // allocate
            int bytes = rand() % 1601;
            int *p = Mem_Alloc(bytes);
            if (p != NULL) {
                arrOfPointers[size++] = p;
            }
        } else { // free
            for (int j = 0; j < size; j++) {
                if (arrOfPointers[j] != NULL) {
                    Mem_Free(arrOfPointers[j]);
                    arrOfPointers[j] = NULL;
                    break;
                }
            }
        }
    }

}