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

#include "benchmarks/base.hh"
#include "benchmarks/registry.hh"

class BinarySearch : public BaseBenchmark {
 private:
  int num_keys;
  int array_size;
  int *A;
  int *keys;
  int found_keys;


 public:
  BinarySearch(std::string name)
      : BaseBenchmark(name),
        num_keys(100),
        array_size(100),
        A(nullptr),
        found_keys(0) {}

  ~BinarySearch() {
    if (A) {
      delete A;
    }
    if (keys) {
      delete keys;
    }
  }

  bool init(YAML::Node &bm_config) override {
    std::cout << "Setup " << _name << std::endl;
    if (bm_config["num_keys"]) {
      num_keys = bm_config["loop_count"].as<int>();
    }
    if (bm_config["array_size"]) {
      array_size = bm_config["array_size"].as<int>();
    }
    A = new int[array_size];
    for (int i = 0; i < array_size; i++) {
      A[i] = std::rand() % 256;
      // printf("A[%d] = %d\n", i, A[i]);
    }
    keys = new int[num_keys];
    for (int i = 0; i < num_keys; i++) {
      keys[i] = std::rand() % 256;
    }
    return true;
  }

  void exec() override {

    for (int i = 0; i < num_keys; i++) {
      int target = keys[i];
      int result = Binary_search(A, array_size, target);
      if (result != -1) {
        found_keys++;
      }
    }
  }

  void repeat() override {
    found_keys = 0;
  }

  void report() override {
    std::cout << "Array size: " << array_size << std::endl;
    std::cout << "Number of keys: " << num_keys << std::endl;
    std::cout << "Found keys: " << found_keys << std::endl;
    std::cout << "Average found keys per run: " << (found_keys / num_keys)
              << std::endl;
  }

  int __attribute__((noinline)) Binary_search(int* x, int xsize, int target) {
    int maximum = xsize - 1;
    int minimum = 0;
    int mean;
    while (maximum > minimum) {
      mean = (maximum + minimum) / 2;
      if (x[mean] == target) {
        // std::cout << "The number you're looking for is found! \n";
        return mean;
      } else if (x[mean] > target) {
        maximum = (mean - 1);
      } else {
        minimum = (mean + 1);
      }
    }
    return -1;
  }

};

REGISTER_BENCHMARK("binary-search", BinarySearch);
