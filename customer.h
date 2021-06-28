#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <ctime>
#include "menu.h"

using namespace std;

class customer
{
    string name;
    bool emailProvided;
    string email;
    unordered_map<int, int> order;
public:
    customer(string name, bool emailProvidedByCustomer, string email)
    {
        this->name = name;
        if (emailProvided)
        {
            this->emailProvided = emailProvidedByCustomer;
            this->email = email;
        }
    }
    void addItemToOrder(int item_id, int qty, map<int, food_item> &menu)
    {
        if (qty <= menu[item_id].getStock())
        {
            menu[item_id].decreaseStock(qty);
            order.insert({item_id, qty});
        }
        else
            cout << "Not enough stock." << endl;
    }
    void copyOrderMap(unordered_map<int, int> currentOrder)
    {
        this->order = currentOrder;
    }
    string createBill(map<int, food_item> menu)
    {
        ofstream bill;
        time_t t = time(NULL);
        tm* tPtr = localtime(&t);
        int dd = tPtr->tm_mday;
        string day = (dd > 9) ? to_string(dd) : ("0" + to_string(dd));
        string billID = to_string(tPtr->tm_year + 1900) + "-" +
                        to_string(tPtr->tm_mon + 1) + "-" + day +
                        "_" + to_string(tPtr->tm_hour) + "-" +
                        to_string(tPtr->tm_min) + "_" + name;
        bill.open("/home/ayaan/.restaurant_pos/bills/" + billID + ".txt"); // PATH
        //bill << setw(45) << "############## Restaurant ABC ##############" << endl;
        bill << "***************************** Restaurant ABC *****************************" << endl << endl;
        bill << "Customer Name: " << name << endl;
        if (emailProvided)
        {
            bill << "Email: " << email << endl;
        }

        bill << "-------------------------------------------------------------------------";
        bill << endl;
        bill << setw(7) << "| S.No. " << left << setw(11) << "| Item ID"
            << setw(21) << "| Name "
            << setw(10) << "| Category "
            << setw(5) << "| Cost" << setw(6) << "| Qty " << "|" << setw(9) << "Price" << "|" << endl;

        bill << "|-------|----------|--------------------|----------|-----|-----|---------|";
        bill << endl;
        int i = 1;
        float total = 0.0;
        for (auto item : order)
        {
            bill << "|" << right << setw(7) << i;
            menu[item.first].addToBill(bill);
            bill << setw(5) << item.second;
            int itemPrice = menu[item.first].getCost() * item.second;
            total += itemPrice;
            bill << "|" << setw(9) << itemPrice << "|" << endl;
            i++;
        }
        bill << "-------------------------------------------------------------------------";
        bill << endl;
        bill << "                                                          Total:" << setw(9) << total << endl;
        bill << "                                                     SGST(2.5%):" << setw(9) << 0.025 * total << endl;
        bill << "                                                     CGST(2.5%):" << setw(9) << 0.025 * total << endl;
        total = 1.05 * total; // After GST
        bill << "                                               Total (inc. tax):" << setw(9) << total << endl;
        bill << "-------------------------------------------------------------------------";
        bill << endl;
        bill << "                             ***End of Bill***                             ";

        return billID;
    }
};
#endif // CUSTOMER_H
