/*
 * Copyright (c) 2025 Technical University of Munich
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



#include <iostream>
#include <string>
#include "benchmarks/base.hh"
// #include "utils/configs.h"

#include "utils/configs.h"
// #include "utils/m5lib/m5lib.h"
#include "utils/m5lib/m5ops.h"



int main(int argc, char **argv)
{
	Config cfg;
	// Parse the command line options and the config file
	if (!parseConfigs(argc, argv, cfg)) {
		std::cerr << "Error parsing command line options" << std::endl;
		return 1;
	}

	// List all available benchmarks
	if (cfg.list_benchmarks) {
		listBenchmarks();
		return 0;
	}

	cfg.print();

	// Create the benchmark
	BaseBenchmark* bench = createBenchmark(cfg.benchmark_name);
	if (!bench) {
		std::cerr << "Error creating benchmark: " << cfg.benchmark_name << std::endl;
		return 1;
	}

	// Initialize the benchmark
	if (!bench->init(cfg.bm_config)) {
		delete bench;
		std::cerr << "Error initializing benchmark: " << cfg.benchmark_name << std::endl;
		return 1;
	}

	// Run the benchmark
	for (int j = 0; j < cfg.repeats; j++) {
		std::cout << "Running iteration: " << j << std::endl;

		// Reset the benchmark
		bench->repeat();

		// Start measuring
		if (cfg.use_m5ops) {
			m5_work_begin(j, 0);
		}

		bench->exec();

		// Stop measuring
		if (cfg.use_m5ops) {
			m5_work_end(j, 0);
		}
	}

	// Print the results
	bench->report();

	delete bench;
}
