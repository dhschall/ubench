#include <iostream>
#include <vector>

#include "benchmarks/base.hh"
#include "benchmarks/registry.hh"
#include "lfsr.h"

class BranchDependentRecycling : public BaseBenchmark
{
private:
  int loop_count;
  int br_exec_count;
  int br_taken_count;
  Lfsr32 lfsr;

  std::vector<int> rand_vals;

public:
  BranchDependentRecycling(std::string name)
      : BaseBenchmark(name),
        loop_count(12500),
        lfsr(0xA01) // Initialize LFSR with a seed
  {
  }

  ~BranchDependentRecycling() {}

  bool init(YAML::Node &bm_config) override
  {
    std::cout << "Setup " << _name << std::endl;
    if (bm_config["loop_count"])
    {
      loop_count = bm_config["loop_count"].as<int>();
    }
    br_exec_count = 0;
    br_taken_count = 0;

    rand_vals.resize(loop_count);
    lfsr.reset();
    for (auto &v : rand_vals)
      v = static_cast<int>(lfsr.next() % 10) + 1;

    return true;
  }

  void exec() override
  {
    for (int i = 0; i < loop_count; i++)
    {

      int val = rand_vals[i];

      // Branch is taken if value < 5, else not taken
      if (val < 5)
      {
        br_taken_count++;
       // i++; //Skip next iteration to create dependency
        if(i < loop_count){
          rand_vals[i+1] = static_cast<int>(lfsr.next() % 10) + 1;
        }

      }
      br_exec_count++;
    }
  }

  void repeat() override
  {
    br_exec_count = 0;
    br_taken_count = 0;
    lfsr.reset();

    if ((int)rand_vals.size() != loop_count)
      rand_vals.resize(loop_count);
    for (auto &v : rand_vals)
      v = static_cast<int>(lfsr.next() % 10) + 1;
  }

  void report() override
  {
    std::cout << "Loop count: " << loop_count << std::endl;
    std::cout << "Branch executed: " << br_exec_count << std::endl;
    std::cout << "Branch taken: " << br_taken_count << std::endl;
    std::cout << "Branch not taken: " << br_exec_count - br_taken_count
              << std::endl;
  }
};

REGISTER_BENCHMARK("branch-dependent-recycling", BranchDependentRecycling);
