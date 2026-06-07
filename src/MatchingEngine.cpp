#include "MatchingEngine.h"

#include <iostream>
#include <algorithm>

MatchingEngine::MatchingEngine()
    : running(false),
      orderPool(100000)
{
}

MatchingEngine::~MatchingEngine() {
    stop();
}

void MatchingEngine::start() {

    running = true;

    matchingThread =
        std::thread(
            &MatchingEngine::matchingLoop,
            this
        );
}

void MatchingEngine::stop() {

    running = false;

    orderQueue.push(nullptr);

    if (matchingThread.joinable()) {
        matchingThread.join();
    }
}

uint64_t MatchingEngine::submitOrder(
    const std::string& symbol,
    OrderSide side,
    OrderType type,
    int price,
    int quantity
) {

    uint64_t id =
        nextOrderId++;

    Order* order =
        orderPool.allocate(
            id,
            symbol,
            side,
            type,
            price,
            quantity
        );

    orderLookup[id] = order;

    orderQueue.push(order);

    return id;
}

void MatchingEngine::matchingLoop() {

    while (running) {

        Order* order =
            orderQueue.pop();

        if (order == nullptr) {
            continue;
        }

        processOrder(order);
    }
}

void MatchingEngine::processOrder(
    Order* order
) {

    if (order->side ==
        OrderSide::BUY)
    {
        matchBuyOrder(order);
    }
    else
    {
        matchSellOrder(order);
    }
}

void MatchingEngine::matchBuyOrder(
    Order* order
) {

    while (!sellBook.empty() &&
           order->remainingQty > 0)
    {

        auto bestSell =
            sellBook.begin();

        int sellPrice =
            bestSell->first;

        if (order->type ==
            OrderType::LIMIT &&
            sellPrice > order->price)
        {
            break;
        }

        auto& sellQueue =
            bestSell->second;

        while (!sellQueue.empty() &&
               order->remainingQty > 0)
        {

            Order* sellOrder =
                sellQueue.front();

            int tradeQty =
                std::min(
                    order->remainingQty,
                    sellOrder->remainingQty
                );

            Trade trade(
                order->orderId,
                sellOrder->orderId,
                order->symbol,
                sellOrder->price,
                tradeQty
            );

            tradeQueue.push(trade);

            order->remainingQty -=
                tradeQty;

            sellOrder->remainingQty -=
                tradeQty;

            if (sellOrder
                ->remainingQty == 0)
            {

                sellOrder->status =
                    OrderStatus::FILLED;

                sellQueue.pop_front();

                orderLookup.erase(
                    sellOrder
                    ->orderId
                );

                orderPool.deallocate(
                    sellOrder
                );
            }
        }

        if (sellQueue.empty()) {
            sellBook.erase(bestSell);
        }
    }

    if (order->remainingQty > 0 &&
        order->type ==
        OrderType::LIMIT)
    {

        buyBook
        [order->price]
        .push_back(order);
    }
    else
    {
        order->status =
            OrderStatus::FILLED;

        orderLookup.erase(
            order->orderId
        );

        orderPool.deallocate(
            order
        );
    }
}

void MatchingEngine::matchSellOrder(
    Order* order
) {

    while (!buyBook.empty() &&
           order->remainingQty > 0)
    {

        auto bestBuy =
            buyBook.begin();

        int buyPrice =
            bestBuy->first;

        if (order->type ==
            OrderType::LIMIT &&
            buyPrice < order->price)
        {
            break;
        }

        auto& buyQueue =
            bestBuy->second;

        while (!buyQueue.empty() &&
               order->remainingQty > 0)
        {

            Order* buyOrder =
                buyQueue.front();

            int tradeQty =
                std::min(
                    order->remainingQty,
                    buyOrder->remainingQty
                );

            Trade trade(
                buyOrder->orderId,
                order->orderId,
                order->symbol,
                buyOrder->price,
                tradeQty
            );

            tradeQueue.push(trade);

            order->remainingQty -=
                tradeQty;

            buyOrder->remainingQty -=
                tradeQty;

            if (buyOrder
                ->remainingQty == 0)
            {

                buyOrder->status =
                    OrderStatus::FILLED;

                buyQueue.pop_front();

                orderLookup.erase(
                    buyOrder
                    ->orderId
                );

                orderPool.deallocate(
                    buyOrder
                );
            }
        }

        if (buyQueue.empty()) {
            buyBook.erase(bestBuy);
        }
    }

    if (order->remainingQty > 0 &&
        order->type ==
        OrderType::LIMIT)
    {

        sellBook
        [order->price]
        .push_back(order);
    }
    else
    {
        order->status =
            OrderStatus::FILLED;

        orderLookup.erase(
            order->orderId
        );

        orderPool.deallocate(
            order
        );
    }
}

void MatchingEngine::cancelOrder(
    uint64_t orderId
) {

    auto it =
        orderLookup.find(
            orderId
        );

    if (it ==
        orderLookup.end())
    {
        return;
    }

    Order* order =
        it->second;

    order->status =
        OrderStatus::CANCELLED;

    order->remainingQty = 0;

    orderLookup.erase(it);
}

void MatchingEngine::modifyOrder(
    uint64_t orderId,
    int newPrice,
    int newQuantity
) {

    auto it =
        orderLookup.find(
            orderId
        );

    if (it ==
        orderLookup.end())
    {
        return;
    }

    Order* order =
        it->second;

    order->price =
        newPrice;

    order->quantity =
        newQuantity;

    order->remainingQty =
        newQuantity;
}

void MatchingEngine::printOrderBook() {

    std::cout
        << "\nBUY BOOK\n";

    for (auto& [price, q]
         : buyBook)
    {
        std::cout
            << price
            << " -> ";

        for (auto* order : q)
        {
            std::cout
                << order
                ->remainingQty
                << " ";
        }

        std::cout << "\n";
    }

    std::cout
        << "\nSELL BOOK\n";

    for (auto& [price, q]
         : sellBook)
    {
        std::cout
            << price
            << " -> ";

        for (auto* order : q)
        {
            std::cout
                << order
                ->remainingQty
                << " ";
        }

        std::cout << "\n";
    }
}

ThreadSafeQueue<Trade>&
MatchingEngine::getTradeQueue()
{
    return tradeQueue;
}