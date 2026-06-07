#pragma once

#include <map>
#include <deque>
#include <unordered_map>
#include <thread>
#include <atomic>
#include <functional>

#include "Order.h"
#include "Trade.h"
#include "ThreadSafeQueue.h"
#include "MemoryPool.h"

class MatchingEngine {
private:

    using BuyBook =
        std::map<int,
        std::deque<Order*>,
        std::greater<int>>;

    using SellBook =
        std::map<int,
        std::deque<Order*>>;

    BuyBook buyBook;
    SellBook sellBook;

    std::unordered_map<uint64_t, Order*> orderLookup;

    MemoryPool<Order> orderPool;

    ThreadSafeQueue<Order*> orderQueue;
    ThreadSafeQueue<Trade> tradeQueue;

    std::thread matchingThread;
    std::atomic<bool> running;

public:

    MatchingEngine();

    ~MatchingEngine();

    void start();

    void stop();

    uint64_t submitOrder(
        const std::string& symbol,
        OrderSide side,
        OrderType type,
        int price,
        int quantity
    );

    void cancelOrder(uint64_t orderId);

    void modifyOrder(
        uint64_t orderId,
        int newPrice,
        int newQuantity
    );

    void processOrder(Order* order);

    void matchBuyOrder(Order* order);

    void matchSellOrder(Order* order);

    void matchingLoop();

    void printOrderBook();

    ThreadSafeQueue<Trade>&
    getTradeQueue();

private:

    std::atomic<uint64_t> nextOrderId{1};
};