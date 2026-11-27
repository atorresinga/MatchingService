# MatchingService

A small C++ limit order book and matching engine, built as a learning project. The `mini-orderbook` directory contains the implementation, a baseline version to compare against, and the rules the book follows.

> **Status:** work in progress. The core data structures are in place, but matching, `print()`, and several edge cases are not finished yet (see [Roadmap](#roadmap)).

## Repository layout

```
MatchingService/
├── mini-orderbook/
│   ├── orderbook.cpp         # Main implementation (price levels + O(1) cancel)
│   ├── orderbook_naive.cpp   # Simpler baseline implementation for comparison
│   ├── rules.txt             # Matching rules / problem spec
│   └── orderbook             # Compiled binary (build output)
└── README.md
```

## How it works

The book keeps two sides of resting limit orders:

- **Bids** (buys) in a `std::map<int, PriceLevel, std::greater<int>>`, so `begin()` is always the highest bid.
- **Asks** (sells) in a `std::map<int, PriceLevel>`, so `begin()` is always the lowest ask.

Each `PriceLevel` holds a `std::list<Order*>` in arrival order, which gives **price-time priority**: better prices match first, and within the same price the earliest order matches first.

An `unordered_map<int, OrderLocation>` maps each order id to its side, price, and list iterator. This lets `cancel(id)` find and remove an order without scanning the book.

### Core types

| Type            | Purpose                                                              |
|-----------------|----------------------------------------------------------------------|
| `Side`          | `Buy` or `Sell`                                                      |
| `Order`         | id, side, price, quantity, and a tick (logical arrival timestamp)    |
| `Trade`         | buyer id, seller id, quantity, price, and tick of the match          |
| `PriceLevel`    | FIFO queue of orders resting at one price                            |
| `OrderLocation` | Where an order lives in the book, used for fast cancellation         |

### API

```cpp
std::vector<Trade> add(int id, Side side, int price, int qty);
void cancel(int id);
void print();
```

- `add` crosses the incoming order against the opposite side while prices overlap, emits a `Trade` for each fill (executed at the **resting order's price**), and rests any unfilled remainder in the book.
- `cancel` removes a resting order by id.
- `print` shows the resting buy and sell orders.

### Complexity (target)

| Operation          | Cost                                   |
|--------------------|----------------------------------------|
| Best bid / ask     | O(1)                                   |
| Add resting order  | O(log P), where P = number of price levels |
| Cancel by id       | O(1) lookup + O(log P) if the level empties |
| Match one fill     | O(1) per fill at the top of book       |

## Build and run

Requires a C++11 (or newer) compiler.

```bash
cd mini-orderbook
g++ -std=c++17 -O2 -Wall -Wextra orderbook.cpp -o orderbook
./orderbook
```

To build the baseline version:

```bash
g++ -std=c++17 -O2 -Wall -Wextra orderbook_naive.cpp -o orderbook_naive
./orderbook_naive
```

## Example

`main()` runs a short scenario:

```cpp
OrderBook book;
book.add(1, Side::Buy,  100, 10);
book.add(2, Side::Sell, 105, 5);
book.add(3, Side::Buy,  200, 10);  // crosses the ask at 105
book.add(4, Side::Buy,  150, 20);
book.add(5, Side::Sell,  80, 17);  // crosses resting bids
book.print();
```

## Roadmap

- [ ] Finish `add`: rest the remainder in the book, populate `order_lookup`, and return `trades`
- [ ] Guard the matching loop against an empty opposite side
- [ ] Fix the resting-order update to decrement `match_order->qty` (not the pointer), and remove filled orders and empty price levels
- [ ] Implement `print()` for both sides of the book
- [ ] Handle `cancel` for unknown ids
- [ ] Print the trades returned from `add`
- [ ] Add tests (partial fills, multi-level sweeps, FIFO at one price, cancels)
- [ ] Benchmark `orderbook.cpp` against `orderbook_naive.cpp`
- [ ] Manage order memory (owning storage instead of raw `Order*`)

## License

Not yet specified.