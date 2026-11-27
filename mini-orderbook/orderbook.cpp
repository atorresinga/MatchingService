#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <list>
#include <map>
#include <unordered_map>

enum class Side { Buy, Sell };

std::string toString(Side s) {
    return (s == Side::Buy ? "Buy" : "Sell");
}

struct Order {
    int id;
    Side side;
    int price;
    int qty;
    long long tick;
    
    Order() : id(0), side(Side::Sell), price(0), qty(0), tick(0) {}
    
    Order(int id_, Side side_, int price_, int qty_, long long tick_)
        : id(id_), side(side_), price(price_), qty(qty_), tick(tick_) {}
    
    void print() const {
        std::cout << "order " << id << " side " << toString(side) << " price " << price << " qty " << qty << " ticks " << tick << "\n";
    }
};

struct Trade {
    int buySideId;
    int sellSideId;
    int qty;
    int price;
    long long tick;
    Trade() : buySideId(-1), sellSideId(-1), qty(0), price(0), tick(0) {}
    
    Trade(int buySideId_, int sellSideId_, int qty_, int price_, long long tick_)
        : buySideId(buySideId_), sellSideId(sellSideId_), qty(qty_), price(price_),
        tick(tick_) {}
    void print() const {
        std::cout << "Buyer id " << buySideId << " Seller id " << sellSideId << " qty " << qty << " price " << price << " ticks " << tick << "\n";
    }
};

// the list will keep the orders ordered by tick time
struct PriceLevel {
    int price;
    std::list<Order*> orders;
};

struct OrderLocation {
    Side side;
    int price;
    std::list<Order*>::iterator it;
};

class OrderBook {
public:
    
    std::vector<Trade> add(int id, Side side, int price, int qty) {
        long long tick = curTick++;
        std::vector<Trade> trades;
        // look out for matches
        while (qty > 0) {
            auto it = (side == Side::Buy ? asks.begin() : bids.begin());
            int match_price = it->first;
            Order* match_order = (it->second).orders.front();
            if (side == Side::Buy) {
                if (match_price > price) {
                    break;
                }
            }
            else {
                if (price > match_price) {
                    break;
                }
            }
            // record trade
            int trade_qty = std::min(match_order->qty, qty);
            int trade_price = match_price;
            int buyId, sellId;
            if (side == Side::Buy) {
                buyId = id, sellId = match_order->id;
            }
            else {
                buyId = match_order->id, sellId = id;
            }
            trades.push_back(Trade(buyId, sellId, trade_qty, trade_price, tick));
            
            // update incomming order
            qty -= trade_qty;
            // update resting order
            match_order -= trade_qty;
            if (match_order == 0) {
                cancel(match_order->id);
            }
        }
        
        if (qty > 0) {
            if (side == Side::Buy) {
                bids[price]
            }
        }
    }
    void cancel(int id) {
        // TO DO
        Side side = order_lookup[id].side;
        auto it = order_lookup[id].it;
        int price = order_lookup[id].price;
        if (side == Side::Buy) {
            bids[price].orders.erase(it);
        }
        else {
            asks[price].orders.erase(it);
        }

    }
    void print() {
        
        std::cout << "Resting Buying Orders:\n";

        std::cout << "Resting Selling Orders\n";

    }
private:
    long long curTick = 0;
    std::map<int, PriceLevel, std::greater<int>> bids;
    std::map<int, PriceLevel> asks;
    std::unordered_map<int, OrderLocation> order_lookup;
};

int main() {
    OrderBook book;
    book.add(1, Side::Buy, 100, 10);
    book.add(2, Side::Sell, 105, 5);
    book.add(3, Side::Buy, 200, 10);
    book.add(4, Side::Buy, 150, 20);
    book.add(5, Side::Sell, 80, 17);
    book.print();
}