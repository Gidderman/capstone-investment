// This class implements ScrollableContainer.h, see header file for more info

#include "ScrollableContainer.h"
#include "InvestmentDisplayItem.h"

ScrollableContainer::ScrollableContainer(CONTAINER_TYPE containerType)
    : containerType(containerType) {
  pLayout = new QVBoxLayout(this);
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
        QString::number(customer.customerID), QString::number(currentWorth),
        QString::number(customer.uninvestedFunds));

    this->customerDisplayList.push_back(displayItem);
  }
  displayList();
}

void ScrollableContainer::addDisplayList(std::vector<Employee> employees) {

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

void ScrollableContainer::addDisplayList(std::vector<Investment> investments) {
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

//****************************PRIVATE FUNCTIONS*******************************
void ScrollableContainer::clearDisplay() {

  QLayoutItem *item;
  // We always take item 0 because as we remove one, the next one slides into
  // index 0.
  while ((item = pLayout->takeAt(0)) != nullptr) {

    if (item->layout()) {

      delete item->layout();

    } else if (item->widget()) {

      // It's a widget, so we delete it
      delete item->widget();
    }
  }

  // Finally, delete the QLayoutItem wrapper itself
  delete item;

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
}

//****************************SLOTS*******************************************

void ScrollableContainer::listenForCustomerItemSingleClick(int id) {
  emit notifyOfCustomerItemSingleClick(id);
}

// Called when a customer item has been double clicked
void ScrollableContainer::listenForCustomerItemDoubleClick(int id) {
  emit notifyOfCustomerItemDoubleClick(id);
}

void ScrollableContainer::listenForEmployeeItemSingleClick(int id) {
  emit notifyOfEmployeeItemSingleClick(id);
}

// Called when an employee item has been double clicked
void ScrollableContainer::listenForEmployeeItemDoubleClick(int id) {
  emit notifyOfEmployeeItemDoubleClick(id);
}
