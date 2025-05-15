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

#ifndef __BRANCH_SORTED_DATA_HH__
#define __BRANCH_SORTED_DATA_HH__

#include "benchmark.hh"


class BranchSortedData : public Benchmark
{
private:
  /* The arrays*/
  uint64_t *A;
  uint64_t array_size;
  uint64_t ref_val;
  uint64_t res_val;


public:
  uint64_t num_iterations;

  BranchSortedData(uint64_t ARRAYSIZE);
  ~BranchSortedData();

  bool init() override;
  void exec() override;
  bool check();
};




class BranchIndirect : public Benchmark
{
private:
  /* The arrays*/
  uint64_t *A;
  uint64_t array_size;
  uint64_t ref_val;
  uint64_t res_val;



public:
  uint64_t num_iterations;

  BranchIndirect(uint64_t ARRAYSIZE);
  ~BranchIndirect();

  bool init() override;
  void exec() override;
  bool check();
};




class BranchReturn : public Benchmark
{
private:
  /* The arrays*/
  uint64_t *A;
  uint64_t array_size;
  uint64_t ref_val;
  uint64_t res_val;


  __attribute__((optimize("O0")))
  int func1(int n) {
    return n;
  }

  __attribute__((optimize("O0")))
  int func2(int n) {
    return func1(n);
  }

int factorial(int);

public:
  uint64_t num_iterations;

  BranchReturn(uint64_t ARRAYSIZE);
  ~BranchReturn();

  bool init() override;
  void exec() override;
  bool check();
};






class BranchBTB : public Benchmark
{
private:
  /* The arrays*/

  void scramble_btb();


  struct blob {
    int memfd;
    void *ptr;
    int hugepage;

    void *ptr_fn;
    void *ptr_ret;
    void *ptr_shadow;
  };

  struct blob blob;

  void blob_alloc(struct blob *blob);
  void blob_fill_code(struct blob *blob, int alignment, int cycle_count,
          char *type);
  void blob_exec(struct blob *blob);
  void blob_warm_itlb(struct blob *blob);
  void blob_warm_icache(struct blob *blob, int alignment, char *type);



public:
  uint64_t num_iterations;

  BranchBTB();
  ~BranchBTB();

  bool init() override;
  void exec() override;
  bool check();
};




#endif // __BRANCH_SORTED_DATA_HH__




#include <algorithm>
#include <ctime>
#include <iostream>
#define LOOP 1

#define ARRAYSIZE 1024*2

