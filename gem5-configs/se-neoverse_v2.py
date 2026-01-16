# Copyright (c) 2024 Technical University of Munich
# All rights reserved.
#
# The license below extends only to copyright in the software and shall
# not be construed as granting a license to any other intellectual
# property including but not limited to intellectual property relating
# to a hardware implementation of the functionality of the software
# licensed hereunder.  You may use the software subject to the license
# terms below provided that you ensure that this notice is replicated
# unmodified and in its entirety in all distributions of the software,
# modified or unmodified, in source code or in binary form.
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
#

"""
Simple gem5 configuration script to run a binary in syscall emulation mode.
Usage: ./build/<ISA>/gem5.opt se-imple.py <BINARY_PATH> <ARG1> <ARG2> ...

"""

import argparse
import m5


from gem5.isas import ISA
from gem5.utils.requires import requires
from gem5.resources.resource import BinaryResource
from gem5.components.memory import SingleChannelDDR3_1600
from gem5.components.boards.simple_board import SimpleBoard

from gem5.components.processors.base_cpu_core import BaseCPUCore
from gem5.components.processors.base_cpu_processor import BaseCPUProcessor
from gem5.simulate.simulator import Simulator
from gem5.simulate.exit_event import ExitEvent

from util import neoverse_v2

parser = argparse.ArgumentParser(
    description="An example configuration script to run a binary in system call emulation"
)

# The only positional argument accepted is the benchmark name in this script.

parser.add_argument("cmd", nargs=argparse.REMAINDER)

args = parser.parse_args()


# This check ensures the gem5 binary is compiled to the correct ISA target.
# If not, an exception will be thrown.
requires(isa_required=ISA.ARM)

# We use a single channel DDR3_1600 memory system
memory = SingleChannelDDR3_1600(size="512MiB")

cache_hierarchy = neoverse_v2.CacheHierarchy()

processor = BaseCPUProcessor(
    cores=[BaseCPUCore(neoverse_v2.NeoverseV2(), isa=ISA.ARM)]
)

cpu = processor.cores[-1].core


print(
    "Running on NeoverseV2: {}".format(args.cmd)
)



# The gem5 library simble board which can be used to run simple SE-mode
# simulations.
board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy,
)


# Here we set the workload.
board.set_se_binary_workload(
    binary=BinaryResource(args.cmd[0]),
    arguments=args.cmd[1:],
)

# This function will be called at the beginning and end of each
# repetition of the benchmark (when the `-m/--m5ops` option is used).
# It will print the current repetition number and dump the statistics
# of the simulation.
def workitems(start) -> bool:
    cnt = 1
    while True:
        if start:
            print(f"Begin Repetition {cnt} @ tick: { m5.curTick() }")
            m5.stats.reset()
        else:
            print(f"End Repetition {cnt} @ tick: { m5.curTick() }")
            m5.stats.dump()
            cnt += 1

        yield False


# Lastly we run the simulation.
# We define the system with the aforementioned system defined.
simulator = Simulator(
    board=board,
    on_exit_event={
        ExitEvent.WORKBEGIN: workitems(True),
        ExitEvent.WORKEND: workitems(False),
        },
)
simulator.run()

print(
    "Exiting @ tick {} because {}.".format(
        simulator.get_current_tick(), simulator.get_last_exit_event_cause()
    )
)
