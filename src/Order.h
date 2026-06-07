#pragma once

#include <string>
#include <chrono>

enum class OrderSide {
    BUY,
    SELL
};

enum class OrderType {
    LIMIT,
    MARKET,
    IOC
};

enum class OrderStatus {
    ACTIVE,
    FILLED,
    CANCELLED,
    PARTIAL
};

struct Order {
    uint64_t orderId;
    std::string symbol;

    OrderSide side;
    OrderType type;

    int price;
    int quantity;
    int remainingQty;

    uint64_t timestamp;

    OrderStatus status;

    Order() = default;

    Order(uint64_t id,
          const std::string& sym,
          OrderSide s,
          OrderType t,
          int p,
          int qty)
        : orderId(id),
          symbol(sym),
          side(s),
          type(t),
          price(p),
          quantity(qty),
          remainingQty(qty),
          status(OrderStatus::ACTIVE)
    {
        timestamp =
            std::chrono::high_resolution_clock::now()
                .time_since_epoch()
                .count();
    }
};