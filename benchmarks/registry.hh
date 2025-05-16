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
 * Benchmark registry
 */

#pragma once

#include <functional>
#include <map>
#include <string>
#include <iostream>

#include "base.hh"

class BenchmarkRegistry {
 public:
  using BenchmarkFactory = std::function<BaseBenchmark *()>;

  static BenchmarkRegistry &getInstance() {
    static BenchmarkRegistry instance;
    return instance;
  }

  void registerBenchmark(const std::string &name, BenchmarkFactory factory) {
    // std::cout << "Registering benchmark: " << name << std::endl;
    benchmarks[name] = factory;
  }

  BaseBenchmark *createBenchmark(const std::string &name) const {
    auto it = benchmarks.find(name);
    if (it != benchmarks.end()) {
      return it->second();
    }
    return nullptr;
  }

    std::map<std::string, BenchmarkFactory> getBenchmarks() const {
        return benchmarks;
    }

 private:
  std::map<std::string, BenchmarkFactory> benchmarks;

  // Private constructor for singleton pattern
  BenchmarkRegistry() = default;
};

#define REGISTER_BENCHMARK(name, className)             \
  static bool _##className##_registered = []() {        \
    BenchmarkRegistry::getInstance().registerBenchmark( \
        name, []() { return new className(name); });    \
    return true;                                        \
  }()

