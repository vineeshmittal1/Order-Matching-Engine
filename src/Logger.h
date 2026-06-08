#pragma once

#include <thread>
#include <atomic>
#include <fstream>
#include <iostream>
#include <string>

#include "Trade.h"
#include "ThreadSafeQueue.h"

class Logger {
private:

    ThreadSafeQueue<Trade>& tradeQueue;

    std::thread loggerThread;

    std::atomic<bool> running;

    std::ofstream logFile;

    int printedTrades = 0;
    const int MAX_PRINT = 20;

public:

    explicit Logger(
        ThreadSafeQueue<Trade>& tq
    )
        : tradeQueue(tq),
          running(false)
    {
        logFile.open("trades.log");

        if (!logFile.is_open()) {
            std::cerr
                << "Failed to open trades.log\n";
        }
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

        if (!running) {
            return;
        }

        running = false;

        // sentinel trade
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

        while (true) {

            Trade trade =
                tradeQueue.pop();

            // stop signal
            if (!running &&
                trade.price == 0)
            {
                break;
            }

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

        // print only first few trades
        if (
            printedTrades
            < MAX_PRINT
        )
        {
            std::cout
                << msg
                << std::endl;

            printedTrades++;
        }

        // save ALL trades
        if (logFile.is_open()) {

            logFile
                << msg
                << std::endl;
        }
    }
};