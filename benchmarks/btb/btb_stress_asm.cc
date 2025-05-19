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
#include <functional>
#include <map>
#include <string>
#include <cassert>


#include "benchmarks/base.hh"
#include "benchmarks/registry.hh"

#include "btb_stress_asm_gen.h"



class BTBStressASM : public BaseBenchmark {
 private:
  int loop_count;
  std::string num_branches_str;
  int num_branches;
  int br_executed;

  std::function<int()> scramble_func;
  std::map<std::string, std::function<int()>> scramble_func_map = {
      {"2", scramble_btb_2},
      {"8", scramble_btb_8},
      {"64", scramble_btb_64},
      {"512", scramble_btb_512},
      {"1K", scramble_btb_1K},
      {"2K", scramble_btb_2K},
      // {"4K", scramble_btb_4K},
      // {"8K", scramble_btb_8K},
      // {"16K", scramble_btb_16K},
      // {"32K", scramble_btb_32K},
      // {"64K", scramble_btb_64K},
      // {"128K", scramble_btb_128K},
      // Add more mappings as needed and expand the list in the CMakefile
      // to generate the new functions
  };

 public:
 BTBStressASM(std::string name)
      : BaseBenchmark(name),
        loop_count(1),
        num_branches_str("2"),
        num_branches(2),
        br_executed(0),
        scramble_func(nullptr)
  {

  }

  ~BTBStressASM() {}

  bool init(YAML::Node &bm_config) override {
    std::cout << "Setup " << _name << std::endl;
    if (bm_config["num_branches"]) {
      num_branches_str = bm_config["num_branches"].as<std::string>();
    }
    if (bm_config["loop_count"]) {
      loop_count = bm_config["loop_count"].as<int>();
    }

    if (num_branches_str.empty()) {
      std::cerr << "Error: num_branches is not specified." << std::endl;
      return false;
    }

    if (num_branches_str.back() == 'K') {
      num_branches = std::stoi(num_branches_str.substr(0, num_branches_str.size() - 1)) * 1024;
    } else if (num_branches_str.back() == 'M') {
      num_branches = std::stoi(num_branches_str.substr(0, num_branches_str.size() - 1)) * 1024 * 1024;
    } else {
      num_branches = std::stoi(num_branches_str);
    }

    auto it = scramble_func_map.find(num_branches_str);
    if (it != scramble_func_map.end()) {
      scramble_func = it->second;
    } else {
      std::cerr << "Error: Invalid number of branches specified." << std::endl;
      std::cerr << "Valid options are: [";
      for (const auto &pair : scramble_func_map) {
        std::cerr << pair.first << " ";
      }
      std::cerr << "]" << std::endl;
      return false;
    }
    assert(scramble_func != nullptr);
    return true;
  }

  void exec() override {
    for (int i = 0; i < loop_count; i++) {
      br_executed += scramble_func();
    }
  }

  void repeat() override {
  }

  void report() override {
    std::cout << "Executed BTB scramble function with " << num_branches
              << " unique branches" << std::endl;
    std::cout << "Loop count: " << loop_count << std::endl;
    std::cout << "Total branch executed: " << br_executed << std::endl;
  }
};


REGISTER_BENCHMARK("btb-stress-asm", BTBStressASM);