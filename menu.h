#ifndef MENU_H
#define MENU_H
#include <QString>
#include <QTextStream>
#include <QTextStreamManipulator>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include "rapidcsv.h"

using namespace std;

const vector<int> item_ids({101, 102, 103, 104, 105, 201, 202, 203, 204, 205,
                        301, 302, 303, 304, 305, 306, 307, 308, 309});
class food_item
{
    int item_id;
    string name;
    string category;
    float costPerUnit;
    float margin;
    int stock;
public:
    food_item()
    {
        cout << "DEFAULT" << endl;
    }
    food_item(int id, vector<string> values_from_csv)
    {
        // cout << "Argument Constructor" << endl;
        // cout << "stoi argument: " << values_from_csv[0] << endl;
        item_id = id;
        name = values_from_csv[0];
        category = values_from_csv[1];
        costPerUnit = (float) stoi(values_from_csv[2]);
        margin = (float) stoi(values_from_csv[3]);
        stock = stoi(values_from_csv[4]);
    }
    QString getName()
    {
        return QString::fromStdString(name);
    }
    int getCost()
    {
        return (int)(costPerUnit * (1 + margin/100));
    }
    int getStock()
    {
        return stock;
    }
    void decreaseStock(int quantity)
    {
        stock = stock - quantity;
    }
    QString display()
    {
        int price = costPerUnit * (1 + margin/100);
        QString itemDetails;
        itemDetails.append("<tr>");
        itemDetails.append("<td align=\"center\">");
        itemDetails.append(QString::number(item_id, 10));
        itemDetails.append("<td>");
        itemDetails.append(QString::fromStdString(name));
        itemDetails.append("<td align=\"center\">");
        itemDetails.append(QString::fromStdString(category));
        itemDetails.append("<td align=\"right\">");
        itemDetails.append(QString::number(price, 10));
        itemDetails.append("</tr>");
        return itemDetails;
    }
    void addToBill(ofstream &bill)
    {
        int price = costPerUnit * (1 + margin/100);
        bill << right << "|" << setw(10)  << item_id << "|"
            << left << setw(20) << name << "|"
            << setw(10) << category << "|"
            << right << setw(5) << price << "|";
    }
};

map<int, food_item> loadMenu()
{
    rapidcsv::Document doc("/home/ayaan/.restaurant_pos/food_items.csv", rapidcsv::LabelParams(0, 0));
    std::clog << "CSV Loaded" << endl;
    map<int, food_item> menu;
    for (auto id : item_ids)
    {
        // cout << id << endl;
        string row_id = to_string(id);
        vector<string> item = doc.GetRow<string>(row_id);
        food_item f(id, item);
        menu.insert({id, f});
    }
    return menu;
}

QString displayMenu(map<int, food_item> menu)
{
    QString menuString;
    menuString.append("<table border=0.5 cellpadding=1.5 width=\"100%\">");
    menuString.append("<tr>");
    menuString.append("<th>Item ID");
    menuString.append("<th width=\"50%\">Name");
    menuString.append("<th>Category");
    menuString.append("<th>Price");
    menuString.append("</tr>");

    for (auto id : item_ids)
    {
        menuString += menu[id].display();
    }
    menuString.append("</table>");

    return menuString;
}

#endif // MENU_H
