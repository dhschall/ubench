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

class RandomBranchArray : public BaseBenchmark {
 private:
  int array_size;
  int array_step;
  int br_exec_count;
  int br_taken_count;
  uint8_t *A;
  Lfsr32 lfsr;

 public:
  RandomBranchArray(std::string name) 
      : BaseBenchmark(name),
        array_size(100),
        array_step(1),
        A(nullptr),
        lfsr(0xA01)  // Initialize LFSR with a seed
  {}

  ~RandomBranchArray() {
    if (A) {
      delete A;
    }
  }

  bool init(YAML::Node &bm_config) override {
    std::cout << "Setup " << _name << std::endl;
    if (bm_config["array_size"]) {
      array_size = bm_config["array_size"].as<int>();
    }
    if (bm_config["array_step"]) {
      array_step = bm_config["array_step"].as<int>();
    }
    br_exec_count = 0;
    br_taken_count = 0;

    
    lfsr.reset();
    A = new uint8_t[array_size];
    for (int i = 0; i < array_size; i+=array_step) {
      auto val = lfsr.next();
      A[i] = uint8_t((val >> 3) % 2);
    }
    return true;
  }

  void exec() override {
    for (int i = 0; i < array_size; i+=array_step) {
      auto v = A[i];
      if (v == 0) {
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
    std::cout << "Array size: " << array_size << " step: " << array_step<< std::endl;
    std::cout << "Branch executed: " << br_exec_count << std::endl;
    std::cout << "Branch taken: " << br_taken_count << std::endl;
    std::cout << "Branch not taken: " << br_exec_count - br_taken_count
              << std::endl;
  }
};


REGISTER_BENCHMARK("random-branch-array", RandomBranchArray);