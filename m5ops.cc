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



#include "m5ops.hh"

#include "m5lib/m5_mmap.h"
#include "m5lib/m5ops.h"


M5Ops::M5Ops()
  : initialized(false)
{
}

M5Ops::~M5Ops()
{
	unmap_m5_mem();
}

void M5Ops::init()
{
	map_m5_mem();
	initialized = true;
}

void M5Ops::start()
{
	// if (!initialized) {
	// 	std::cerr << "Counters not initialized " << std::endl;
	// }
	resetStats();
}


void M5Ops::stop()
{
	dumpStats();
}

void M5Ops::dumpStats()
{
	m5_dump_stats(0,0);
}

void M5Ops::resetStats()
{
	m5_reset_stats(0,0);
}

void M5Ops::workBegin(uint id)
{
	m5_work_begin(id,0);
}

void M5Ops::workEnd(uint id)
{
	m5_work_end(id,0);
}
