#include <iostream>

enum class Side { Buy, Sell };

struct Order {
    int id;
    Side side;
    int price;
    int qty;
    long long tick;
};


int main() {
    Order o{1, Side::Buy, 100, 10, 5};
    std::cout << "order " << o.id << " price " << o.price << " qty " << o.qty << " ticks " << o.tick << "\n";
}