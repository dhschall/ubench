# Microbenchmarks

The following table lists the already available microbenchmarks in the &mu;Bench tool. 
Name | Group | Description | `x86` | `Arm` | `RISC-V`|
--- | --- | --- | :-: | :-: | :-: |
`simple-loop` | Example | Simple loop that does nothing | ✅ | ✅ | ✅
`random-branch` | Branch | A loop with a random branch |  ✅ | ✅ | ✅
`branch-throughput` | Branch | Executes a sequence of branches and Nops. Useful to determine maximum branch througput.| :x: | ✅ | :x: 
`btb-stress` | BTB | Executes `N` number of unique branch instructions |  ✅ | ✅ | ✅
`btb-stress-asm` | BTB | Similarly to `btb-stress` but written in assembly allowing more specific functionality |  ✅ | ✅ | ✅


## Adding a New Benchmark

To add a new benchmark, you can either use the [simple-loop](./template/simple_loop.cc) benchmark as a template or create a new file in the `benchmarks` folder. 
1. Implement your benchmark as a new class derived from the `BaseBenchmark` class (see below).
2. Add your benchmark to the `CMakeLists.txt` file in the `benchmarks` folder.
3. Register your benchmark using the `REGISTER_BENCHMARK` macro after your class definition. This macro takes the name of the benchmark and the class name as arguments. For example:
```cpp
REGISTER_BENCHMARK("simple-loop", SimpleLoop);
```

### Interface
Each microbenchmark should be derived from the `BaseBenchmark` class defined in [`base.hh`](base.hh) which provides a common interface for all microbenchmarks. Each must at least implement the following functions:

- `init()`: Initializes the benchmark. This function is called once before the benchmark is run.
- `exec()`: Runs the benchmark. This function is called `repeats` times.
<!-- - `report()`: Reports the results of the benchmark. This function is called once after the benchmark is run. -->
- `cleanup()`: Cleans up the benchmark. This function is called once after the benchmark is run.

