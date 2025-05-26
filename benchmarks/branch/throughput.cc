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

#include "benchmarks/base.hh"
#include "benchmarks/registry.hh"

class RandomThroughput : public BaseBenchmark {
 private:
  int loop_count;

 public:
 RandomThroughput(std::string name) 
      : BaseBenchmark(name),
        loop_count(100)
  {}

  ~RandomThroughput() {}

  bool init(YAML::Node &bm_config) override {
    std::cout << "Setup " << _name << std::endl;
    if (bm_config["loop_count"]) {
      loop_count = bm_config["loop_count"].as<int>();
    }
    return true;
  }


  void exec() override {
    for (int i = 0; i < loop_count; i++) {

#if defined(ARCH) && ARCH == X86_64
#elif defined(ARCH) && ARCH == ARM64
      asm volatile (
        "	bne label_2_0;\n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "label_2_0: \n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "	bne label_2_1;\n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "label_2_1: \n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "	bne label_2_2;\n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "label_2_2: \n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "	bne label_2_3;\n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "label_2_3: \n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "	bne label_2_4;\n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "label_2_4: \n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "	bne label_2_5;\n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        "label_2_5: \n"
        "	nop;nop;nop;nop;nop;nop;nop;nop;\n"
        );
#elif defined(ARCH) && ARCH == RISCV64
#else
#error "Unsupported architecture. Please define ARCH to X86_64, ARM64, or RISCV64."
#endif
    }
  }

  void report() override {
    std::cout << "Loop count: " << loop_count << std::endl;
  }
};


REGISTER_BENCHMARK("branch-throughput", RandomThroughput);