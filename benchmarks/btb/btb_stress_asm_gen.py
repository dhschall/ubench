import sys


_K=1024

number_of_branches_to_generate = [
    2,
    8,
    64,
    512,
    1*_K,
    2*_K,
    # 4*_K,
    # 8*_K,
    # 16*_K,
    # 32*_K,
    # 64*_K,
    # 128*_K,
]


def generate_x86_64(nbranches, func_name) -> str:
    """Generate x86_64 assembly code for branch target buffer scrambling."""
    code = []

    ## Function header
    code.append('''
int __attribute__((noinline)) scramble_btb_%s() {
    int c = 0;

    asm volatile (
        ".align 16\\n"
'''[1:] % func_name)

    ## Generate n branch instructions
    for i in range(nbranches):
        code.append('''
        "\tjmp label_{fn}_{i};\\n"
        "\tnop;nop;nop;nop;nop;nop;nop;nop;\\n"
        "label_{fn}_{i}: \\n"
        "\tinc %%ecx\\n"
'''.format(fn=func_name, i=i)[1:])

    # Finally generate the function footer
    code.append('''
        : "=r"(c)
		: "r"(c)
		: "%ecx" );
    return c;
}
''')
    return ''.join(code)



def generate_arm(nbranches, func_name) -> str:
    """Generate ARM assembly code for branch target buffer scrambling."""
    code = []

    ## Function header
    code.append('''
int __attribute__((noinline)) scramble_btb_%s() {
    int c = 0;

    asm volatile (
        ".align 16\\n"
'''[1:] % func_name)

    ## Generate n branch instructions
    for i in range(nbranches):
        code.append('''
        "\tb label_{fn}_{i};\\n"
        "\tnop;nop;nop;nop;nop;nop;nop;nop;\\n"
        "label_{fn}_{i}: \\n"
        "\tadd w0,w0, #1;\\n"
'''.format(fn=func_name, i=i)[1:])

    # Finally generate the function footer
    code.append('''
        : "=r"(c)
		: "r"(c)
		: "w0" );
    return c;
}
''')
    return ''.join(code)



def generate_riscv(nbranches, func_name) -> str:
    """Generate RISC-V assembly code for branch target buffer scrambling."""
    code = []

## Function header
    code.append('''
int __attribute__((noinline)) scramble_btb_%s() {
    int c = 0;

    asm volatile (
        ".align 16\\n"
'''[1:] % func_name)

    ## Generate n branch instructions
    for i in range(nbranches):
        code.append('''
        "\tjal label_{fn}_{i};\\n"
        "\tnop;nop;nop;nop;nop;nop;nop;nop;\\n"
        "label_{fn}_{i}: \\n"
        "\taddi t0, t0, 1;\\n"
'''.format(fn=func_name, i=i)[1:])

    # Finally generate the function footer
    code.append('''
        : "=r"(c)
		: "r"(c)
		: "t0" );
    return c;
}
''')
    return ''.join(code)


def generate_combined(nbranches) -> str:
    """Generate combined x86_64 and ARM assembly code for branch target buffer scrambling."""
    code = []
    func_name = str(nbranches)
    if nbranches >= _K:
        func_name = str(nbranches // _K) + 'K'

    ## Ifdef header
    code.append("""

#define X86_64 1
#define ARM64 2
#define RISCV64 3

""")
    code.append("#if defined(ARCH) && ARCH == X86_64\n")
    code.append(generate_x86_64(nbranches, func_name))
    code.append("#elif defined(ARCH) && ARCH == ARM64\n")
    code.append(generate_arm(nbranches, func_name))
    code.append("#elif defined(ARCH) && ARCH == RISCV64\n")
    code.append(generate_riscv(nbranches, func_name))
    code.append("#else\n")
    code.append("#error \"Unsupported architecture. Please define ARCH to X86_64, ARM64, or RISCV64.\"\n")
    code.append("#endif\n")
    return ''.join(code)


file_name = sys.argv[1] if len(sys.argv) > 1 else 'btb_stress_gen.h'

with open(file_name, 'w') as f:

    for nbranches in number_of_branches_to_generate[:]:
        f.write("\n\n")
        f.write(generate_combined(nbranches))
        f.write("\n\n")