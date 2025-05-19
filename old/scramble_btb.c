

#ifdef __x86_64__


void __attribute__((noinline)) scramble_btb() {

	int c = 3;

	asm volatile (
		".align 16, 0x90\n"
		"	nop;nop;nop;nop;\n"
		"	mov %0, %%ecx;\n"
		"	jmp label_0;\n"
		"	nop;nop;nop;nop;nop;nop;nop;nop;\n"
		"label_0: \n"
		"	dec %%ecx\n"
		"	jnz label_0;\n"
		"	mov %0, %%ecx;\n"
		"	jmp label_1;\n"
		"	nop;nop;nop;nop;nop;nop;nop;nop;\n"
		"label_1: \n"
		"	dec %%ecx\n"
		"	jnz label_1;\n"
		"	mov %0, %%ecx;\n"
		"	jmp label_2;\n"
		"	nop;nop;nop;nop;nop;nop;nop;nop;\n"
		"label_2: \n"
		"	dec %%ecx\n"
		"	jnz label_2;\n"
		"	mov %0, %%ecx;\n"
		"	jmp label_3;\n"
		"	nop;nop;nop;nop;nop;nop;nop;nop;\n"
		"label_3: \n"
		"	dec %%ecx\n"
		"	jnz label_3;\n"

		: /*no outputs*/
		: "r"(c)
		: "%ecx" );
}


#else
void __attribute__((noinline)) scramble_btb(){
	int c = 3;

	asm volatile (
		".align 4\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_0_0: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_0_0;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_0_1: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_0_1;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_0_2: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_0_2;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_0_3: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_0_3;\n"
"nop;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_1_0: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_1_0;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_1_1: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_1_1;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_1_2: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_1_2;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_1_3: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_1_3;\n"
"nop;\n"
"nop;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_2_0: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_2_0;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_2_1: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_2_1;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_2_2: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_2_2;\n"
		"	mov w0, %w0;\n"
		".align 1\n"
		"label_2_3: \n"
		"	subs w0,w0, #1;\n"
		"	bne label_2_3;\n"

		: /* no out */
		: "r" (c)
		: "w0" );
}
#endif

