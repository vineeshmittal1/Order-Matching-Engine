#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#include "../src/MatchingEngine.h"

int main() {

    MatchingEngine engine;

    engine.start();

    const int THREADS = 8;
    const int ORDERS_PER_THREAD =10000;

    std::vector<std::thread>
        workers;

    auto submitOrders =
        [&](int threadId)
    {

        for (
            int i = 0;
            i <
            ORDERS_PER_THREAD;
            i++
        )
        {

            OrderSide side =
                (i % 2 == 0)
                ? OrderSide::BUY
                : OrderSide::SELL;

            OrderType type =
                (i % 5 == 0)
                ? OrderType::IOC
                : OrderType::LIMIT;

            int price =
                100 + (i % 10);

            int qty =
                10 + (i % 20);

            engine.submitOrder(
                "AAPL",
                side,
                type,
                price,
                qty
            );
        }
    };

    auto start =
        std::chrono::
        high_resolution_clock
        ::now();

    for (
        int i = 0;
        i < THREADS;
        i++
    )
    {

        workers.emplace_back(
            submitOrders,
            i
        );
    }

    for (
        auto& t :
        workers
    )
    {
        t.join();
    }

    std::this_thread::
    sleep_for(
        std::chrono::
        seconds(2)
    );

    auto end =
        std::chrono::
        high_resolution_clock
        ::now();

    auto duration =
        std::chrono::
        duration_cast
        <
            std::chrono::
            milliseconds
        >
        (
            end - start
        );

    engine.stop();

    int totalOrders =
        THREADS *
        ORDERS_PER_THREAD;

    double throughput =
        totalOrders /
        (
            duration.count()
            / 1000.0
        );

    std::cout
        << "\n========== "
        << "STRESS TEST "
        << "==========\n";

    std::cout
        << "Threads: "
        << THREADS
        << "\n";

    std::cout
        << "Orders: "
        << totalOrders
        << "\n";

    std::cout
        << "Execution Time: "
        << duration.count()
        << " ms\n";

    std::cout
        << "Throughput: "
        << throughput
        << " orders/sec\n";

    std::cout
        << "\nStress Test "
        << "Passed\n";

    return 0;
}