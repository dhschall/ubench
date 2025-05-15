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

#pragma once

#include <yaml-cpp/yaml.h>
#include <string>
#include <iostream>

struct Config
{
   std::string benchmark_name;
   bool use_m5ops;
   bool use_perf;
   int repeats;
   YAML::Node bm_config;

    Config()
         : benchmark_name(""),
            use_m5ops(false),
            use_perf(false),
            repeats(1)
    {
    }

    void print() const 
    {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << " Base Configuration" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Benchmark:\t" << benchmark_name << std::endl;
        std::cout << "Repeats:\t" << repeats << std::endl;
        std::cout << "Use m5ops:\t" << (use_m5ops ? "true" : "false") << std::endl;
        std::cout << "Use perf:\t" << (use_perf ? "true" : "false") << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        if (bm_config.size() > 0) {
            std::cout << " Benchmark Configuration" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            // for (std::size_t i=0;i<bm_config.size();i++) {
            //     std::cout << bm_config[i].as<std::string>() << "\n";
            // }
            std::cout << YAML::Dump(bm_config) << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        }
    }
};

/**
 * @brief Parse the command line options and the config file
 * 
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @param config Configuration struct to be filled
 * @return true if parsing was successful, false otherwise
 */
bool parseConfigs(int argc, char **argv, Config &config);