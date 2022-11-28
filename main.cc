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



#include <errno.h>
#include <getopt.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef linux
#include <error.h>
#include <sys/personality.h>
#else
#error "OS not supported!"
#endif

#include "util.hh"
#include "perf.hh"
#include "benchmark.hh"



const char *optstring_from_long_options(const struct option *opt)
{
	static char optstring[256] = { 0 };
	char *osp = optstring;

	for (; opt->name != NULL; opt++) {
		if (opt->flag == 0 && opt->val > 0 && opt->val < 256) {
			*osp++ = opt->val;
			switch (opt->has_arg) {
			case optional_argument:
				*osp++ = ':';
				*osp++ = ':';
				break;
			case required_argument:
				*osp++ = ':';
				break;
			}
		}
	}
	*osp++ = '\0';

	if (osp - optstring >= (int)sizeof(optstring)) {
		abort();
	}
	return optstring;
}

int main(int argc, char **argv)
{
	/* [1] Disable ASLR */
	{
		int persona = personality(0xffffffff);
		if ((persona & ADDR_NO_RANDOMIZE) == 0) {
			int r = personality(persona | ADDR_NO_RANDOMIZE);
			if (r == -1) {
				error(-1, errno, "personality()");
			}
			// re-run yourself
			execvp(argv[0], argv);
			perror("execvp()");
			exit(-1);
		}
	}


	int cycle_count = 1000;
	int alignment = 2;
	int scramble_mode = 0;
	int cpu_number = 0;
	int stride = 64;
	int num_iterations = 10;


	/* Here is a problem. On aarch64 the timer is not precise
	 * enough to catch very short loops. Lets run the thing N
	 * times (10?) for small runs. */
	int repeats = 1;

	int use_perf = 0;
	int raw_counter = 0;

	static struct option long_options[] = {
		{ "count", required_argument, 0, 'c' },
		{ "alignment", required_argument, 0, 'a' },
		{ "cpu-pin", required_argument, 0, 'p' },
		// { "type", required_argument, 0, 't' },
		{ "stride", required_argument, 0, 's' },
		{ "num_inter", required_argument, 0, 'n' },
		{ "repeats", required_argument, 0, 'r' },
		{ NULL, 0, 0, 0 }
	};

	const char *optstring = optstring_from_long_options(long_options);

	optind = 1;
	while (1) {
		int option_index = 0;

		int arg = getopt_long(argc, argv, optstring, long_options,
				      &option_index);
		if (arg == -1) {
			break;
		}

		switch (arg) {
		default:
			error(-1, errno, "Unknown option: %s", argv[optind]);
			break;
		case 'h':
			error(-1, errno,
			      "Options: --repeat");
			break;
		case '?':
			exit(-1);
			break;

		case 'c':
			cycle_count = atoi(optarg);
			break;
		case 'n':
			num_iterations = atoi(optarg);
			break;
		case 's':
			stride = atoi(optarg);
			break;
		case 'p':
			cpu_number = atoi(optarg);
			break;
		case 'a':
			/* You're allowed to shoot yourself in a foot here. */
			alignment = atoi(optarg);
			break;
		case 'r':
			repeats = atoi(optarg);
			break;
		}
	}

	if (argv[optind]) {
		error(-1, errno, "Not sure what you mean by %s", argv[optind]);
	}

	/* [2] Setaffinity */
	{
		cpu_set_t set;
		CPU_ZERO(&set);
		CPU_SET(cpu_number, &set);
		int r = sched_setaffinity(0, sizeof(set), &set);
		if (r == -1) {
			perror("sched_setaffinity([0])");
			exit(-1);
		}
	}


	PerfEvent perf;

	perf.registerCounter("cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
    perf.registerCounter("instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
    perf.registerCounter("L1D-misses", PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_L1D|(PERF_COUNT_HW_CACHE_OP_READ<<8)|(PERF_COUNT_HW_CACHE_RESULT_MISS<<16));
    perf.registerCounter("L1D-reads", PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_L1D|(PERF_COUNT_HW_CACHE_OP_READ<<8)|(PERF_COUNT_HW_CACHE_RESULT_ACCESS<<16));
    perf.registerCounter("L1D-writes", PERF_TYPE_HW_CACHE, PERF_COUNT_HW_CACHE_L1D|(PERF_COUNT_HW_CACHE_OP_WRITE<<8)|(PERF_COUNT_HW_CACHE_RESULT_ACCESS<<16));

    // perf.registerCounter("LLC-misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES);

	perf.init();

	/** Initialize the benchmark */
	const int size = 16*1024*1024;
	SimpleStride bench(size, stride);
	bench.num_iterations = num_iterations;


	uint64_t numbers[repeats][4] = { { 0 } };
	for (uint j = 0; j < repeats; j++) {

		// Start measuring
		perf.start();

		// perform benchmark
		bench.exec();

		// Stop measuring
		perf.stop();
		perf.printCounters();

		double cycles = perf.getCounter("cycles");
		double instr = perf.getCounter("instructions");
		double misses = perf.getCounter("L1D-misses");
		double reads = perf.getCounter("L1D-reads");
		double writes = perf.getCounter("L1D-writes");

		std::cout << std::setw(static_cast<int>(15))
				  << "IPC: " <<  std::fixed << std::setprecision(2)
				  << perf.getIPC() << std::endl;

		std::cout << std::setw(static_cast<int>(15))
				  << "MPKI: " <<  std::fixed << std::setprecision(2)
				  << misses / instr * 1000 << std::endl;

		std::cout << std::setw(static_cast<int>(15))
				  << "Miss ratio: " <<  std::fixed << std::setprecision(2)
				  << misses / (reads + writes) << std::endl;




	}

	// double div = repeats * cycle_count;
	// if (!use_perf) {
	// 	for (uint j = 0; j < repeats; j++) {
	// 		double d = numbers[j][0] * 1.0 / div;
	// 		printf("%5.3f\n", d);
	// 	}
	// } else {
	// 	for (uint j = 0; j < repeats; j++) {
	// 		double cycles = numbers[j][0] * 1.0 / div;
	// 		double branches = numbers[j][1] * 1.0; // or raw
	// 		double missed_branches = numbers[j][2] * 1.0;
	// 		double instructions = numbers[j][3] * 1.0;
	// 		printf("%5.3f, %5.3f, %5.3f, %5.3f\n", cycles,
	// 		       instructions, missed_branches, branches);
	// 	}
	// }
}
