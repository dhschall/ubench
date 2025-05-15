import sys


if len(sys.argv) < 2:
    print('Usage: python3 scramble_btb.py <nbranches> [main/branch_btb]')
    sys.exit(1)

nbranches = int(sys.argv[1])

## Generate the main function
if len(sys.argv) > 2 and sys.argv[2] == 'main':

    print('''
void scramble_btb();

int main() {
	scramble_btb();
}

''')


## Generate the scramble_btb function
## x86_64 version
# First, the function header
print('''

#ifdef __x86_64__
''')

## Generate the surrounding BranchBTB member method
if len(sys.argv) > 2 and sys.argv[2] == 'branch_btb':

    print('''
#include "branch_test.hh"

void __attribute__((noinline)) BranchBTB::scramble_btb() {
''')
else:
    print('''
void __attribute__((noinline)) scramble_btb() {
''')


print('''
\tint c = 3;

\tasm volatile (
\t\t".align 16, 0x90\\n"
\t\t"\tnop;nop;nop;nop;\\n"
'''[1:].rstrip())


## Now generate n branch instructions
for i in range(nbranches):
    print('''
\t\t"\tmov %%0, %%%%ecx;\\n"
\t\t"\tjmp label_%d;\\n"
\t\t"\tnop;nop;nop;nop;nop;nop;nop;nop;\\n"
\t\t"label_%d: \\n"
\t\t"\tdec %%%%ecx\\n"
\t\t"\tjnz label_%d;\\n"
'''[1:].rstrip() % (i,i, i))

# Now generate the function footer
print('''
		: /*no outputs*/
		: "r"(c)
		: "%ecx" );
}
''')


## ARM version ------------------------
# First, the function header
print("""}
#else
void __attribute__((noinline)) scramble_btb(){
\tint c = 3;\n
\tasm volatile (
\t\t".align 4\\n"
"""[1:].rstrip())


## Now generate n branch instructions
for pad in range(3):
    for i in range(pad):
        print("\"nop;\\n\"");

    for i in range(nbranches):
        print('''
\t\t"\tmov w0, %%w0;\\n"
\t\t".align 1\\n"
\t\t"label_%d_%d: \\n"
\t\t"\tsubs w0,w0, #1;\\n"
\t\t"\tbne label_%d_%d;\\n"
'''[1:].rstrip() % (pad,i,pad,i))


# Now generate the function footer
print("""
\t\t: /* no out */
\t\t: "r" (c)
\t\t: "w0" );
}
#endif
""")
