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


BranchSortedData::BranchSortedData(uint64_t array_size)
	: Benchmark("Branch: sorted data"),
    A(nullptr),
    array_size(array_size),
	num_iterations(1),
	ref_val(0), res_val(0)
{
}

BranchSortedData::~BranchSortedData()
{
	delete A;
}

bool BranchSortedData::init()
{
  	if (array_size == 0) {
		std::cerr << "Array of size 0 makes no sense!" << std::endl;
	}

	fatal_if(!isPowerOf2(array_size), "Must be power of two!\n");

	A = new uint64_t[array_size];

	// Here we initialize the memory.
    for (unsigned c = 0; c < array_size; ++c) {
		auto v = std::rand() % 256;
		ref_val += v;
		A[c] = v;
	}

	// std::sort(A, A + array_size);

	Benchmark::init();
  	return true;
}

// void BranchSortedData::exec()
// {
// 	// Call the assembly
// 	// res_val =  singlestride(A, stride, num_iterations);

// 	for (unsigned i = 0; i < LOOP; ++i)
// 	{
// 		for (unsigned c = 0; c < array_size; ++c)
// 		{
// 		if (A[c] >= 128)
// 			res_val += A[c];
// 		}
// 	}
// }

void BranchSortedData::exec()
{
	// Call the assembly
	// res_val =  singlestride(A, stride, num_iterations);

	for (unsigned i = 0; i < LOOP; ++i)
	{

		// for (unsigned c = 0; c < array_size; ++c)
		// {
		factorial(func2(10));
		// }
	}
}




int
BranchSortedData::factorial(int n) {
    if (n > 1) {
        return n * factorial(n - 1);
    } else {
        return 1;
    }
}


bool BranchSortedData::check()
{
	// Call the assembly
    std::cout << "Ref: " << ref_val << " Res: " << res_val << std::endl;
	// return (ref_val == res_val) ? true : false;
	return true;
}








// void unsorted_data_with_branch()
// {
//   std::cout << "Unsorted data with branch: ";
//   const unsigned arraySize = 32768;
//   int data[arraySize];
//   for (unsigned c = 0; c < arraySize; ++c)
//     data[c] = std::rand() % 256;
// //   clock_t start = clock();
//   long long sum = 0;
//   for (unsigned i = 0; i < LOOP; ++i)
//   {
//     for (unsigned c = 0; c < arraySize; ++c)
//     {
//       if (data[c] >= 128)
//         sum += data[c];
//     }
//   }
// //   double elapsedTime = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
// //   std::cout << elapsedTime << " seconds" << std::endl;
// }
// void sorted_data_with_bitwise()
// {
//   std::cout << "Sorted Data without branch and with a bitwise operation: ";
//   // Generate data
//   const unsigned arraySize = 32768;
//   int data[arraySize];
//   for (unsigned c = 0; c < arraySize; ++c)
//     data[c] = std::rand() % 256;
//   std::sort(data, data + arraySize);
// //   clock_t start = clock();
//   long long sum = 0;
//   for (unsigned i = 0; i < LOOP; ++i)
//   {
//     for (unsigned c = 0; c < arraySize; ++c)
//     {
//         int t = (data[c] - 128) >> 31;
//         sum += ~t & data[c];
//     }
//   }
// //   double elapsedTime = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
// //   std::cout << elapsedTime << " seconds" << std::endl;
// }
// void unsorted_data_with_bitwise()
// {
//   std::cout << "Unsorted Data without branch and with a bitwise operation: ";
//   // Generate data
//   const unsigned arraySize = 32768;
//   int data[arraySize];
//   for (unsigned c = 0; c < arraySize; ++c)
//     data[c] = std::rand() % 256;
// //   clock_t start = clock();
//   long long sum = 0;
//   for (unsigned i = 0; i < LOOP; ++i)
//   {
//     for (unsigned c = 0; c < arraySize; ++c)
//     {
//         int t = (data[c] - 128) >> 31;
//         sum += ~t & data[c];
//     }
//   }
// //   double elapsedTime = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
// //   std::cout << elapsedTime << " seconds" << std::endl;
// }
