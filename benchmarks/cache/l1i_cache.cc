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

/**
 * @file
 * Simple loop benchmark as a template for other benchmarks.
 * This benchmark is a simple loop that runs for a specified number of
 * iterations.
 */

#include <iostream>
#include <algorithm>
#include <cassert>

#include "benchmarks/base.hh"
#include "benchmarks/registry.hh"


#define L1I_CACHE_LINE_SIZE (64)
#define L1I_CACHE_SIZE (16 * 1024)
#define L1I_CACHE_ASSOCIATIVITY (4)

#define L1D_CACHE_LINE_SIZE (64)
#define L1D_CACHE_SIZE (64 * 1024)
#define L1D_CACHE_ASSOCIATIVITY (4)

#define L2D_CACHE_LINE_SIZE (64)
#define L2D_CACHE_SIZE (2048 * 1024)
#define L2D_CACHE_ASSOCIATIVITY (8)

#define STORE_BUFFER_SIZE (120)

#define L1D_TLB_SIZE (64)

#define PAGE_SIZE (4096)


#define UNUSED(x) (void)x

#define FUNC(f) \
  static __attribute__((aligned(L1I_CACHE_SIZE))) void f(void* fp) { \
    void (**funcPtr)(void*) = reinterpret_cast<void (**)(void*)>(fp); \
    void(*func)(void*) = *(funcPtr++); \
    (*func)(funcPtr); \
  }

FUNC(fA)
FUNC(fB)
FUNC(fC)
FUNC(fD)
FUNC(fE)
FUNC(fF)
FUNC(fG)
FUNC(fH)
FUNC(fI)
FUNC(fJ)
FUNC(fK)
FUNC(fL)

static __attribute__((aligned(L1I_CACHE_SIZE))) void fZ(void* fp) {
    UNUSED(fp);
}


static void (*funcs[])(void*) = {
  &fA, &fB, &fC, &fD, &fE, &fF, &fG, &fH, &fI, &fJ, &fK, &fL,
  &fZ
};

static void assertFuncsArePageAligned(void) {
  void(**funcPtr)(void*) = funcs;
  uintptr_t pageMask = (uintptr_t)(L1I_CACHE_SIZE - 1);
  while(*funcPtr) {
    uintptr_t diff = (uintptr_t)(*funcPtr) & pageMask;
    UNUSED(diff);
    assert(diff == 0);
    funcPtr++;
  }
}

static void assertFuncsAreDistinct(void) {
  void(**funcPtr1)(void*) = funcs;
  while(*funcPtr1) {
    void(**funcPtr2)(void*) = funcPtr1 + 1;
    while(*funcPtr2) {
      assert(*funcPtr1 != *funcPtr2);
      funcPtr2++;
    }
    funcPtr1++;
  }
}



class L1ICache : public BaseBenchmark {
  private:
  int loop_count;
  int sum;

 public:
 L1ICache(std::string name)
  : BaseBenchmark(name),
  	loop_count(1000000),
	  sum(0)
	{}

  ~L1ICache() {}

  bool init(YAML::Node &bm_config) override {
    std::cout << "Setup " << _name << std::endl;
    if (bm_config["loop_count"]) {
      loop_count = bm_config["loop_count"].as<int>();
    }
    sum = 0;

    assertFuncsArePageAligned();
    assertFuncsAreDistinct();
    return true;
  }

  void exec() override {
    for (int i = 0; i < loop_count; i++) {

      (*funcs[0])((void*)funcs);

      sum++;
    }
  }

  void report() override {
    std::cout << "Loop count: " << loop_count << std::endl;
    std::cout << "Sum: " << sum << std::endl;
  }

};

REGISTER_BENCHMARK("cache-l1i", L1ICache);
