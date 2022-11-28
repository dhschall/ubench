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

#ifndef __BENCHMARK_HH__
#define __BENCHMARK_HH__

#include <iostream>



class Benchmark {
  const std::string name;
  bool initialized;
  const unsigned blkSize;

  public:
  Benchmark(std::string _name)
    : name(_name),
      initialized(false),
      blkSize(64)
      {}

  /** Setup the benchmark such as allocating arrays and so on. */
  virtual bool init() {
    std::cout << "Setup " << name << std::endl;
    return true;
    }

  /** Performing the actual benchmark itself */
  virtual void exec() = 0;
};



class SimpleStride : public Benchmark
{
private:
  /* The arrays*/
  int *A, *B, *C;
  const int array_size;
  const int stride;
public:
  int num_iterations;

  SimpleStride(int _array_size, int _stride);
  ~SimpleStride();

  bool init() override;
  void exec() override;
};


#endif // __BENCHMARK_HH__