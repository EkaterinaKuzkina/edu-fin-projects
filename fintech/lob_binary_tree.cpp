#include <cassert>
#include <iostream>
#include <set>

namespace binary_tree
{
struct Order
{
    int id;
    double price;
    double quantity;

    Order(int id, double price, double quantity)
        : id(id), price(price), quantity(quantity) {};

    // used for set ordering
    bool operator<(const Order& other) const { return price < other.price; }
};

class LimitOrderBook
{
    // set is implemented as a balanced binary tree
    std::set<Order> orders_bid;
    std::set<Order> orders_ask;
    int depth;

public:
    LimitOrderBook(int depth) : depth(depth) {};
    // O(log n)
    void add_order(const Order& order, bool is_bid)
    {
        if (is_bid) {
            if (orders_bid.size() < depth) {
                orders_bid.insert(order);
            }
            // if price of new order is higher then lowest price in lob
            // delete lowest price and insert new
            else if (order.price > orders_bid.begin()->price) {
                orders_bid.erase(orders_bid.begin());
                orders_bid.insert(order);
            }
        }
        else {
            if (orders_ask.size() < depth) {
                orders_ask.insert(order);
            }
            // if price of new order is lower then highest price in lob
            // delete highest price and insert new
            else if (order.price < orders_bid.rbegin()->price) {
                orders_ask.erase(--orders_ask.end());
                orders_ask.insert(order);
            }
        }
    }

    // O(log n)
    void delete_order(const Order& order, bool is_bid)
    {
        if (is_bid) {
            if (orders_bid.find(order) != orders_bid.end()) {
                orders_bid.erase(order);
            }
        }
        else {
            if (orders_ask.find(order) != orders_ask.end()) {
                orders_ask.erase(order);
            }
        }
    }

    // Max price is the best - we want somebody who is willing to pay the higherst price
    // O(1)
    double get_best_bid()
    {
        if (orders_bid.size()) {
            return orders_bid.rbegin()->price;
        }
        return 0.0;
    }

    // Min price is the best - we want somebody who is selling by the lowest price
    // O(1)
    double get_best_ask()
    {
        if (orders_ask.size()) {
            return orders_ask.begin()->price;
        }
        return 0.0;
    }

    void print_bids()
    {
        for (const auto& order : orders_bid) {
            std::cout << "id: " << order.id << " price: " << order.price
                      << " quantity: " << order.quantity << "\n";
        }
    }

    void print_asks()
    {
        for (const auto& order : orders_ask) {
            std::cout << "id: " << order.id << " price: " << order.price
                      << " quantity: " << order.quantity << "\n";
        }
    }
};
}  // namespace binary_tree

void test_bid()
{
    std::cout << "Testing bid \n";
    bool is_bid = true;
    binary_tree::LimitOrderBook lob(4);
    assert(lob.get_best_bid() == 0.0);
    lob.add_order(binary_tree::Order(0, 100.0, 105), is_bid);
    lob.add_order(binary_tree::Order(3, 101.0, 106), is_bid);
    lob.add_order(binary_tree::Order(2, 98.0, 96), is_bid);
    lob.add_order(binary_tree::Order(1, 105.5, 102), is_bid);

    std::cout << "best bid " << lob.get_best_bid() << "\n";
    lob.add_order(binary_tree::Order(9, 108.1, 102), is_bid);
    std::cout << "best bid " << lob.get_best_bid() << "\n";
    lob.print_bids();
    lob.delete_order(binary_tree::Order(9, 108.1, 102), is_bid);
    std::cout << "best bid " << lob.get_best_bid() << "\n";
}

void test_ask()
{
    std::cout << "Testing ask \n";
    bool is_bid = false;
    binary_tree::LimitOrderBook lob(4);
    assert(lob.get_best_ask() == 0.0);

    lob.add_order(binary_tree::Order(0, 100.0, 105), is_bid);
    lob.add_order(binary_tree::Order(3, 101.0, 106), is_bid);
    lob.add_order(binary_tree::Order(2, 98.0, 96), is_bid);
    lob.add_order(binary_tree::Order(1, 105.5, 102), is_bid);

    std::cout << "best ask " << lob.get_best_ask() << "\n";
    lob.add_order(binary_tree::Order(9, 97.1, 102), is_bid);
    std::cout << "best ask " << lob.get_best_ask() << "\n";
    lob.print_asks();
    lob.delete_order(binary_tree::Order(9, 97.1, 102), is_bid);
    std::cout << "best ask " << lob.get_best_ask() << "\n";
}

int main()
{
    test_bid();
    test_ask();
}
