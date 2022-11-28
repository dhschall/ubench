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



#include "perf.hh"

#include <error.h>

#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <unistd.h>

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
			    int cpu, int group_fd, unsigned long flags)
{
	int ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd,
			  flags);
	return ret;
}


double
PerfEvent::event::readCounter() {
	double multiplexingCorrection = static_cast<double>(data.time_enabled) / static_cast<double>(data.time_running);
	return static_cast<double>(data.value) * multiplexingCorrection;
}

void PerfEvent::registerCounter(const std::string &name, uint64_t type,
				uint64_t eventID)
{
	events.push_back(event());
	auto &event = events.back();
	event.name = name;
	auto &pe = event.pe;
	memset(&pe, 0, sizeof(struct perf_event_attr));
	pe.type = static_cast<uint32_t>(type);
	pe.size = sizeof(struct perf_event_attr);
	pe.config = eventID;
	pe.disabled = true;
	pe.inherit = 1;
	pe.inherit_stat = 0;
	pe.exclude_kernel = false;
	pe.exclude_hv = false;
	pe.read_format =
		PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING;
}

void PerfEvent::init()
{
	if (!events.size()) {
		std::cerr << "No counters configured" << std::endl;
	}
	events[0].fd = -1;
	for (int i = 0; i < events.size(); i++) {
		events[i].fd = perf_event_open(&(events[i].pe), 0, -1, events[0].fd,
					       PERF_FLAG_FD_CLOEXEC);

		if (events[i].fd == -1) {
			std::cerr << "Error setting counter: " << events[i].name  << std::endl;
		}
	}

	initialized = true;
}

void PerfEvent::start()
{
	//   for (unsigned i=0; i<events.size(); i++) {
	//      auto& event = events[i];
	//      ioctl(event.fd, PERF_EVENT_IOC_RESET, 0);
	//      ioctl(event.fd, PERF_EVENT_IOC_ENABLE, 0);
	//      if (read(event.fd, &event.prev, sizeof(uint64_t) * 3) != sizeof(uint64_t) * 3)
	//         std::cerr << "Error reading counter " << names[i] << std::endl;
	//   }
	if (!initialized) {
		std::cerr << "Counters not initialized " << std::endl;
	}


	asm volatile("" ::: "memory");
	ioctl(events[0].fd, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
	ioctl(events[0].fd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
	asm volatile("" ::: "memory");
	startTime = std::chrono::steady_clock::now();
}

PerfEvent::~PerfEvent()
{
	for (auto &event : events) {
		close(event.fd);
	}
}

void PerfEvent::stop()
{
	stopTime = std::chrono::steady_clock::now();
	asm volatile("" ::: "memory");
	ioctl(events[0].fd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

	for (unsigned i = 0; i < events.size(); i++) {
		auto &event = events[i];
		if (read(event.fd, &event.data, sizeof(uint64_t) * 3) !=
		    sizeof(uint64_t) * 3)
			std::cerr << "Error reading counter " << event.name  << std::endl;
		//  ioctl(event.fd, PERF_EVENT_IOC_DISABLE, 0);
		// std::cout << "Setup: " << event.name << " " << event.fd << std::endl;
	}
}

double
PerfEvent::getCounter(const std::string &name)
{

	for (auto& event : events)
		if (event.name == name)
			return event.readCounter();
	return -1;
}

void PerfEvent::printCounters() {
	int width = 15;
    for (auto event : events) {
		std::cout << std::setw(static_cast<int>(width))
				  << event.name << ": "
				  <<  std::fixed << std::setprecision(0)
				  << event.readCounter()
				  << std::endl;
	}
}
