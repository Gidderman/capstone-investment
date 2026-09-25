// This class implements ScrollableContainer.h, see header file for more info

// TODO:
// THIS CLASS IS NOT YET FINISHED, REQUIRING FORMATING AND SCROLL FUNCTIONALITY.
// Additionally considering turning this into a template class

#include "ScrollableContainer.h"
#include "InvestmentDisplayItem.h"

#include <iostream>

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
    int numCustomersManaged = 0;

    std::string numCustomersManagedDisplayText =
        std::to_string(numCustomersManaged) + " accounts managed.";
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
        QString::fromStdString(investment.stock.stockName),
        QString::fromStdString(investment.stock.stockCode),
        QString::number(investment.numHeld),
        QString::number(investment.stock.stockPrice));

    this->investmentDisplayList.push_back(investmentDisplay);
  }

  displayList();
}

void ScrollableContainer::refreshDisplayList(std::vector<Customer> customers) {
  std::cout << "ScrollableContainer::refreshDisplayList - entering"
            << std::endl;

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

  std::cout << "ScrollableContainer::refreshDisplayList - exiting" << std::endl;
}

void ScrollableContainer::refreshDisplayList(std::vector<Employee> employees) {
  clearDisplay();

  for (Employee employee : employees) {
    int numCustomersManaged = 0;

    std::string numCustomersManagedDisplayText =
        std::to_string(numCustomersManaged) + " accounts managed.";
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
  std::cout << "ScrollableContainer::clearDisplay - entering" << std::endl;

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

  std::cout << "ScrollableContainer::clearDisplay - exiting" << std::endl;
}

void ScrollableContainer::displayList() {
  std::cout << "ScrollableContainer::displayList - entering" << std::endl;

  if (containerType == CUSTOMERS) {
    for (CustomerDisplayItem *displayItem : customerDisplayList) {
      pLayout->addWidget(displayItem);

      connect(displayItem, &CustomerDisplayItem::clicked, this,
              &ScrollableContainer::listenForCustomerItemSelection);
    }
  } else if (containerType == TRADERS) {

    for (TraderDisplayItem *displayItem : employeeDisplayList) {

      pLayout->addWidget(displayItem);

      connect(displayItem, &TraderDisplayItem::clicked, this,
              &ScrollableContainer::listenForEmployeeItemSelection);
    }
  } else {
    for (InvestmentDisplayItem *displayItem : investmentDisplayList) {
      pLayout->addWidget(displayItem);
      // TODO: stock display click connections
    }
  }

  std::cout << "ScrollableContainer::displayList - exiting" << std::endl;
}

//****************************SLOTS*******************************************
// Called when a customer item has been double clicked
void ScrollableContainer::listenForCustomerItemSelection(int id) {
  emit notifyOfCustomerItemSelection(id);
}

// Called when an employee item has been double clicked
void ScrollableContainer::listenForEmployeeItemSelection(int id) {
  emit notifyOfEmployeeItemSelection(id);
}
