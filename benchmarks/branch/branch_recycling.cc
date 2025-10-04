#include <iostream>
#include <array>

#include "benchmarks/base.hh"
#include "benchmarks/registry.hh"
#include "lfsr.h"

class BranchRecycling : public BaseBenchmark {
 private:
  int loop_count;
  int br_exec_count;
  int br_taken_count;
  Lfsr32 lfsr;

    // Pre-allocated array to hold 100 random values between 1 and 10
  std::array<int, 100> rand_vals;

 public:
  BranchRecycling(std::string name)
      : BaseBenchmark(name),
        loop_count(100),
        lfsr(0xA01)  // Initialize LFSR with a seed
  {}

  ~BranchRecycling() {}

  bool init(YAML::Node &bm_config) override {
    std::cout << "Setup " << _name << std::endl;
    if (bm_config["loop_count"]) {
      loop_count = bm_config["loop_count"].as<int>();
    }
    br_exec_count = 0;
    br_taken_count = 0;

    // Fill the array with 100 random values between 1 and 10
    lfsr.reset();
    for (auto &v : rand_vals) {
      v = static_cast<int>(lfsr.next() % 10) + 1;
    }

    return true;
  }

  void exec() override {
    for (int i = 0; i < loop_count; i++) {
      int val = rand_vals[loop_count];

      // Branch is taken if value < 5, else not taken
      if (val < 5) {
        br_taken_count++;
      }
      br_exec_count++;
    }
  }

  void repeat() override {
    br_exec_count = 0;
    br_taken_count = 0;
    lfsr.reset();

    // Refill the array with fresh random 1..10 values
    for (auto &v : rand_vals) {
      v = static_cast<int>(lfsr.next() % 10) + 1;
    }
  }

  void report() override {
    std::cout << "Loop count: " << loop_count << std::endl;
    std::cout << "Branch executed: " << br_exec_count << std::endl;
    std::cout << "Branch taken: " << br_taken_count << std::endl;
    std::cout << "Branch not taken: " << br_exec_count - br_taken_count
              << std::endl;
  }
};

REGISTER_BENCHMARK("branch-recycling", BranchRecycling);
