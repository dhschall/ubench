import sys


_K=1024
MAX_BRANCHES=8*_K



main_tmpl = """

#define MAX_BRANCHES {MAX_BRANCHES}

int __attribute__((optimize("O0"))) scramble_btb(int nbranches) {{
    int i = 0;
    int N = nbranches;

{loop_tmpl}

end:
    return i;
}}

"""

# Generate N branches that all check if i is less than `nbranches`
# After that many unique branches, the function will return
snippet = """
    if (i > N) {
        i = i - 1;
        goto end;
    } else {
        i = i + 1;
    }
"""

loop_tmpl = "".join([snippet for _ in range(MAX_BRANCHES)])


file_name = sys.argv[1] if len(sys.argv) > 1 else 'btb_stress_asm_gen.h'

with open(file_name, 'w') as f:
    f.write(main_tmpl.format(MAX_BRANCHES=MAX_BRANCHES, loop_tmpl=loop_tmpl))