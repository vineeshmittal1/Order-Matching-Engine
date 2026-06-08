#include <thread>
#include <chrono>

#include "MatchingEngine.h"
#include "Logger.h"
#include "Benchmark.h"

int main() {

    MatchingEngine engine;

    engine.start();

    Logger logger(
        engine.getTradeQueue()
    );

    logger.start();

    Benchmark benchmark;

    benchmark.start();

    const int TOTAL =
        100000;

    for (
        int i = 0;
        i < TOTAL;
        i++
    ) {

        engine.submitOrder(
            "AAPL",
            i % 2 == 0
            ? OrderSide::BUY
            : OrderSide::SELL,
            OrderType::LIMIT,
            100 + (i % 10),
            10
        );
    }

    std::this_thread::
    sleep_for(
        std::chrono::
        seconds(2)
    );

    benchmark.stopAndPrint(
        TOTAL
    );

    engine.printOrderBook();

    logger.stop();

    engine.stop();

    return 0;
}