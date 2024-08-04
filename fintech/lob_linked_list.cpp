#include <iostream>

namespace single_linked_list
{
struct Order
{
    double price;
    double quantity;
    int id;
    Order* next;

    Order(double price, double quantity, int id)
        : price(price), quantity(quantity), id(id), next(nullptr) {};
    Order(const Order& other)
    {
        price = other.price;
        quantity = other.quantity;
        id = other.id;
        next = other.next;
    };
    Order(Order&& other)
    {
        price = other.price;
        quantity = other.quantity;
        id = other.id;
        next = other.next;
        other.price = 0.0;
        other.quantity = 0.0;
        other.id = -1;
        other.next = nullptr;
    };
};

class LimitOrderBook
{
private:
    Order* head;
    Order* tail;
    int depth;
    int size;

public:
    LimitOrderBook(int depth)
        : depth(depth), size(0), head(nullptr), tail(nullptr) {};

    // O(N)
    void add_order(const Order& in_order)
    {
        Order* order = new Order(in_order);
        if (size == depth) {
            if (order->price <= head->price) {
                delete order;
                return;
            }
            else {
                Order* temp = head;
                head = head->next;
                delete temp;
                size--;
            }
        }
        if (!head || head->price > order->price) {
            order->next = head;
            head = order;
            if (!tail) {
                tail = head;
            }
        }
        else {
            auto* temp = head;
            while (temp->price < order->price && temp->next != nullptr) {
                temp = temp->next;
            }
            if (temp->next == nullptr) {
                temp->next = order;
                tail = order;
            }
            else {
                order->next = temp->next;
                temp->next = order;
            }
        }
        size++;
    }

    // O(N)
    void delete_order(const Order& order)
    {
        if (!head) {
            return;
        }
        if (head->price == order.price && order.quantity == head->quantity) {
            if (head->next) {
                Order* tmp = head;
                head = head->next;
                delete tmp;
            }
            else {
                Order* tmp = head;
                head = nullptr;
                delete tmp;
            }
            size--;
            return;
        }

        Order* tmp = head;
        while (tmp->next && tmp->next->price != order.price &&
               order.quantity != tmp->next->quantity) {
            tmp = tmp->next;
        }
        if (tmp->next) {
            Order* to_delete = tmp->next;
            if (to_delete == tail) {
                tail = tmp;
            }
            tmp->next = tmp->next->next;
            delete to_delete;
            size--;
            return;
        }
    }

    // Values in list store in ascendic order, tail has the biggerst price
    // O(1)
    double get_best_bid()
    {
        if (tail) {
            return tail->price;
        }
        return -1;
    }

    void print_bid()
    {
        if (!head) {
            return;
        }
        auto* temp = head;
        while (temp) {
            std::cout << "id: " << temp->id << " price: " << temp->price
                      << " quantity: " << temp->quantity << "\n";
            temp = temp->next;
        }
    }

    void print_ask();
    double get_best_ask();
};

}  // namespace single_linked_list

int test_bid()
{
    single_linked_list::LimitOrderBook lob(3);
    lob.add_order(single_linked_list::Order(10, 22, 0));
    lob.add_order(single_linked_list::Order(17, 11, 1));
    lob.add_order(single_linked_list::Order(8, 20, 3));
    lob.add_order(single_linked_list::Order(22, 78, 2));
    lob.print_bid();
    lob.get_best_bid();

    lob.delete_order(single_linked_list::Order(17, 11, 1));
    lob.delete_order(single_linked_list::Order(22, 78, 2));
    lob.delete_order(single_linked_list::Order(10, 22, 0));
    lob.print_bid();
    lob.get_best_bid();
}

int main()
{
    test_bid();
}
