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

#include "single_stride.hh"
#include "util.hh"

// This let the compiler know about the external assembly
extern "C" {
	uint64_t singlestride(uint64_t* A, uint64_t stride, uint64_t num_iterations) __attribute__((sysv_abi));
}


SingleStride::SingleStride(uint64_t _stride)
	: Benchmark("Single Stride"),
    A(nullptr),
    array_size(0), stride(_stride),
	num_iterations(0),
	ref_val(0), res_val(0)
{
}

SingleStride::~SingleStride()
{
	delete A;
}

bool SingleStride::init()
{
	array_size = stride*num_iterations;
  	if (array_size == 0) {
		std::cerr << "Array of size 0 makes no sense!" << std::endl;
	}

	if (stride == 0) {
		std::cerr << "Stride of size 0 makes no sens!" << std::endl;
	}

	fatal_if(!isPowerOf2(array_size), "Must be power of two!\n");

	A = new uint64_t[array_size];

	// Here we initialize the memory.
    // We set the values to verify that the assembly works
    uint64_t idx = 0;
    for (int i = 0; i < num_iterations; i++) {
		idx += stride;
        A[idx] = i;
		ref_val += i;
	}

	Benchmark::init();
  return true;
}

void SingleStride::exec()
{
	// Call the assembly
	res_val =  singlestride(A, stride, num_iterations);
}

bool SingleStride::check()
{
	// Call the assembly
    std::cout << "Ref: " << ref_val << " Res: " << res_val << std::endl;
	return (ref_val == res_val) ? true : false;
}