#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

// ===== Backend Classes =====
class MenuItem {
public:
    string name;
    double price;
    string category;
    MenuItem(string n, double p, string c): name(n), price(p), category(c) {}
};

class Menu {
public:
    vector<MenuItem> items;
    void addItem(MenuItem item) { items.push_back(item); }
    MenuItem getItem(int index) { return items[index]; }
    int getSize() { return items.size(); }
};

class Table {
public:
    int tableNumber;
    bool isReserved;
    Table(int t): tableNumber(t), isReserved(false) {}
};

class Order {
public:
    vector<MenuItem> orderItems;
    void placeOrder(vector<int> indices, Menu &menu){
        orderItems.clear();
        for(int idx : indices) orderItems.push_back(menu.getItem(idx));
    }
    double calculateBill() {
        double total = 0;
        for(auto &item : orderItems) total += item.price;
        return total;
    }
};

template<typename T>
class Feedback {
public:
    vector<T> feedbacks;
    void add(T f) { feedbacks.push_back(f); }
};

// ===== GUI Class =====
class RestaurantGUI : public QWidget {
public:
    Menu menu;
    vector<Table> tables;
    Feedback<int> ratings;
    Feedback<string> comments;

    RestaurantGUI(QWidget *parent=nullptr): QWidget(parent) {
        setWindowTitle("Restaurant Management System");

        // Add menu items
        menu.addItem(MenuItem("Pizza",8.99,"Main"));
        menu.addItem(MenuItem("Burger",5.49,"Main"));
        menu.addItem(MenuItem("Coke",1.99,"Beverage"));

        tables = {Table(1), Table(2), Table(3)};

        // Layout
        QVBoxLayout *layout = new QVBoxLayout(this);

        QTableWidget *menuTable = new QTableWidget(menu.getSize(),3);
        menuTable->setHorizontalHeaderLabels({"Item","Price","Category"});
        for(int i=0;i<menu.getSize();i++){
            menuTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(menu.getItem(i).name)));
            menuTable->setItem(i,1,new QTableWidgetItem(QString::number(menu.getItem(i).price)));
            menuTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(menu.getItem(i).category)));
        }
        layout->addWidget(menuTable);

        QPushButton *reserveBtn = new QPushButton("Reserve Table");
        QPushButton *onlineOrderBtn = new QPushButton("Online Order");
        QPushButton *feedbackBtn = new QPushButton("Give Feedback");
        layout->addWidget(reserveBtn);
        layout->addWidget(onlineOrderBtn);
        layout->addWidget(feedbackBtn);

        // Reserve Table
        connect(reserveBtn,&QPushButton::clicked,[=](){
            bool ok;
            QString name = QInputDialog::getText(this,"Reserve Table","Enter name:",QLineEdit::Normal,"",&ok);
            if(ok){
                int tNum = QInputDialog::getInt(this,"Reserve Table","Enter table number:",1,1,tables.size(),1,&ok);
                if(ok){
                    if(!tables[tNum-1].isReserved){
                        tables[tNum-1].isReserved = true;
                        QMessageBox::information(this,"Success","Table "+QString::number(tNum)+" reserved for "+name);
                    } else QMessageBox::warning(this,"Error","Table already reserved");
                }
            }
        });

        // Online Order
        connect(onlineOrderBtn,&QPushButton::clicked,[=](){
            vector<int> selected = {0,1}; // example, user selects first 2 items
            Order oOrder;
            oOrder.placeOrder(selected,menu);
            double bill = oOrder.calculateBill();
            QMessageBox::information(this,"Order Summary","Total Bill: $"+QString::number(bill));
        });

        // Feedback
        connect(feedbackBtn,&QPushButton::clicked,[=](){
            int rate = QInputDialog::getInt(this,"Feedback","Rate 1-5:",5,1,5,1);
            ratings.add(rate);
            QString comment = QInputDialog::getText(this,"Feedback","Enter comment:");
            comments.add(comment.toStdString());
            QMessageBox::information(this,"Thanks","Feedback recorded!");
        });
    }
};

// ===== Main =====
int main(int argc, char *argv[]){
    QApplication app(argc,argv);
    RestaurantGUI w;
    w.show();
    return app.exec();
}
