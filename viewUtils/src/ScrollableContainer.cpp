// This class implements ScrollableContainer.h, see header file for more info

#include "ScrollableContainer.h"
#include "InvestmentDisplayItem.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <qboxlayout.h>
#include <qnamespace.h>

ScrollableContainer::ScrollableContainer(CONTAINER_TYPE containerType)
    : containerType(containerType), pSelectedItem(nullptr) {

  this->setObjectName("ScrollableContainer");

  pContainerWidget = new QWidget();
  pContainerWidget->setObjectName("ScrollableContent");

  pLayout = new QVBoxLayout(pContainerWidget);

  pScrollArea = new QScrollArea();
  pScrollArea->setWidget(pContainerWidget);
  pScrollArea->setWidgetResizable(true);
  pScrollArea->setBackgroundRole(QPalette::Dark);
  pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  pMasterLayout = new QVBoxLayout(this);
  pMasterLayout->addWidget(pScrollArea);
}

ScrollableContainer::~ScrollableContainer() {}

void ScrollableContainer::addDisplayList(std::vector<Customer> customers) {

  for (Customer customer : customers) {
    float currentWorth = 0.0f;

    for (Investment investment : customer.investments) {
      currentWorth += investment.currentInvestmentWorth;
    }

    std::string customerDisplayName =
        customer.lastName + ", " + customer.firstName.at(0);

    CustomerDisplayItem *displayItem = new CustomerDisplayItem(
        QString::fromStdString(customerDisplayName),
        QString::number(customer.customerID),
        QString::number(currentWorth, 'f', 2),
        QString::number(customer.uninvestedFunds, 'f', 2));

    this->customerDisplayList.push_back(displayItem);
  }
  displayList();
}

void ScrollableContainer::addDisplayList(std::vector<Employee> employees) {

  for (Employee employee : employees) {

    std::string employeeDisplayName =
        employee.lastName + ", " + employee.firstName.at(0);

    TraderDisplayItem *displayItem =
        new TraderDisplayItem(QString::fromStdString(employeeDisplayName),
                              QString::number(employee.accountID),
                              QString::number(employee.numAccountsManaged));

    this->employeeDisplayList.push_back(displayItem);
  }

  displayList();
}

void ScrollableContainer::addDisplayList(std::vector<Investment> investments) {
  for (Investment investment : investments) {
    InvestmentDisplayItem *investmentDisplay = new InvestmentDisplayItem(
        investment.investmentID,
        QString::fromStdString(investment.stock.stockName),
        QString::fromStdString(investment.stock.stockCode),
        QString::number(investment.numHeld),
        QString::number(investment.stock.stockPrice, 'f', 2));

    this->investmentDisplayList.push_back(investmentDisplay);
  }

  displayList();
}

void ScrollableContainer::refreshDisplayList(std::vector<Customer> customers) {

  clearDisplay();
  for (Customer customer : customers) {
    float currentWorth = 0.0f;

    for (Investment investment : customer.investments) {
      currentWorth += investment.currentInvestmentWorth;
    }

    std::string customerDisplayName =
        customer.lastName + ", " + customer.firstName.at(0);

    CustomerDisplayItem *displayItem = new CustomerDisplayItem(
        QString::fromStdString(customerDisplayName),
        QString::number(customer.customerID), QString::number(currentWorth),
        QString::number(customer.uninvestedFunds));

    this->customerDisplayList.push_back(displayItem);
  }
  displayList();
}

void ScrollableContainer::refreshDisplayList(std::vector<Employee> employees) {
  clearDisplay();

  for (Employee employee : employees) {
    std::string numCustomersManagedDisplayText =
        std::to_string(employee.numAccountsManaged) + " accounts managed.";
    std::string employeeDisplayName =
        employee.lastName + ", " + employee.firstName.at(0);

    TraderDisplayItem *displayItem = new TraderDisplayItem(
        QString::fromStdString(employeeDisplayName),
        QString::number(employee.accountID),
        QString::fromStdString(numCustomersManagedDisplayText));

    this->employeeDisplayList.push_back(displayItem);
  }

  displayList();
}

void ScrollableContainer::refreshDisplayList(
    std::vector<Investment> investments) {
  clearDisplay();
  for (Investment investment : investments) {
    InvestmentDisplayItem *investmentDisplay = new InvestmentDisplayItem(
        investment.investmentID,
        QString::fromStdString(investment.stock.stockName),
        QString::fromStdString(investment.stock.stockCode),
        QString::number(investment.numHeld),
        QString::number(investment.stock.stockPrice));

    this->investmentDisplayList.push_back(investmentDisplay);
  }
  displayList();
}

// This is necessarcy to use style sheets.
void ScrollableContainer::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//****************************PRIVATE FUNCTIONS*******************************
void ScrollableContainer::clearDisplay() {
  pSelectedItem = nullptr;

  // We always take item 0 because as we remove one, the next one slides into
  // index 0.
  while (pLayout->count() > 1) {
    QLayoutItem *item = pLayout->takeAt(0);
    if (item->widget()) {
      delete item->widget();
    }
    // Finally, delete the QLayoutItem wrapper itself
    delete item;
  }

  employeeDisplayList.clear();
  customerDisplayList.clear();
  investmentDisplayList.clear();
}

void ScrollableContainer::displayList() {

  if (containerType == CUSTOMERS) {
    for (CustomerDisplayItem *displayItem : customerDisplayList) {
      pLayout->addWidget(displayItem);

      connect(displayItem, &CustomerDisplayItem::clicked, this,
              &ScrollableContainer::listenForCustomerItemSingleClick);
      connect(displayItem, &CustomerDisplayItem::doubleClicked, this,
              &ScrollableContainer::listenForCustomerItemDoubleClick);
    }
  } else if (containerType == TRADERS) {

    for (TraderDisplayItem *displayItem : employeeDisplayList) {

      pLayout->addWidget(displayItem);

      connect(displayItem, &TraderDisplayItem::clicked, this,
              &ScrollableContainer::listenForEmployeeItemSingleClick);
      connect(displayItem, &TraderDisplayItem::doubleClicked, this,
              &ScrollableContainer::listenForEmployeeItemDoubleClick);
    }
  } else {
    for (InvestmentDisplayItem *displayItem : investmentDisplayList) {
      pLayout->addWidget(displayItem);
      // TODO: stock display click connections
    }
  }
  pLayout->addStretch();
}

// Setting the selected property for the stylesheet
void ScrollableContainer::selectItem(QWidget *item) {
  if (item == pSelectedItem) {
    return;
  }

  if (pSelectedItem != nullptr) {
    pSelectedItem->setProperty("selected", false);
    pSelectedItem->style()->unpolish(pSelectedItem);
    pSelectedItem->style()->polish(pSelectedItem);
    pSelectedItem->update();
  }

  pSelectedItem = item;

  if (pSelectedItem != nullptr) {
    pSelectedItem->setProperty("selected", true);
    pSelectedItem->style()->unpolish(pSelectedItem);
    pSelectedItem->style()->polish(pSelectedItem);
    pSelectedItem->update();
  }
}

//****************************SLOTS*******************************************

void ScrollableContainer::listenForCustomerItemSingleClick(int id) {
  selectItem(qobject_cast<QWidget *>(sender()));
  emit notifyOfCustomerItemSingleClick(id);
}

// Called when a customer item has been double clicked
void ScrollableContainer::listenForCustomerItemDoubleClick(int id) {
  emit notifyOfCustomerItemDoubleClick(id);
}

void ScrollableContainer::listenForEmployeeItemSingleClick(int id) {
  selectItem(qobject_cast<QWidget *>(sender()));
  emit notifyOfEmployeeItemSingleClick(id);
}

// Called when an employee item has been double clicked
void ScrollableContainer::listenForEmployeeItemDoubleClick(int id) {
  emit notifyOfEmployeeItemDoubleClick(id);
}
