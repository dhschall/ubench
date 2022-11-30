.intel_syntax noprefix

.text

.global singlestride


/*
uint64_t singlestride(uint64_t* A, uint64_t stride, uint64_t num_iterations)

    For the sysv_abi calling convention
    rdi = *A,
    rsi = stride,
    rdx = num_iterations,
    rcx,
    r8,
    r9
    rax = return value
*/

singlestride:
    push    rbp
    mov     rbp, rsp
    push    r15  /* push registers that we use */
    push    r14
    mov     r15, 0    /* Iteration counter */
    mov     r14, rdi  /* Index */
    sal     rsi, 3    /* Shift index by 3 -> multiply by 8 because we have a uint64_t array */
    add     r14, rsi  /* Add stride because of big/small endian allignment */
    mov     rax, 0    /* Result */
.L3:
    add     rax, QWORD PTR [r14]   /*Perform the actual load */
    add     r14, rsi    /* Add the stride to the index */
    add     r15, 1      /* Increment the Iteration count */
    cmp     r15, rdx    /* Compare check for end of loop */
    jb      .L3
    pop     r14
    pop     r15
    pop     rbp
    ret