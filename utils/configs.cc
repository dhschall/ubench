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

#include "configs.h"
#include <iostream>
#include <string>
#include <map>
// #include <boost/program_options.hpp>

// namespace po = boost::program_options;

/**
 * @brief Parse the command line options and the config file
 * 
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @param config Configuration struct to be filled
 * @return true if parsing was successful, false otherwise
 */
bool parseConfigs(int argc, char **argv, Config &config)
{

    std::map<std::string, std::string> options;
    std::string config_file;

    // Parse command line arguments into a map
    for (int i = 1; i < argc; i++) {
        std::string key = argv[i];
        if (key[0] == '-' && i + 1 < argc && argv[i + 1][0] != '-') {
            options[key] = argv[i + 1];
            i++;
        } else if (key[0] == '-') {
            options[key] = ""; // Flags without values
        }
    }

    // Handle options
    if (options.count("--help") || options.count("-h")) {
        std::cout << "Usage: program [options]\n"
                  << "Options:\n"
                  << "  -h, --help           Produce help message\n"
                  << "  -b, --bmname         Define the benchmark to run\n"
                  << "  -c, --config         Specify a YAML config file\n"
                  << "  -r, --repeats        Number of times the benchmark should be repeated\n"
                  << "  -m, --m5ops          Enable m5ops to reset and dump stats between repeats\n"
                  << "  -z, --perf           Enable perf"
                  << "  -l, --list           List all available benchmarks\n"
                  << "\n";
        return false;
    }

    if (options.count("-b") || options.count("--bmname")) {
        config.benchmark_name = options.count("-b") ? options["-b"] : options["--bmname"];
    }

    if (options.count("-c") || options.count("--config")) {
        config_file = options.count("-c") ? options["-c"] : options["--config"];
    }

    if (options.count("-r") || options.count("--repeats")) {
        config.repeats = std::stoi(options.count("-r") ? options["-r"] : options["--repeats"]);
    }

    if (options.count("-m") || options.count("--m5ops")) {
        config.use_m5ops = true;
    }

    if (options.count("-z") || options.count("--perf")) {
        config.use_perf = true;
    }

    if (options.count("-l") || options.count("--list")) {
        config.list_benchmarks = true;
        return true;
    }


    // Check if the benchmark name is set 
    if ((config_file == "") && (config.benchmark_name == "")) {
        std::cerr << "Either a config file or benchmark name must be specified" << std::endl;
        std::cerr << "Use --help for more information" << std::endl;
        return false;
    }


    // If a config file is specified, parse it
    

    // Load the configuration of the benchmark
    if (!config_file.empty()) {
        // Load the config file
        std::cout << "Loading config file: " << config_file << std::endl;
        config.bm_config = YAML::LoadFile(config_file);
        if (!config.bm_config["benchmark"])
        {
            std::cerr << "No benchmark specified in config file" << std::endl;
            return 1;
        }
        config.benchmark_name = config.bm_config["benchmark"].as<std::string>();
    }

    return true;
}
