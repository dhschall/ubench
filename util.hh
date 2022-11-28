/*
 * Copyright (c) 2022 EASE Group, The University of Edinburgh
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */



#ifndef __UTIL_HH__
#define __UTIL_HH__

#include <stdarg.h>
#include <time.h>
#include <iostream>

#ifdef __aarch64__
/* this also works on apple. see mach_absolute_time assembly:
https://opensource.apple.com/source/xnu/xnu-7195.50.7.100.1/libsyscall/wrappers/mach_absolute_time.s
 */
#define RDTSC_START(c)                                                         \
	do {                                                                   \
		asm volatile("mrs %0, cntvct_el0" : "=r"(c)::"memory");        \
	} while (0)
#define RDTSC_STOP(c) RDTSC_START(c)
#define RDTSC_UNIT                                                             \
	({                                                                     \
		uint64_t c;                                                    \
		asm volatile("mrs %0, cntfrq_el0" : "=r"(c));                  \
		(1000000000 / c);                                              \
	});

#elif __x86_64__
#define RDTSC_DIRTY "%rax", "%rbx", "%rcx", "%rdx"
#define RDTSC_START(cycles)                                                    \
	do {                                                                   \
		register unsigned cyc_high, cyc_low;                           \
		asm volatile("CPUID\n\t"                                       \
			     "RDTSC\n\t"                                       \
			     "mov %%edx, %0\n\t"                               \
			     "mov %%eax, %1\n\t"                               \
			     : "=r"(cyc_high), "=r"(cyc_low)::RDTSC_DIRTY);    \
		(cycles) = ((uint64_t)cyc_high << 32) | cyc_low;               \
	} while (0)

#define RDTSC_STOP(cycles)                                                     \
	do {                                                                   \
		register unsigned cyc_high, cyc_low;                           \
		asm volatile("RDTSCP\n\t"                                      \
			     "mov %%edx, %0\n\t"                               \
			     "mov %%eax, %1\n\t"                               \
			     "CPUID\n\t"                                       \
			     : "=r"(cyc_high), "=r"(cyc_low)::RDTSC_DIRTY);    \
		(cycles) = ((uint64_t)cyc_high << 32) | cyc_low;               \
	} while (0)
#define RDTSC_UNIT 1
#else
#error unknown platform
#endif




template <class T>
static constexpr bool
isPowerOf2(const T& n)
{
    // If n is non-zero, and subtracting one borrows all the way to the MSB
    // and flips all bits, then this is a power of 2.
    return n && !(n & (n - 1));
}

/**
 * Conditional fatal macro that checks the supplied condition and only causes a
 * fatal error if the condition is true and allows the programmer to specify
 * diagnostic printout.  Useful to replace if + fatal, or if + print + assert,
 * etc.
 *
 * @param cond Condition that is checked; if true -> fatal
 * @param ...  Printf-based format string with arguments, extends printout.
 *
 * \def fatal_if(...)
 *
 * @ingroup api_logger
 */
#define fatal_if(cond, ...)                                     \
    do {                                                        \
        if (cond) {                                        \
            std::cerr << # cond << " " << __VA_ARGS__;         \
        }                                                       \
    } while (0)



#endif // __UTIL_HH__