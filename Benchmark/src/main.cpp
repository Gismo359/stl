#include <benchmark/benchmark.h>

static void Benchmark(benchmark::State & state) {
    int i;
    for (auto _ : state)
    {
    }
    benchmark::DoNotOptimize(i);
}
BENCHMARK(Benchmark);