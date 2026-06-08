#pragma once

#include <thread>
#include <atomic>
#include <fstream>
#include <iostream>

#include "Trade.h"
#include "ThreadSafeQueue.h"

class Logger {
private:

    ThreadSafeQueue<Trade>& tradeQueue;

    std::thread loggerThread;

    std::atomic<bool> running;

    std::ofstream logFile;

public:

    explicit Logger(
        ThreadSafeQueue<Trade>& tq
    )
        : tradeQueue(tq),
          running(false)
    {
        logFile.open("trades.log");
    }

    ~Logger() {
        stop();

        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void start() {

        running = true;

        loggerThread =
            std::thread(
                &Logger::loggingLoop,
                this
            );
    }

    void stop() {

        running = false;

        tradeQueue.push(
            Trade()
        );

        if (
            loggerThread.joinable()
        ) {
            loggerThread.join();
        }
    }

private:

    void loggingLoop() {

        while (running) {

            Trade trade =
                tradeQueue.pop();

            if (trade.price == 0) {
                continue;
            }

            logTrade(trade);
        }
    }

    void logTrade(
        const Trade& trade
    ) {

        std::string msg =
            "TRADE | BUY=" +
            std::to_string(
                trade.buyOrderId
            ) +
            " SELL=" +
            std::to_string(
                trade.sellOrderId
            ) +
            " SYMBOL=" +
            trade.symbol +
            " PRICE=" +
            std::to_string(
                trade.price
            ) +
            " QTY=" +
            std::to_string(
                trade.quantity
            );

        std::cout
            << msg
            << std::endl;

        logFile
            << msg
            << std::endl;
    }
};