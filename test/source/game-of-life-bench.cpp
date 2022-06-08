#include <vector>

#include <benchmark/benchmark.h>

#include "gol.h"
#include "gol.hpp"

static void gol_cpp_bench_update(benchmark::State& state)
{
  const uint64_t width = state.range(0);
  const uint64_t height = state.range(0);
  const uint64_t cells = width * height;

  auto grid = benlib::Gol(width, height);
  grid.RandomFill();
  benchmark::DoNotOptimize(grid);

  for (auto _ : state) {
    grid.Update();
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations() * cells);
  state.SetBytesProcessed(state.iterations() * cells * sizeof(bool));
}
BENCHMARK(gol_cpp_bench_update)->Name("gol_cpp_bench_update")->RangeMultiplier(2)->Range(16, 64);

static void gol_cpp_bench_random_fill(benchmark::State& state)
{
  const uint64_t width = state.range(0);
  const uint64_t height = state.range(0);
  const uint64_t cells = width * height;

  auto grid = benlib::Gol(width, height);
  benchmark::DoNotOptimize(grid);

  for (auto _ : state) {
    grid.RandomFill();
    benchmark::ClobberMemory();
  }

  state.SetItemsProcessed(state.iterations() * cells);
  state.SetBytesProcessed(state.iterations() * cells * sizeof(bool));
}
BENCHMARK(gol_cpp_bench_random_fill)->Name("gol_cpp_bench_random_fill")->RangeMultiplier(2)->Range(16, 64);

static void gol_c_bench_update(benchmark::State& state)
{
  const uint64_t width = state.range(0);
  const uint64_t height = state.range(0);
  const uint64_t cells = width * height;

  bool** grid = CreateGrid(width, height);
  RandomFill(grid, width, height);

  benchmark::DoNotOptimize(grid);

  for (auto _ : state) {
    UpdateGrid(grid, width, height);
    benchmark::ClobberMemory();
  }
  FreeGrid(grid, width);

  state.SetItemsProcessed(state.iterations() * cells);
  state.SetBytesProcessed(state.iterations() * cells * sizeof(bool));
}
BENCHMARK(gol_c_bench_update)->Name("gol_c_bench_update")->RangeMultiplier(2)->Range(16, 64);

static void gol_c_bench_random_fill(benchmark::State& state)
{
  const uint64_t width = state.range(0);
  const uint64_t height = state.range(0);
  const uint64_t cells = width * height;

  bool** grid = CreateGrid(width, height);
  benchmark::DoNotOptimize(grid);

  for (auto _ : state) {
    RandomFill(grid, width, height);
    benchmark::ClobberMemory();
  }
  FreeGrid(grid, width);

  state.SetItemsProcessed(state.iterations() * cells);
  state.SetBytesProcessed(state.iterations() * cells * sizeof(bool));
}
BENCHMARK(gol_c_bench_random_fill)->Name("gol_c_bench_random_fill")->RangeMultiplier(2)->Range(16, 64);