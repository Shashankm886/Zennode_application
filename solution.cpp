#include <iostream>
#include <bits/stdc++.h>
using namespace std;

unordered_map<char, int> product_prices = {
    {'A', 20},
    {'B', 40},
    {'C', 50}};

unordered_map<char, pair<int, bool>> product_info;

void update_product_info(char product_name, int quantity, bool gift_wrap)
{
    product_info[product_name] = make_pair(quantity, gift_wrap);
}

int calculate_subtotal()
{
    int subtotal = 0;
    for (const auto &info : product_info)
    {
        char product_name = info.first;
        int quantity = info.second.first;
        subtotal += product_prices[product_name] * quantity;
    }
    return subtotal;
}

pair<string, int> apply_discount_rules(int subtotal)
{
    unordered_map<string, int> discounts = {
        {"flat_10_discount", 10},
        {"bulk_5_discount", 0},
        {"bulk_10_discount", 0},
        {"tiered_50_discount", 0}};

    for (const auto &info : product_info)
    {
        char product_name = info.first;
        int quantity = info.second.first;
        if (quantity > 10)
        {
            discounts["bulk_5_discount"] = max(discounts["bulk_5_discount"], int(product_prices[product_name] * quantity * 0.05));
        }
    }

    int total_quantity = 0;
    for (const auto &info : product_info)
    {
        total_quantity += info.second.first;
    }
    if (total_quantity > 20)
    {
        discounts["bulk_10_discount"] = max(discounts["bulk_10_discount"], int(subtotal * 0.1));
    }

    bool tiered_discount_applicable = false;
    for (const auto &info : product_info)
    {
        char product_name = info.first;
        int quantity = info.second.first;
        if (quantity > 15)
        {
            tiered_discount_applicable = true;
            break;
        }
    }

    if (total_quantity > 30 && tiered_discount_applicable)
    {
        int excess_quantity_discount = 0;
        for (const auto &info : product_info)
        {
            char product_name = info.first;
            int quantity = info.second.first;
            if (quantity > 15)
            {
                excess_quantity_discount += (quantity - 15) * product_prices[product_name] * 0.5;
            }
        }
        discounts["tiered_50_discount"] = max(discounts["tiered_50_discount"], excess_quantity_discount);
    }

    int max_discount_amount = 0;
    string max_discount_name = "";
    for (const auto &discount : discounts)
    {
        if (discount.second > max_discount_amount)
        {
            max_discount_amount = discount.second;
            max_discount_name = discount.first;
        }
    }

    return make_pair(max_discount_name, max_discount_amount);
}

int calculate_shipping_fee()
{
    int total_quantity = 0;
    for (const auto &info : product_info)
    {
        total_quantity += info.second.first;
    }
    return (total_quantity / 10) * 5;
}

int calculate_gift_wrap_fee()
{
    int total_quantity = 0;
    for (const auto &info : product_info)
    {
        total_quantity += info.second.first;
    }
    return total_quantity;
}

int calculate_total(int subtotal, int discount_amount, int shipping_fee, int gift_wrap_fee)
{
    return subtotal - discount_amount + shipping_fee + gift_wrap_fee;
}

int main()
{
    for (char product_name = 'A'; product_name <= 'C'; product_name++)
    {
        int quantity;
        bool gift_wrap;

        cout << "Enter quantity for product " << product_name << ": ";
        cin >> quantity;

        cout << "Gift wrap for product " << product_name << "? (1 for yes, 0 for no): ";
        cin >> gift_wrap;

        update_product_info(product_name, quantity, gift_wrap);
    }

    int subtotal = calculate_subtotal(); // contains total amount without accounting for giftwrap, discount, or shipping

    pair<string, int> discount = apply_discount_rules(subtotal);
    int shipping_fee = calculate_shipping_fee();
    int gift_wrap_fee = calculate_gift_wrap_fee();
    int total = calculate_total(subtotal, discount.second, shipping_fee, gift_wrap_fee);

    if (subtotal > 200)
    {
        int temp_total = calculate_total(subtotal, 0, shipping_fee, gift_wrap_fee) - 10;
        total = min(temp_total, total);
        if (temp_total < total)
            discount.first = "flat_10_discount";
    }

    // Output
    cout << "Product Details:" << endl;
    for (const auto &info : product_info)
    {
        char product_name = info.first;
        int quantity = info.second.first;
        int total_amount = product_prices[product_name] * quantity;
        cout << "Product: " << product_name << ", Quantity: " << quantity << ", Total Amount: " << total_amount << endl;
    }

    cout << "\nSubtotal: " << subtotal << endl;

    if (!discount.first.empty())
    {
        cout << "Discount Applied: " << discount.first << endl;
        cout << "Discount Amount: " << discount.second << endl;
    }

    cout << "Shipping Fee: " << shipping_fee << endl;
    cout << "Gift Wrap Fee: " << gift_wrap_fee << endl;
    cout << "Total: " << total << endl;

    return 0;
}
