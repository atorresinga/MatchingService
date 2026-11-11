#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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

class OrderBook {
public:
    Order* priority (Order* x, Order* y) {
        if (x->price == y->price) {
            return (x->tick < y->tick ? x : y);
        }
        if (x->side == Side::Buy) {
            // when buying, greatest price is better
            return (x->price > y->price ? x : y);
        }
        else {
            // when selling, lowest price is better
            return (x->price < y->price ? x : y);
        }
    }
    void add(int id, Side side, int price, int qty) {
        while (qty > 0) {
            Order* matchingOrder = NULL;
            for (auto* o : orders_) {
                if (o->side == side) {
                    continue;
                }
                if (side == Side::Buy) {
                    if (o->price > price) {
                        continue;
                    } 
                }
                if (side == Side::Sell) {
                    if (o->price < price) {
                        continue;
                    }
                }
                if (matchingOrder == NULL) {
                    matchingOrder = o;
                }
                else {
                    matchingOrder = priority(matchingOrder, o);
                }
            }
            if (matchingOrder != NULL) {
                int setQty = std::min(matchingOrder->qty, qty);
                // matchingOrder update
                matchingOrder->qty -= setQty;
                if (matchingOrder->qty == 0) {
                    cancel(matchingOrder->id);
                }
                // incomming order update
                qty -= setQty;
            }
            else {
                break;
            }
        }
        if (qty != 0) {
            Order* o = new Order(id, side, price, qty, nextTick_++);
            orders_.push_back(o);
        }
    }
    void cancel(int id) {
        std::vector<Order*> upd_orders;
        for (auto* o : orders_) {
            if (o->id != id) {
                upd_orders.push_back(o);
            }
        }
        orders_ = upd_orders;
    }
    void print() {
        std::cout << nextTick_ << "\n";
        for (auto* o : orders_) {
            (*o).print();
        }
    }
private:
    std::vector<Order*> orders_;
    long long nextTick_ = 0;
};

int main() {
    OrderBook book;
    book.add(1, Side::Buy, 100, 10);
    book.print();
    book.add(2, Side::Sell, 105, 5);
    book.print();
    book.add(3, Side::Buy, 200, 10);
    book.print();
    book.add(4, Side::Buy, 150, 20);
    book.print();
    book.add(5, Side::Sell, 80, 17);
    book.print();
}