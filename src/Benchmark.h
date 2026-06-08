#pragma once

#include <chrono>
#include <iostream>

class Benchmark {
private:

    std::chrono::
    high_resolution_clock::
    time_point startTime;

public:

    void start() {

        startTime =
            std::chrono::
            high_resolution_clock::
            now();
    }

    void stopAndPrint(
        int totalOrders
    ) {

        auto endTime =
            std::chrono::
            high_resolution_clock::
            now();

        auto duration =
            std::chrono::
            duration_cast
            <
                std::chrono::
                microseconds
            >
            (
                endTime
                - startTime
            );

        double totalMicros =
            duration.count();

        double throughput =
            totalOrders
            /
            (totalMicros
            / 1e6);

        double latency =
            totalMicros
            /
            totalOrders;

        std::cout
            << "\n=========== "
            << "BENCHMARK "
            << "===========\n";

        std::cout
            << "Orders: "
            << totalOrders
            << "\n";

        std::cout
            << "Total Time: "
            << totalMicros
            << " us\n";

        std::cout
            << "Avg Latency: "
            << latency
            << " us/order\n";

        std::cout
            << "Throughput: "
            << throughput
            << " orders/sec\n";
    }
};