# -*- coding: utf-8 -*-
# Copyright (c) 2017 Jason Lowe-Power
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

""" This file creates a barebones system and executes 'hello', a simple Hello
World application. Adds a simple cache between the CPU and the membus.

This config file assumes that the x86 ISA was built.
"""

# import the m5 (gem5) library created when gem5 is built
import m5
# import all of the SimObjects
from m5.objects import *
from pathlib import Path
ROOT = '/disk/local/home/s2033074/vSwarm-u'
print(ROOT)

import sys
sys.path.append(ROOT +'/gem5utils/systems/') # For the next line...
from intel.system import IntelSystem
from simple.system import SimpleSystem
from drive.system import DriveSystem

from intel.icelake.caches import *
from intel.icelake.core import *

# from intel.skylake.caches import *
# from intel.skylake.core import *



class X86O3Checker(BaseO3Checker):
    mmu = X86MMU()
    ArchDecoder = X86Decoder
    ArchMMU = X86MMU
    ArchInterrupts = X86LocalApic
    ArchISA = X86ISA








# create the system we are going to simulate
system = System()

# Set the clock fequency of the system (and all of its children)
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = '1GHz'
system.clk_domain.voltage_domain = VoltageDomain()

# Set up the system
system.mem_mode = 'timing'               # Use timing accesses

# Create an address range for the system
sys_range = AddrRange('512MB')
# Reserve the last two MB for the istream traces
record_trace_range = AddrRange(start='480MB', size='16MB')
replay_trace_range = AddrRange(start='496MB', size='16MB')


system.mem_ranges = [sys_range]


print(system.mem_ranges[0])

# Create a simple CPU
system.cpu = TimingSimpleCPU()
system.cpu = TimingSimpleCPU()
system.cpu = IcxTunedCPU(fetchTagetWidth=32)

FDP=4
if FDP > 0:
    system.cpu.decoupledFrontEnd=True
    system.cpu.numFTQEntries=FDP

system.cpu.branchPred = BranchPredInf()
system.cpu.branchPred.takenOnlyHistory=True
system.cpu.branchPred.tage.speculativeHistUpdate=True
# system.cpu.branchPred.takenOnlyHistory=False
# system.cpu.addCheckerCpu()





# system.cpu_atomic = AtomicSimpleCPU(switched_out = True)
# to_atomic = [(system.cpu,system.cpu_atomic)]
# to_timing = [(system.cpu_atomic,system.cpu)]

# Create a memory bus, a coherent crossbar, in this case
system.membus = SystemXBar()


#### Parameter def
region_size='1kB'
buffer_entries=4
va=True

# trace_file='itrace_{}x{}_{}'.format(region_size,buffer_entries,"VA" if va else "PA")
# if not os.path.exists(trace_file):
#   trace_file = None






# Create a simple cache
system.icache = L1ICache()
system.icache.connectCPU(system.cpu)
system.icache.connectBus(system.membus)
# system.icache.prefetcher = IStreamPrefetcher(
#   # region_size= "256B",
#   # buffer_entries=4,
#   record_misses_only=True,
#   region_size=region_size,
#   record_buffer_entries=buffer_entries,
#   replay_buffer_entries=32,
#   queue_size=64,
#   use_virtual_addresses=va,
#   cache_snoop=True,
#   record_addr_range=record_trace_range,
#   replay_addr_range=replay_trace_range,
#   )

# system.icache.prefetcher.listenFromProbe(system.icache, "All")
# system.icache.prefetcher.addCacheProbes(system.icache)
# system.icache.prefetcher = TaggedPrefetcher()
## Add TLB
# system.iTLB = X86TLB()
# system.icache.prefetcher.registerTLB(system.iTLB)

## Connect the Prefetcher to the memory
# Directly
# system.icache.prefetcher.port = system.membus.cpu_side_ports
# Put a com monitor in between
# system.com_mon = CommMonitor()
# system.com_mon.stackdist = StackDistProbe(verify = True,
#                                           line_size=64)
# # system.icache.prefetcher.port = system.com_mon.cpu_side_port
# system.com_mon.mem_side_port = system.membus.cpu_side_ports

# Connect also the backdoor directly
# system.icache.prefetcher.backdoor = system.membus.cpu_side_ports
# system.icache.prefetcher.dmaport = system.membus.cpu_side_ports


system.dcache = L1DCache()
# Connect the I and D cache ports of the CPU to the memobj.
# Since cpu_side is a vector port, each time one of these is connected, it will
# create a new instance of the CPUSidePort class

system.dcache.connectCPU(system.cpu)


# Hook the cache up to the memory bus

system.dcache.connectBus(system.membus)

# create the interrupt controller for the CPU and connect to the membus
system.cpu.createInterruptController()
system.cpu.interrupts[0].pio = system.membus.mem_side_ports
system.cpu.interrupts[0].int_requestor = system.membus.cpu_side_ports
system.cpu.interrupts[0].int_responder = system.membus.mem_side_ports

# Create a DDR3 memory controller and connect it to the membus
system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

# Connect the system up to the membus
system.system_port = system.membus.cpu_side_ports

# Create a process for a simple "Hello World" application
process = Process()
# Set the command
# grab the specific path to the binary
thispath = os.path.dirname(os.path.realpath(__file__))
# binpath = os.path.join(thispath, ROOT + '/resources/gem5/',
#                        'tests/test-progs/hello/bin/x86/linux/hello')

binpath = os.path.join(thispath, ROOT + "/../uarch-bench/bench")
                    # 'tests/test-progs/hello/src/hello')

# exitpath = os.path.join(thispath, '../gem5/',
#                        'tests/test-progs/m5-exit/bin/x86/linux/hello')
# cmd is a list which begins with the executable (like argv)
process.cmd = [binpath, "-z", "-m", "-s", "512", "-r", "2"]
# Set the cpu to use the process as its workload and create thread contexts
system.cpu.workload = process
system.cpu.createThreads()
system.cpu.workload = process
# system.cpu_atomic.createThreads()


system.cpu.inst_trace = InstTrace(
                trace_commit = False,
                trace_fetch = False,
                trace_branches = True,
                trace_memref = True,
                instTraceFile = "inst_commit.gz",
        )



system.m5ops_base = int("ffff0000",16)
system.exit_on_work_items = True



system.workload = SEWorkload.init_compatible(binpath)

# system.cpu.addCheckerCpu()

# system.cpu.checker =  X86O3Checker(workload=system.cpu.workload,
#                                     exitOnError=False,
#                                     updateOnError=True,
#                                     warnOnlyOnLoadError=True)
# system.cpu.checker.mmu.itb.size = system.cpu.mmu.itb.size
# system.cpu.checker.mmu.dtb.size = system.cpu.mmu.dtb.size
# system.cpu.checker.cpu_id = system.cpu.cpu_id










## Run a few instructions then stop t
# system.cpu.max_insts_any_thread = 10

# set up the root SimObject and start the simulation
root = Root(full_system = False, system = system)
# instantiate all of the objects we've created above

use_checkpoint=True
# use_checkpoint=False

if use_checkpoint:
    m5.instantiate("cpt")
else:
    m5.instantiate()
#


# print("Beginning simulation!")
# exit_event = m5.simulate()
# print('Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause()))

## Now drain the state and start the recording
# print("Switch to atomic for init")
# m5.switchCpus(to_atomic)
# system.icache.prefetcher.initReplay('%s/%s.in' %(m5.options.outdir,trace_file))
# m5.drain()
# m5.switchCpust(to_timing)

# Start record and replay.
# system.icache.prefetcher.startRecord(False)
# print("Start record...")
# if (system.icache.prefetcher.startReplay(False)):
#     print("Start replay...")


# # system.icache.prefetcher.startReplay()
# print("Beginning simulation!")
# # system.cpu.max_insts_any_thread = 1000
# exit_event = m5.simulate()
# print('Exiting @ tick %i because %s' % (m5.curTick(), exit_event.getCause()))

# m5.stats.dump()
# exit(0)

take_checkpoint=True
take_checkpoint=False


def workitem(begin, id):
    global take_checkpoint
    if begin:
        if take_checkpoint:
            m5.checkpoint("cpt3")
            take_checkpoint = False
        print("Start measuring: ", id)
        m5.stats.reset()

    else:
        print("Stop measuring: ", id)
        m5.stats.dump()

def simulate():
    '''
    --- Main simulation loop ---
    Run the simulator until either
    - user exits
    - or the run script exits with fail code -1
    '''
    _exit=False
    while not _exit:
        print("Start simulation...")
        exit_event = m5.simulate()

        if exit_event.getCause() in ["workbegin", "workend"]:
            begin= True if exit_event.getCause() == "workbegin" else False

            _exit=workitem(begin,exit_event.getCode())

        elif exit_event.getCause() == "user interrupt received":
            print("Received user interrupt. Exit simulation")
            _exit=True

        else:
            print("Exit cause: %s | code: %d" % (exit_event.getCause(), exit_event.getCode()))
            _exit = True



print("Beginning simulation!")
simulate()
print('Exiting @ tick %i' % (m5.curTick()))