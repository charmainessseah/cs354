	.file	"find_max_template.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"The length of the array is %d\n"
	.text
	.globl	Find_Max
	.type	Find_Max, @function
Find_Max:
// prologue - write code for the prologue here
    // to show this is a valid place to jump to for security purposes
    endbr64
    // push bp onto the stack
    pushq   %rbp
    // let rbp point to bottom stack frame of Find_Max
    movq    %rsp, %rbp

// printf("The length of the array is %d\n", n);
// update this code if necessary to print the length of the array
// this block prints the value of register %edi
// initially the parameter n
   	//movl	%edi, %eax
	//movl	%eax, %esi
	//leaq	.LC0(%rip), %rdi
	//movl	$0, %eax
	//call	printf@PLT

// if length of array is 0, return immediately
    // compare length of array to 0
    cmpl    $0, %edi
    // jump to return label if it is 0
    je      Return_Invalid_Array_Length

// reserve space for local variables
/* make a table here the variables from 
 * your c code and the addresses used in assembly
 * -32(%rbp) - array base address (8 bytes)
 * -12(%rbp) - length of array
 * -8(%rbp) - max index
 * -4(%rbp) - index i
*/ 

    // reserve 32 bytes on the stack for local variables
    subq    $32, %rsp
    // move length of array from %edi onto stack
    movl    %edi, -12(%rbp)
    // move array base address onto stack
    movq    %rsi, -32(%rbp)
    // init max index
    movl    $0, -8(%rbp)
    // init index i
    movl    $1, -4(%rbp)

// this code calls Print_One_Number to 
// print the number found in register %eax - use this code to debug 
// replace $999 with a register or memory address you wish to print
    //movq  -32(%rbp), %rax
    //movl  (%rax), %eax
    //movl  %eax, %edi
    //call  Print_One_Number@PLT

// write your code to find the index of the maximum value here
    
Check_Condition:
    // move current index into %edx
    movl    -4(%rbp), %edx
    // compare current index and array length
    cmpl    -12(%rbp), %edx
    // break out of loop if (current index - array length) == 0
    je      End_Loop

Loop:
// this portion gets a[i] and stores it in %edi
    // move array base address into %rax
    movq    -32(%rbp), %rax
    // move current index into %edx
    movl    -4(%rbp), %edx
    // compute address for a[i] and put into %rbx
    leaq    (%rax, %rdx, 4), %rbx
    // get a[i] and put into %edi
    movl    (%rbx), %edi

//this portion gets a[max index] and stores it in %ecx
    // move array base address into %rax
    movq    -32(%rbp), %rax
    // get max index and put into %esi
    movl    -8(%rbp), %esi
    // compute address for a[max index] and put into %rbx
    leaq    (%rax, %rsi, 4), %rbx
    // get max value and put into %ecx
    movl    (%rbx), %ecx

// this portion computes whether or not updating max index is necessary
    // compare current value and max value
    cmpl    %edi, %ecx
    // skip Update_Max index if (max value - curr value) >= 0
    jge     Increment_Index

Update_Max:
    // move current index into %edx
    movl    -4(%rbp), %edx
    // update max index to current index
    movl    %edx, -8(%rbp)
    
Increment_Index:
    // i += 1
    addq    $1, -4(%rbp)
    // check loop condition again
    jmp     Check_Condition

End_Loop:
    // move max index into %eax to return
    movl    -8(%rbp), %eax
    jmp     Return
    
Return_Invalid_Array_Length:
    // if length of array is invalid, we return -1
    movl    $-1, %eax

Return:
// epilogue - complete the epilogue below
    leave
    ret

.LFE0:
	.size	Find_Max, .-Find_Max
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
