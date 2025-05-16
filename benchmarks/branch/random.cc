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

#include "benchmarks/base.hh"
#include "benchmarks/registry.hh"
#include "lfsr.h"

class RandomBranch : public BaseBenchmark {
 private:
  int loop_count;
  int br_exec_count;
  int br_taken_count;
  Lfsr32 lfsr;

 public:
  RandomBranch(std::string name) 
      : BaseBenchmark(name),
        loop_count(100),
        lfsr(0xA01)  // Initialize LFSR with a seed
  {}

  ~RandomBranch() {}

  bool init(YAML::Node &bm_config) override {
    std::cout << "Setup " << _name << std::endl;
    if (bm_config["loop_count"]) {
      loop_count = bm_config["loop_count"].as<int>();
    }
    br_exec_count = 0;
    br_taken_count = 0;
    return true;
  }

  void exec() override {
    for (int i = 0; i < loop_count; i++) {
      auto val = lfsr.next();
      if ((val >> 3) % 2 == 0) {
        // Simulate a branch taken
        br_taken_count++;
      }
      br_exec_count++;
    }
  }

  void repeat() override {
    br_exec_count = 0;
    br_taken_count = 0;
    lfsr.reset();
  }

  void report() override {
    std::cout << "Loop count: " << loop_count << std::endl;
    std::cout << "Branch executed: " << br_exec_count << std::endl;
    std::cout << "Branch taken: " << br_taken_count << std::endl;
    std::cout << "Branch not taken: " << br_exec_count - br_taken_count
              << std::endl;
  }
};


REGISTER_BENCHMARK("random-branch", RandomBranch);