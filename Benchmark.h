#pragma once

#include <chrono>
#include <iostream>


#define BENCHMARK(result, func, ...){\
std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now();\
func(##__VA_ARGS__);\
std::chrono::time_point<std::chrono::steady_clock> finish = std::chrono::high_resolution_clock::now();\
std::chrono::duration<double, std::nano> elapsed = finish - start;\
result = elapsed;\
}

#define BENCHMARK_MEAN(result, func, numbr_repeatings, ...)  { \
double mean = 0.0; \
for (size_t i = 0; i < numbr_repeatings; ++i) {\
std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now(); \
func(##__VA_ARGS__); \
std::chrono::time_point<std::chrono::steady_clock> finish = std::chrono::high_resolution_clock::now(); \
std::chrono::duration<double, std::nano> elapsed = finish - start; \
mean += elapsed.count()/numbr_repeatings;}\
result = mean; \
}



