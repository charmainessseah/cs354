#include "mem.h"

//------------------------ function headers ---------------------------
void Print_Block_Data(BLOCK_HEADER* header);

int Is_Free(BLOCK_HEADER* header);

void Set_Allocated(BLOCK_HEADER* header);

void Set_Free(BLOCK_HEADER* header);

void* Get_User_Pointer(BLOCK_HEADER* header);

BLOCK_HEADER* Get_Next_Header(BLOCK_HEADER* header);

int Get_Padding(BLOCK_HEADER* header);

int Set_Padding(int payload);

int Get_Block_Size(BLOCK_HEADER* header);

void Merge_After(BLOCK_HEADER *curr_header, unsigned long currAddress);

void Merge_Before(BLOCK_HEADER *curr_header, unsigned long currAddress);

void* Mem_Alloc(int size);

int Mem_Free(void *ptr);
//------------------------------- end ---------------------------------

extern BLOCK_HEADER* first_header;

void Print_Block_Data(BLOCK_HEADER* header) {
    int allocated = Is_Free(header);
    printf("allocted: %d\n", allocated);
    printf("header: 8 bytes\n");
    printf("payload: %d bytes\n", header -> payload);  
    int padding = Get_Padding(header); 
    printf("padding: %d bytes\n", padding); 
}

// returns 1 if block is free, 0 if not
// &1 grabs the value of the last bit
int Is_Free(BLOCK_HEADER* header) {
    int totalSize = header -> size_alloc;
    int allocated = totalSize & 1;
    return !allocated;
}

// sets allocated bit to 1
// | 1 sets last bit to 1 if not already
void Set_Allocated(BLOCK_HEADER* header) {
    int currentSize = header -> size_alloc;
    header -> size_alloc = currentSize | 1;
}

// sets allocated bit to 0
// currentSize & 11111110 sets last bit to 0 
// 11111110 = -2 
void Set_Free(BLOCK_HEADER* header) {
    int currentSize = header -> size_alloc;
    header -> size_alloc = currentSize & -2;
}

// returns pointer to the start of payload of the block
// headers are located 8 bytes before user pointers (pointer to payload)
void* Get_User_Pointer(BLOCK_HEADER* header) {
    unsigned long headerAddress = (unsigned long)header; // convert to do arithmetic
    headerAddress += 8; // move down 8 bytes (address of payload)
    void *p = (void *)headerAddress; 
    return p;
}

// gets the header of the next block
// if no next block, return null (current header payload = 0)
BLOCK_HEADER* Get_Next_Header(BLOCK_HEADER* header) {
    int payloadSize = header -> payload;
    if (payloadSize == 0) {
        return NULL;
    }
    int currentSize = header -> size_alloc;
    int blockSize = currentSize & -2;
    unsigned long nextHeaderAddress = (unsigned long)header + blockSize;
    BLOCK_HEADER* nextHeader = (BLOCK_HEADER *)nextHeaderAddress;
    return nextHeader;
}

// block size = header + payload + padding
int Get_Padding(BLOCK_HEADER* header) {
    int payloadSize = header -> payload;
    int blockSize = Get_Block_Size(header);
    int paddingSize = blockSize - payloadSize - 8;
    return paddingSize;
}

/*
 * returns padding such that the block size is at least 16 bytes and the block size is 
 * divisible by 16
 */
int Set_Padding(int payload) {
    int blockSize = payload + 8;
    int padding = 0;
    while((blockSize + padding) < 16 || (blockSize + padding) % 16) {
        padding++;
    }
    return padding;
}

// block size = header_size + payload_size + padding
// this method removes alloc bit to get the block size
int Get_Block_Size(BLOCK_HEADER* header) {
    int totalSize = header -> size_alloc;
    int blockSize = totalSize & -2;
    return blockSize;
}

void Merge_After(BLOCK_HEADER *curr_header, unsigned long currAddress) {
    if ((curr_header -> payload) == 0) {
        return; // we are in last block
    }
    int currBlockSize = Get_Block_Size(curr_header);
    unsigned long nextAddress = currAddress + currBlockSize;
    BLOCK_HEADER *next_header = (BLOCK_HEADER *)nextAddress;
    if (Is_Free(next_header)) {
        // update curr header
        int nextBlockSize = Get_Block_Size(next_header);
        int newSizeAlloc = currBlockSize + nextBlockSize;
        curr_header -> size_alloc = newSizeAlloc;
        curr_header -> payload = newSizeAlloc - 8;
    }
}

void Merge_Before(BLOCK_HEADER *curr_header, unsigned long currAddress) {
    // traverse from first_header until our next_header is curr_header
    // goal is to merge prev and next_header
    unsigned long firstHeaderAddress = (unsigned long)first_header;
    if (currAddress == firstHeaderAddress) {
        return; // our allocated block is the first block and its after block has been merged
    }

    // traverse to find target header
    BLOCK_HEADER *first_header_copy = first_header; 
    int firstHeaderBlockSize = Get_Block_Size(first_header);
    unsigned long nextHeaderAddress = firstHeaderAddress + firstHeaderBlockSize;
    BLOCK_HEADER *next_header = (BLOCK_HEADER *)nextHeaderAddress;
    while (nextHeaderAddress != currAddress) {
        first_header_copy = next_header; 
        firstHeaderBlockSize = Get_Block_Size(next_header);
        firstHeaderAddress = (unsigned long)first_header_copy;
        nextHeaderAddress = firstHeaderAddress + firstHeaderBlockSize;
        next_header = (BLOCK_HEADER *)nextHeaderAddress;
    }

    // our next_header now is the same as curr_header
    // and first_header is prev header of next/curr_header
    // check that first_header is free
    if (!Is_Free(first_header_copy)) {
        return; // header is allocated, no need to merge
    }

    // merge firstt_header with next/curr_header
    Merge_After(first_header_copy, (unsigned long)first_header_copy);
}

// return a pointer to the payload
// if a large enough free block isn't available, return NULL
void* Mem_Alloc(int size){
    if (size == 0) {
        return NULL;
    }
    // find a free block that's big enough
    BLOCK_HEADER* curr = first_header;
    int payloadSize = curr -> payload;
    // keep iterating while we have not found a block big enough (min 16) and block is allocated,
    //  and we haven't reached the last block 
    while ((payloadSize < size) || !Is_Free(curr) || Get_Block_Size(curr) <  16)  {
        if (payloadSize == 0) {
            return NULL; // reached the last block
        }
        BLOCK_HEADER* next = Get_Next_Header(curr); // get next block's header
        payloadSize = next -> payload; // next block's payload
        curr = next; // set curr header to next
    }
   
// allocate the block
// min block size = 16, block sizes must be multiples of 16, and pointers returned to users must be addresses divisible by 16
    int newPayloadSize = size;
    int newPadding = Set_Padding(newPayloadSize);
    int newBlockSize = newPayloadSize + 8 + newPadding;
    // calcuate next header data
    int currBlockSize = Get_Block_Size(curr);
    int nextHeaderBlockSize = currBlockSize - newBlockSize;
    // if we allocate current header and there is no more space for a new header
    if (nextHeaderBlockSize == 0) {
        // update current header
        curr -> size_alloc = newBlockSize;
        curr -> payload = newPayloadSize;
        Set_Allocated(curr);
        return Get_User_Pointer(curr);
    } else if (nextHeaderBlockSize < 0) {
        return NULL;
    }
    int nextHeaderPayload = nextHeaderBlockSize - 8; // payload does not include header 
    // update curr header
    curr -> size_alloc = newBlockSize; 
    curr -> payload = size; 
    Set_Allocated(curr); // set alloc bit to 1

    // create new header 
    unsigned long currHeaderAddress = (unsigned long)curr;
    unsigned long nextHeaderAddress = currHeaderAddress + newBlockSize;
    BLOCK_HEADER *next_header = (BLOCK_HEADER *)nextHeaderAddress;
    // set values in next header
    next_header -> size_alloc = nextHeaderBlockSize;
    next_header -> payload = nextHeaderPayload;
    
// return pointer to currHeader's payload
    return Get_User_Pointer(curr);
}

// return 0 on success
// return -1 if the input ptr was invalid
int Mem_Free(void *ptr){
    if (ptr == NULL) {
        return -1;
    }
// traverse the list and check all pointers to find the correct block 
// if you reach the end of the list without finding it return -1
    unsigned long payloadAddress = (unsigned long)ptr;
    unsigned long targetHeaderAddress = payloadAddress - 8; // move up 8 bytes to get header address
    unsigned long currAddress = (unsigned long)first_header; // convert to traverse from first header using arithmetic   
    BLOCK_HEADER *curr_header = first_header;
    // check if the next header is the last header
    BLOCK_HEADER *next_header = Get_Next_Header(curr_header);
    if (!Is_Free(curr_header) && (next_header -> payload) == 0) {
        Set_Free(curr_header);
        // reset payload to be all padding and payload
        int currBlockSize = Get_Block_Size(curr_header);
        int newPayload = currBlockSize - 8;
        curr_header -> payload = newPayload;
        return 0;
    }
    
    int currPayload = curr_header -> payload; // keep track of payload to make sure it is not the last block
    while (currAddress != targetHeaderAddress && currPayload != 0) { // if target header is not found and it is not the last block
        int currSize = Get_Block_Size(curr_header); // get block size of currHeader
        currAddress += currSize; // increment address to get next header
        curr_header = (BLOCK_HEADER *)currAddress; // set currHeader as next header
        currPayload = curr_header -> payload; // update payload to check that is not the last header
    }         
    if (currPayload == 0) {
        return -1; // target header was not found
    }
    // free the block 
    Set_Free(curr_header); 
  
// coalesce adjacent free blocks
    // curr_header - header of block we freed
    // currAddress - address of curr_header
    Merge_After(curr_header, currAddress);
    Merge_Before(curr_header, currAddress);
    return 0;
}

