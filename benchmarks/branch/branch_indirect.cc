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



/** Simple stride benchmark
 *
 * Description:
*/

#include "branch_test.hh"
#include "util.hh"

// // This let the compiler know about the external assembly
// extern "C" {
// 	uint64_t singlestride(uint64_t* A, uint64_t stride, uint64_t num_iterations) __attribute__((sysv_abi));
// }


BranchIndirect::BranchIndirect(uint64_t array_size)
	: Benchmark("Branch: sorted data"),
    A(nullptr),
    array_size(array_size),
	num_iterations(1),
	ref_val(0), res_val(0)
{
}

BranchIndirect::~BranchIndirect()
{
	delete A;
}

bool BranchIndirect::init()
{
  	if (array_size == 0) {
		std::cerr << "Array of size 0 makes no sense!" << std::endl;
	}

	fatal_if(!isPowerOf2(array_size), "Must be power of two!\n");

	A = new uint64_t[array_size];

	// Here we initialize the memory.
    for (unsigned c = 0; c < array_size; ++c) {
		auto v = std::rand() % 16;
		ref_val += v;
		A[c] = v;
	}

	// std::sort(A, A + array_size);

	Benchmark::init();
  	return true;
}

__attribute__((optimize("O0")))
void BranchIndirect::exec()
{
	// Call the assembly
	// res_val =  singlestride(A, stride, num_iterations);

	for (unsigned i = 0; i < LOOP; ++i)
	{
		for (unsigned c = 0; c < array_size; ++c)
		{
			switch (A[c])
			{
			case 0:
				res_val += 4;
				break;

			case 2:
				res_val += 5;
				break;

			case 5:
				res_val += 3;
				continue;
				break;
			}


			switch (A[c])
			{
			case 1:
			case 9:
				res_val += 4;
				break;

			case 6:
				res_val += 5;
				break;

			case 5:
				res_val += 3;
				break;
				res_val += 4;
				break;

			case 12:
				res_val += 5;
				continue;
				break;
			}
		}
	}
}

