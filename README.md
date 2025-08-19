# &mu;Bench

&mu;Bench is a microbenchmarking tool to evaluate microarchitectural features on run real hardware and [gem5](https://gem5.org/) simulator. The tool is designed to be extensible, allowing users to add easily add their own microbenchmarks.

Refer to the [benchmarks README](benchmarks/README.md) for more information on the available benchmarks and how to add new ones.


## Build the Benchmarks

### Native Build
```bash
# Creates a build directory and runs cmake to generate the build files
# The -DARCH flag is used to specify the architecture for which the benchmarks are being built.
# The default is x86_64, but you can specify aarch64
cmake -B build -DARCH=x86_64                                      
# Build the benchmark.
cmake --build ./build -j $(nproc)
```

### Using Docker + Cross Compilation

&mu;Bench is designed to be cross-platform and can be built for different architectures using the `-DARCH` flag.
We use docker ([dockcross](https://github.com/dockcross/dockcross)) as a cross-compilation toolchain. 

You can use the provided `Makefile` to build the benchmarks for different architectures which will automatically download the required docker images and run the cross-compilation commands.

```bash
make <arch>
```

Where `<arch>` can be one of the following:
- `x86`: Build for x86_64 architecture (default)
- `arm64`: Build for ARM64 architecture
- `riscv64`: Build for RISC-V architecture
- `all`: Build for all architectures

You will find the compiled binaries in the corresponing `build-<arch>` folder.



If you want to cross-compile the microbenchmarking tool for ARM or X86 we can use [dockcross](https://github.com/dockcross/dockcross)

```bash
# The following command will create an invocation script for the cross compilation toolchain for ARM64
docker run --rm dockcross/linux-arm64 > ./dockcross-arm64 
chmod +x ./dockcross-arm64

# Once you have the invocation script, you can run the cmake command to generate the build files for ARM64
./dockcross-arm64 cmake -B <builddir> -DARCH=aarch64
# Build the benchmark.
./dockcross-arm64 cmake --build <builddir> -j $(nproc)
```

The same for x86_64:

```bash
# The following command will create an invocation script for the cross compilation toolchain for ARM64
docker run --rm dockcross/linux-x64 > ./dockcross-x64 
chmod +x ./dockcross-x64

# Once you have the invocation script, you can run the cmake command to generate the build files for ARM64
./dockcross-x64 cmake -B <builddir> 
# Build the benchmark.
./dockcross-x64 cmake --build <builddir> -j $(nproc)
```



## Running a benchmark

To run a benchmark, you need to specify the benchmark to run and optionally the number of repetitions. There are two ways to select the benchmark to run. (1) By name passing the benchmark name with the `--bmname/-b` flag to `ubench` or (2) by providing a configuration file with the `--config/-c` flag. The configuration file is a YAML file that can contain further configuration options for the benchmark. See `./configs` for example configuration files.


```bash
# Run the benchmark with the benchmark name
./build/ubench --bmname <benchmark_name> --repeats <num_repeats> 
# Run the benchmark with a configuration file
./build/ubench --config <config_file> --repeats <num_repeats> 
```


### Run the benchmark in gem5

The folder contains a minimal gem5 configuration to run the microbenchmarks in system call emulation mode (SE).
```bash
# Run the benchmark in gem5
<path/to/gem5>/build/<arch>/gem5.opt ./gem5-configs/se-simple.py --isa <arch> build-<arch>/ubench --bmname <benchmark_name> --repeats <num_repeats> 
```

Example:
```bash
# Run the benchmark in gem5
<path/to/gem5>/build/X86/gem5.opt ./gem5-configs/se-simple.py --isa X86 build-x64/ubench --bmname simple_loop --repeats 10 
```

### M5Ops
The microbenchmarks is compiled using gem5's magic instructions (m5ops). These instructions are useful the exit the simulation loop at specific point during the simulation.
By passing the `--m5ops/-m` flag to the microbenchmark, the benchmark execute a `workitem_start` before a repetition starts and a `workitem_end` after the repetition ends.
The user can then define in the gem5 configuration file what to do when the `workitem_start` and `workitem_end` instructions are executed. For example dumping or resetting the stats.
Refer to the [se-simple.py](./gem5-configs/se-simple.py) for an example of how to process the workitem exit instructions.


## Benchmarks
All the benchmarks are located in the `benchmarks` folder. Refer to the [benchmarks README](benchmarks/README.md) for more information on the benchmarks and how to add new ones.