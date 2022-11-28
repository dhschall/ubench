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

#include "benchmark.hh"
#include "util.hh"

SimpleStride::SimpleStride(int _array_size, int _stride)
	: Benchmark("Simple Stride"),
    A(nullptr), B(nullptr), C(nullptr),
    array_size(_array_size), stride(_stride), num_iterations(0)
{
  init();
}

SimpleStride::~SimpleStride()
{
	delete A;
	delete B;
	delete C;
}

bool SimpleStride::init()
{
  if (array_size == 0) {
		std::cerr << "Array of size 0 makes no sense!" << std::endl;
	}

	if (stride == 0) {
		std::cerr << "Stride of size 0 makes no sens!" << std::endl;
	}

	fatal_if(!isPowerOf2(array_size), "Must be power of two!\n");

	A = new int[array_size];
	B = new int[array_size];
	C = new int[array_size];
	Benchmark::init();
  return true;
}

void SimpleStride::exec()
{
	uint idx_a = 0;
	uint idx_b = 0;
	uint idx_c = 0;

	for (size_t i = 0; i < num_iterations; i++) {
		idx_a += stride;
		idx_b += 2 * stride;
		idx_c -= stride;

		/* code */
		int x = A[idx_a % array_size];
		x += B[idx_b % array_size];
		C[idx_c % array_size] = x;
	}
}