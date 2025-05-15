# Microbenchmarks

## Interface
Each microbenchmark should be derived from the `Benchmark` class defined in [`benchmark.cc`](benchmark.cc) which provides the following interface:
- `init()`: Initializes the benchmark. This function is called once before the benchmark is run.
- `run()`: Runs the benchmark. This function is called `repeats` times.
<!-- - `report()`: Reports the results of the benchmark. This function is called once after the benchmark is run. -->
- `cleanup()`: Cleans up the benchmark. This function is called once after the benchmark is run.


## Adding a New Benchmark