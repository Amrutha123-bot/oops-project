#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QHeaderView>
#include <vector>
#include <string>
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

class RestaurantGUI : public QWidget {
public:
    Menu menu;
    vector<Table> tables;
    Feedback<int> ratings;
    Feedback<string> comments;
    QTableWidget *menuTable;
    QPushButton *showMenuBtn;

    RestaurantGUI(QWidget *parent=nullptr): QWidget(parent) {
        setWindowTitle("Restaurant Management System");
        resize(850, 670);

        // Add menu items (no ID/number for display)
        menu.addItem(MenuItem("Pizza Margherita",9.99,"Main"));
        menu.addItem(MenuItem("Spaghetti Carbonara",8.49,"Main"));
        menu.addItem(MenuItem("Grilled Chicken",10.99,"Main"));
        menu.addItem(MenuItem("Paneer Tikka",7.79,"Main"));
        menu.addItem(MenuItem("French Fries",3.99,"Sides"));
        menu.addItem(MenuItem("Garlic Bread",2.99,"Sides"));
        menu.addItem(MenuItem("Chocolate Cake",5.49,"Desserts"));
        menu.addItem(MenuItem("Ice Cream",4.29,"Desserts"));
        menu.addItem(MenuItem("Coke",1.99,"Beverages"));
        menu.addItem(MenuItem("Orange Juice",2.49,"Beverages"));
        tables = {Table(1), Table(2), Table(3), Table(4), Table(5)};

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

        // Header
        QLabel *title = new QLabel("🍴 Welcome to Our Restaurant 🍕");
        QFont titleFont("Arial", 26, QFont::Bold);
        title->setFont(titleFont);
        title->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(title);

        // Show Menu Button
        showMenuBtn = new QPushButton("Show Menu");
        showMenuBtn->setToolTip("Click to view our mouth-watering menu!");
        mainLayout->addWidget(showMenuBtn, 0, Qt::AlignHCenter);

        // Menu Table: Name, Price, Type only (no number/id column)
        menuTable = new QTableWidget(menu.getSize(), 3, this);
        menuTable->setFixedWidth(500);
        menuTable->setHorizontalHeaderLabels({"Name", "Price ($)", "Type"});
        menuTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        menuTable->setSelectionMode(QAbstractItemView::NoSelection);
        QFont menuFont("Segoe UI", 14);
        menuTable->setFont(menuFont);
        menuTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        menuTable->setFixedHeight(270);
        for (int i = 0; i < menu.getSize(); i++) {
            QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(menu.getItem(i).name));
            nameItem->setTextAlignment(Qt::AlignCenter);
            menuTable->setItem(i, 0, nameItem);

            QTableWidgetItem *priceItem = new QTableWidgetItem(QString::number(menu.getItem(i).price, 'f', 2));
            priceItem->setTextAlignment(Qt::AlignCenter);
            menuTable->setItem(i, 1, priceItem);

            QTableWidgetItem *typeItem = new QTableWidgetItem(QString::fromStdString(menu.getItem(i).category));
            typeItem->setTextAlignment(Qt::AlignCenter);
            menuTable->setItem(i, 2, typeItem);
        }
        menuTable->hide();

        QHBoxLayout *tableRow = new QHBoxLayout;
        tableRow->addStretch();
        tableRow->addWidget(menuTable);
        tableRow->addStretch();
        mainLayout->addLayout(tableRow);

        // Buttons
        QHBoxLayout *buttonRow = new QHBoxLayout;
        QPushButton *reserveBtn = new QPushButton("🛎️ Reserve Table");
        QPushButton *onlineOrderBtn = new QPushButton("🍽️ Order Food");
        QPushButton *feedbackBtn = new QPushButton("🗨️ Give Feedback");
        reserveBtn->setToolTip("Book your table easily!");
        onlineOrderBtn->setToolTip("Order food for online delivery or dine-in.");
        feedbackBtn->setToolTip("Share your feedback to help us improve.");
        buttonRow->addStretch();
        buttonRow->addWidget(reserveBtn);
        buttonRow->addWidget(onlineOrderBtn);
        buttonRow->addWidget(feedbackBtn);
        buttonRow->addStretch();
        mainLayout->addLayout(buttonRow);

        // Stylesheet
        qApp->setStyleSheet(
            "QWidget { "
                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #232526, stop:1 #029fa7); "
                "color: #eafffc;"
            "}"
            "QLabel { color: #eafffc; font-weight:bold; font-size:22px; background: transparent; border:none; text-shadow: 0 0 8px #029fa7; }"
            "QPushButton { background: #232323; color: #29e3e6; border-radius: 18px; font-size:20px; font-family:'Segoe UI','Arial',sans-serif; font-weight:bold; min-width:182px; min-height:48px; padding-left:32px; padding-right:32px; margin:16px; border:none; box-shadow:0 8px 24px #029fa755; }"
            "QPushButton:hover { background: #029fa7; color: #232526; box-shadow:0 0 36px #029fa777; }"
            "QLineEdit { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #222831, stop:1 #029fa7); color: #eafffc; border-radius: 13px; font-size:20px; font-family: 'Segoe UI','Arial',sans-serif; font-weight:500; padding:14px 21px; border: 2px solid #029fa7; margin:8px; box-shadow: 0 2px 16px #029fa744; }"
            "QInputDialog, QMessageBox { color:#222; font-size:18px; font-family: 'Segoe UI','Arial',sans-serif; }"
            "QTableWidget { background: rgba(24,40,56,0.95); border-radius:24px; color:#29e3e6; border:none; box-shadow:0 8px 32px #029fa73a; font-weight:500; padding:8px; font-size:16px; }"
            "QHeaderView::section { background:#029fa7; color:#232526; font-size:19px; font-weight:bold; border:none; border-radius:14px; height:38px; }"
            "QTableWidget::item { background: transparent; border:none; font-size:16px; }"
        );

        // Show/Hide menu logic
        connect(showMenuBtn, &QPushButton::clicked, [=](){
            if (menuTable->isVisible()) {
                menuTable->hide();
                showMenuBtn->setText("Show Menu");
            } else {
                menuTable->show();
                showMenuBtn->setText("Hide Menu");
            }
        });

        // Reservation Dialog
        connect(reserveBtn, &QPushButton::clicked, [=](){
            QDialog dialog(this);
            dialog.setWindowTitle("Reserve Table");
            dialog.setFixedSize(580, 320);
            QVBoxLayout *layout = new QVBoxLayout(&dialog);
            QLabel *label = new QLabel("Enter reservation details");
            label->setAlignment(Qt::AlignCenter);
            QFont font("Arial", 22, QFont::Bold);
            label->setFont(font);
            layout->addWidget(label);

            QLineEdit *nameEdit = new QLineEdit();
            nameEdit->setPlaceholderText("Enter your name");
            nameEdit->setMinimumHeight(54);
            nameEdit->setFont(QFont("Segoe UI", 20));
            layout->addWidget(nameEdit);

            QLineEdit *tableEdit = new QLineEdit();
            tableEdit->setPlaceholderText("Table number (1-5)");
            tableEdit->setMinimumHeight(54);
            tableEdit->setFont(QFont("Segoe UI", 20));
            layout->addWidget(tableEdit);

            QPushButton *okBtn = new QPushButton("Reserve Now");
            okBtn->setMinimumHeight(52);
            layout->addWidget(okBtn);

            dialog.setLayout(layout);

            QObject::connect(okBtn, &QPushButton::clicked, [&](){
                QString name = nameEdit->text().trimmed();
                QString tableStr = tableEdit->text().trimmed();

                if (name.isEmpty()) {
                    QMessageBox::warning(&dialog, "Input Error", "Please enter your name.");
                    return;
                }
                bool ok;
                int tNum = tableStr.toInt(&ok);
                if (!ok || tNum < 1 || tNum > tables.size()) {
                    QMessageBox::warning(&dialog, "Input Error", "Enter table number from 1 to 5.");
                    return;
                }
                if (!tables[tNum-1].isReserved) {
                    tables[tNum-1].isReserved = true;
                    QMessageBox::information(this, "Success",
                        "Table " + QString::number(tNum) + " reserved for " + name);
                    dialog.accept();
                } else {
                    QMessageBox::warning(this, "Error", "Table is already reserved");
                }
            });
            dialog.exec();
        });

        // Order Dialog
        connect(onlineOrderBtn, &QPushButton::clicked, this, [=](){
            QDialog dialog(this);
            dialog.setWindowTitle("Order Food");
            dialog.setFixedSize(550, 470);
            QVBoxLayout *layout = new QVBoxLayout(&dialog);
            QLabel *label = new QLabel("Select items to order:");
            QFont font("Arial", 18, QFont::Bold);
            label->setFont(font);
            layout->addWidget(label);

            QListWidget *listWidget = new QListWidget();
            listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
            listWidget->setFont(QFont("Segoe UI",16));
            for(int i=0; i<menu.getSize(); i++) {
                QString itemText = QString::fromStdString(menu.getItem(i).name) +
                                   " ($" + QString::number(menu.getItem(i).price, 'f', 2) + ")";
                listWidget->addItem(new QListWidgetItem(itemText));
            }
            layout->addWidget(listWidget);

            QObject::connect(listWidget, &QListWidget::itemSelectionChanged, [=](){
                for(int i=0; i<listWidget->count(); ++i) {
                    QListWidgetItem *item = listWidget->item(i);
                    if(item->isSelected()) {
                        if(!item->text().contains("✅"))
                            item->setText("✅ " + QString::fromStdString(menu.getItem(i).name) +
                                " ($" + QString::number(menu.getItem(i).price, 'f', 2) + ")");
                    } else {
                        if(item->text().contains("✅"))
                            item->setText(QString::fromStdString(menu.getItem(i).name) +
                                " ($" + QString::number(menu.getItem(i).price, 'f', 2) + ")");
                    }
                }
            });

            QPushButton *okBtn = new QPushButton("Order Selected");
            okBtn->setMinimumHeight(52);
            layout->addWidget(okBtn);
            dialog.setLayout(layout);

            QObject::connect(okBtn, &QPushButton::clicked, [&](){
                QList<QListWidgetItem*> selected = listWidget->selectedItems();
                if(selected.isEmpty()) {
                    QMessageBox::warning(&dialog, "No Selection", "Please select at least one item.");
                    return;
                }
                vector<int> indices;
                for(QListWidgetItem* item : selected) {
                    int idx = listWidget->row(item);
                    indices.push_back(idx);
                }
                Order oOrder;
                oOrder.placeOrder(indices, menu);
                double bill = oOrder.calculateBill();

                QString summary = "<p>You have ordered:<br>";
                for(int idx : indices)
                    summary += QString::fromStdString(menu.getItem(idx).name) + "<br>";
                summary += "<br>Total Bill: $" + QString::number(bill, 'f', 2) + "</p>";

                QString offerMsg = "<p>Today's Offers:<br>"
                                   "• Buy 1 Get 1 Free on Garlic Bread ($2.99 extra)<br>"
                                   "• 20% off if you order both Cake and Ice Cream ($1.50 extra)<br>"
                                   "Would you like to include the offers?</p>";
                QMessageBox::StandardButton offerResult = QMessageBox::question(
                    this, "Offers", offerMsg,
                    QMessageBox::Yes | QMessageBox::No);

                double extraOffer = 0.0;
                if(offerResult == QMessageBox::Yes) {
                    extraOffer += 2.99;
                    extraOffer += 1.50;
                    bill += extraOffer;
                }

                QDialog billDialog(this);
                billDialog.setWindowTitle("Final Bill");
                billDialog.setFixedSize(400, 200);
                QVBoxLayout *billLayout = new QVBoxLayout(&billDialog);
                QLabel *finalLabel = new QLabel(
                    "<p>Your order:<br>" + summary +
                    "<br><b style='font-size:20px;'>Final Amount Due: $" + QString::number(bill, 'f', 2) + "</b></p>");
                finalLabel->setFont(QFont("Arial", 17, QFont::Bold));
                finalLabel->setAlignment(Qt::AlignCenter);
                finalLabel->setWordWrap(true);
                billLayout->addWidget(finalLabel);
                billDialog.exec();
                dialog.accept();
            });
            dialog.exec();
        });

        // Feedback Dialog
        connect(feedbackBtn, &QPushButton::clicked, [=](){
            QDialog dialog(this);
            dialog.setWindowTitle("Give Feedback");
            dialog.setFixedSize(540, 280);
            QVBoxLayout *layout = new QVBoxLayout(&dialog);
            QLabel *label = new QLabel("Rate us & leave a comment");
            label->setAlignment(Qt::AlignCenter);
            QFont font("Arial", 20, QFont::Bold);
            label->setFont(font);
            layout->addWidget(label);

            QLineEdit *ratingEdit = new QLineEdit();
            ratingEdit->setPlaceholderText("Rate (1-5)");
            ratingEdit->setMinimumHeight(50);
            ratingEdit->setFont(QFont("Segoe UI", 18));
            layout->addWidget(ratingEdit);

            QLineEdit *commentEdit = new QLineEdit();
            commentEdit->setPlaceholderText("Enter comment");
            commentEdit->setMinimumHeight(50);
            commentEdit->setFont(QFont("Segoe UI", 18));
            layout->addWidget(commentEdit);

            QPushButton *okBtn = new QPushButton("Submit Feedback");
            okBtn->setMinimumHeight(52);
            layout->addWidget(okBtn);

            dialog.setLayout(layout);

            QObject::connect(okBtn, &QPushButton::clicked, [&](){
                QString rateStr = ratingEdit->text().trimmed();
                QString comment = commentEdit->text().trimmed();

                bool ok;
                int rate = rateStr.toInt(&ok);
                if (!ok || rate < 1 || rate > 5) {
                    QMessageBox::warning(&dialog, "Input Error", "Please enter a rating from 1 to 5.");
                    return;
                }
                if(comment.isEmpty()) {
                    QMessageBox::warning(&dialog, "Input Error", "Please give some feedback!");
                    return;
                }
                ratings.add(rate);
                comments.add(comment.toStdString());
                QMessageBox::information(this, "Thanks", "Feedback recorded!");
                dialog.accept();
            });
            dialog.exec();
        });

        // Accessibility message at start
        QMessageBox::information(this, "Accessibility",
                                 "Tip:\n• Use Tab/Arrow keys to navigate.\n• Buttons are extra wide and readable.\n• Large text and color contrast for all input fields!");
    }
};

int main(int argc, char *argv[]){
    QApplication app(argc,argv);
    RestaurantGUI w;
    w.show();
    return app.exec();
}
