#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <array>
#include "utils.h"
#include "algorithms.h"
#include "papi_wrapper.h"

using namespace std;

enum class Algo { Naive, Oblivious, Aware };

constexpr const char* algo_to_str(Algo a) {
    switch (a) {
        case Algo::Naive:     return "Naive";
        case Algo::Oblivious: return "Oblivious";
        case Algo::Aware:     return "Aware";
    }
}

struct TimeEntry {
    Algo algo;
    int N, B, run;
    double time_ms;
};

struct PapiEntry {
    Algo algo;
    int N, B, run;
    long long l2_misses;
};

template<typename T>
inline void DoNotOptimize(T& val) {
    asm volatile("" : "+r,m"(val) : : "memory");
}

int main() {
    bool papi_available = papi_init();
    if (!papi_available) {
        cerr << "Warning: PAPI not available. Cache miss measurements will be skipped/invalid." << endl;
    }

    constexpr array<int, 5> N_values = {256, 512, 1024, 2048, 4096};
    constexpr array<int, 6> B_values = {32, 128, 256, 512, 1024, 2048};
    constexpr int NUM_RUNS = 100;

    // N_values * (2 fixed algos + B_values block sizes) * NUM_RUNS
    constexpr size_t TOTAL_RUNS = N_values.size() * (2 + B_values.size()) * NUM_RUNS;

    array<TimeEntry, TOTAL_RUNS> time_results;
    array<PapiEntry, TOTAL_RUNS> papi_results;
    size_t time_idx = 0;
    size_t papi_idx = 0;

    cout << "Starting Benchmarks... Tip: Run via 'taskset -c 0 ./matrix_benchmark' for CPU pinning." << endl;

    for (int N : N_values) {
        cout << "\n=== Running experiments for N = " << N << " ===" << endl;

        vector<int> matrix = generate_random_matrix(N);

        auto run_timing = [&](Algo algo, int B, auto func) {
            cout << "  [Time Phase] " << algo_to_str(algo) << " (B=" << B << ")..." << flush;
            for (int run = 1; run <= NUM_RUNS; ++run) {
                auto start = chrono::high_resolution_clock::now();
                func(matrix, N, B);
                DoNotOptimize(matrix[0]);
                auto end = chrono::high_resolution_clock::now();

                chrono::duration<double, milli> duration = end - start;
                time_results[time_idx++] = {algo, N, B, run, duration.count()};
            }
            cout << " Done." << endl;
        };

        auto run_papi = [&](Algo algo, int B, auto func) {
            if (!papi_available) return;
            cout << "  [PAPI Phase] " << algo_to_str(algo) << " (B=" << B << ")..." << flush;
            for (int run = 1; run <= NUM_RUNS; ++run) {
                papi_start();
                func(matrix, N, B);
                DoNotOptimize(matrix[0]);
                long long l2_misses = papi_stop();

                papi_results[papi_idx++] = {algo, N, B, run, l2_misses};
            }
            cout << " Done." << endl;
        };

        auto naive_wrap     = [](vector<int>& m, int n, int b) { transpose_naive(m, n); };
        auto oblivious_wrap = [](vector<int>& m, int n, int b) { transpose_cache_oblivious(m, n); };
        auto aware_wrap     = [](vector<int>& m, int n, int b) { transpose_cache_aware(m, n, b); };

        run_timing(Algo::Naive,     0, naive_wrap);
        run_papi  (Algo::Naive,     0, naive_wrap);
        run_timing(Algo::Oblivious, 0, oblivious_wrap);
        run_papi  (Algo::Oblivious, 0, oblivious_wrap);

        for (int B : B_values) {
            run_timing(Algo::Aware, B, aware_wrap);
            run_papi  (Algo::Aware, B, aware_wrap);
        }
    }

    // --- Write collected results to CSV ---
    cout << "\nWriting results to CSV files..." << endl;

    string output_dir = "data";
    for (Algo algo : {Algo::Naive, Algo::Oblivious, Algo::Aware}) {
        const char* name = algo_to_str(algo);

        ofstream csv_time(string(output_dir + "/results_time_") + name + ".csv");
        csv_time << "N,BlockSize,RunID,Time_ms\n";
        for (size_t i = 0; i < time_idx; ++i) {
            const auto& e = time_results[i];
            if (e.algo != algo) continue;
            csv_time << e.N << "," << e.B << "," << e.run << "," << e.time_ms << "\n";
        }
        csv_time.close();

        ofstream csv_papi(string(output_dir + "/results_papi_") + name + ".csv");
        csv_papi << "N,BlockSize,RunID,L2_TCM\n";
        for (size_t i = 0; i < papi_idx; ++i) {
            const auto& e = papi_results[i];
            if (e.algo != algo) continue;
            csv_papi << e.N << "," << e.B << "," << e.run << "," << e.l2_misses << "\n";
        }
        csv_papi.close();
    }

    cout << "Experiments completed. Results saved to per-algorithm CSV files.\n";

    return 0;
}