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
 * Implementation of a benchmark interface.
 */

#pragma once

#include <iostream>
#include "utils/configs.h"
#include <list>


#define X86_64 1
#define ARM64 2
#define RISCV64 3

class BaseBenchmark
{
protected:
  /** The name of the benchmark */
  const std::string _name;

  bool initialized;

public:
  BaseBenchmark(std::string name)
      : _name(name),
        initialized(false)
  {
  }
  virtual ~BaseBenchmark() {}

  /** Setup the benchmark such as allocating an array an populating
   *  it with data */
  virtual bool init(YAML::Node &bm_config) {
    std::cout << "Setup " << _name << std::endl;
    return true;
  }

  /** Performing the actual benchmark itself */
  virtual void exec() = 0;

  /** Report results of the benchmark if needed */
  virtual void report() {}

  /** Repeat ended. Can be used to reset something to make each repeat
   * deterministic */
  virtual void repeat() {}

  std::string getName() const {
    return _name;
  }
};


BaseBenchmark* createBenchmark(const std::string& name);
void listBenchmarks();