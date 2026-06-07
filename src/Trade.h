#pragma once

#include <string>
#include <chrono>

struct Trade {
    uint64_t buyOrderId;
    uint64_t sellOrderId;

    std::string symbol;

    int price;
    int quantity;

    uint64_t timestamp;

    Trade() = default;

    Trade(uint64_t buyId,
           uint64_t sellId,
           const std::string& sym,
           int p,
           int qty)
        : buyOrderId(buyId),
          sellOrderId(sellId),
          symbol(sym),
          price(p),
          quantity(qty)
    {
        timestamp =
            std::chrono::high_resolution_clock::now()
                .time_since_epoch()
                .count();
    }
};