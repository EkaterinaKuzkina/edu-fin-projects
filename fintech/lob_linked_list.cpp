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

    void add_order(const Order& in_order)
    {
        Order* order = new Order(in_order);
        if (size == depth) {
            if (order->price <= head->price) {
                delete order;
                return;
            }
            else {
                auto& temp = head;
                head = head->next;
                delete head;
                size--;
            }
        }
        if (!head || head->price > order->price) {
            order->next = head;
            head = order;
            if (!tail) {
                tail = order;
            }
        }
        else {
            auto temp = head;
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

    void delete_order(Order order);
    double get_best_bid();
    void print_bid()
    {
        auto& temp = head;
        while (temp)
        {   
            std::cout << "id: " << temp->id << " price: " << temp->price << " quantity: " << temp->quantity << "\n";
            temp = temp->next;
        }
    }
};

}  // namespace single_linked_list

int main()
{
    single_linked_list::LimitOrderBook lob(4);
    lob.add_order(single_linked_list::Order(10, 22, 0));
    lob.add_order(single_linked_list::Order(17, 11, 1));
    lob.add_order(single_linked_list::Order(8, 20, 3));
    lob.add_order(single_linked_list::Order(22, 78, 2));

    lob.print_bid();
}
