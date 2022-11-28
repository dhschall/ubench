/*
 * Copyright (c) 2022 EASE Group, The University of Edinburgh
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


#ifndef __PERF_HH__
#define __PERF_HH__

#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <errno.h>

#include "util.hh"

#include <error.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>

class PerfEvent {
	struct event {
		struct read_format {
			uint64_t value;
			uint64_t time_enabled;
			uint64_t time_running;
			uint64_t id;
		};

		perf_event_attr pe;
      std::string name;
		int fd;
		read_format prev;
		read_format data;

      double readCounter();
      uint64_t readRaw() { return data.value; }
	};

	std::vector<event> events;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> stopTime;

	// A group file descriptor
	int gfd;
	bool initialized;

    public:
	PerfEvent() : gfd(-1), initialized(false)
	{
	}
   // PerfEvent(std::string _name) :  gfd(-1), initialized(false), name(_name) {}

	/** Register new counter events */
	void registerCounter(const std::string &name, uint64_t type,
			     uint64_t eventID);

	/** Initialize all counters */
	void init();
	void start();

	~PerfEvent();

	void stop();

	double getDuration()
	{
		return std::chrono::duration<double>(stopTime - startTime)
			.count();
	}

	double getIPC() {
		return getCounter("instructions") / getCounter("cycles");
	}

   // double getMPKI() {
	// 	return getCounter("instructions") * 1000 / getCounter("L1D-misses");
	// }

   // double getMissRatio() {
	// 	return getCounter("instructions") / getCounter("cycles");
	// }

	double getCounter(const std::string &name);

   void printCounters();
};

#endif // __PERF_HH__