// This class implements ScrollableContainer.h, see header file for more info

#include "ScrollableContainer.h"
#include "Investment.h"
#include "InvestmentDisplayItem.h"
#include "QuickSort.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <qboxlayout.h>
#include <qlineedit.h>
#include <qnamespace.h>

ScrollableContainer::ScrollableContainer(CONTAINER_TYPE containerType)
    : containerType(containerType), pSelectedItem(nullptr) {

  this->setObjectName("ScrollableContainer");

  pSearchBar = new QLineEdit();
  pSearchBar->setPlaceholderText("Search for a name...");

  pSortLabel = new QLabel("Sort By:");
  pSortLabel->setObjectName("ScrollableContainerLabel");

  pSortSelection = new QComboBox();
  if (containerType == CUSTOMERS) {
    pSortSelection->addItem("Name");
    pSortSelection->addItem("Name (reverse order)");
    pSortSelection->addItem("Customer ID");
    pSortSelection->addItem("Customer ID (reverse order)");
    pSortSelection->addItem("Available Funds");
    pSortSelection->addItem("Available Funds (reverse order)");
  } else if (containerType == TRADERS) {
    pSortSelection->addItem("Name");
    pSortSelection->addItem("Name (reverse order)");
    pSortSelection->addItem("Employee ID");
    pSortSelection->addItem("Employee ID (reverse order)");
    pSortSelection->addItem("Number of Managed Accounts");
    pSortSelection->addItem("Number of Managed Accounts (reverse order)");
  } else {
    pSortSelection->addItem("Investment Worth");
    pSortSelection->addItem("Investment Worth (reverse order)");
  }

  connect(pSearchBar, &QLineEdit::textChanged, this,
          &ScrollableContainer::listenForFilterBySearch);
  connect(pSortSelection, &QComboBox::currentIndexChanged, this,
          &ScrollableContainer::reorderBySortSelection);

  pContainerWidget = new QWidget();
  pContainerWidget->setObjectName("ScrollableContent");

  pLayout = new QVBoxLayout(pContainerWidget);

  pScrollArea = new QScrollArea();
  pScrollArea->setWidget(pContainerWidget);
  pScrollArea->setWidgetResizable(true);
  pScrollArea->setBackgroundRole(QPalette::Dark);
  pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  pSortLayout = new QHBoxLayout();
  pSortLayout->addWidget(pSortLabel, 0, Qt::AlignRight);
  pSortLayout->addWidget(pSortSelection, 0, Qt::AlignLeft);
  pSortLayout->addWidget(pSearchBar, 5, Qt::AlignLeft);

  pMasterLayout = new QVBoxLayout(this);
  pMasterLayout->addLayout(pSortLayout, 0);
  pMasterLayout->addWidget(pScrollArea);
}

ScrollableContainer::~ScrollableContainer() {}

void ScrollableContainer::addDisplayList(std::vector<Customer> customers) {
  customerList = customers;

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
  employeeList = employees;

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
  investmentList = investments;

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
  customerList = customers;

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
        QString::number(customer.customerID),
        QString::number(currentWorth, 'f', 2),
        QString::number(customer.uninvestedFunds, 'f', 2));

    this->customerDisplayList.push_back(displayItem);
  }
  displayList();
}

void ScrollableContainer::refreshDisplayList(std::vector<Employee> employees) {
  employeeList = employees;

  clearDisplay();

  for (Employee employee : employees) {
    std::string numCustomersManagedDisplayText =
        std::to_string(employee.numAccountsManaged);
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
  investmentList = investments;

  clearDisplay();
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
  while (pLayout->count() >= 1) {
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

void ScrollableContainer::listenForFilterBySearch() {
  if (containerType == CUSTOMERS) {
    for (CustomerDisplayItem *displayItem : customerDisplayList) {
      displayItem->hide();
    }

    for (unsigned int i = 0; i < customerList.size(); i++) {
      if (QString::fromStdString(customerList.at(i).firstName)
              .contains(pSearchBar->text(), Qt::CaseInsensitive) ||
          QString::fromStdString(customerList.at(i).lastName)
              .contains(pSearchBar->text(), Qt::CaseInsensitive)) {
        customerDisplayList.at(i)->show();
      }
    }

  } else if (containerType == TRADERS) {
    for (TraderDisplayItem *displayItem : employeeDisplayList) {
      displayItem->hide();
    }

    for (unsigned int i = 0; i < employeeList.size(); i++) {
      if (QString::fromStdString(employeeList.at(i).firstName)
              .contains(pSearchBar->text(), Qt::CaseInsensitive) ||
          QString::fromStdString(employeeList.at(i).lastName)
              .contains(pSearchBar->text(), Qt::CaseInsensitive)) {
        employeeDisplayList.at(i)->show();
      }
    }
  } else {
    for (InvestmentDisplayItem *displayItem : investmentDisplayList) {
      displayItem->hide();
    }

    for (unsigned int i = 0; i < investmentList.size(); i++) {
      if (QString::fromStdString(investmentList.at(i).stock.stockName)
              .contains(pSearchBar->text(), Qt::CaseInsensitive) ||
          QString::fromStdString(investmentList.at(i).stock.stockCode)
              .contains(pSearchBar->text(), Qt::CaseInsensitive)) {
        investmentDisplayList.at(i)->show();
      }
    }
  }
}

void ScrollableContainer::reorderBySortSelection() {
  if (containerType == CUSTOMERS) {
    switch (pSortSelection->currentIndex()) {
    case 0:
      Custom::quickSort(customerList, Custom::customerName);
      break;
    case 1:
      Custom::quickSort(customerList, Custom::reverseCustomerName);
      break;
    case 2:
      Custom::quickSort(customerList, Custom::customerId);
      break;
    case 3:
      Custom::quickSort(customerList, Custom::reverseCustomerId);
      break;
    case 4:
      Custom::quickSort(customerList, Custom::uninvestedFunds);
      break;
    case 5:
      Custom::quickSort(customerList, Custom::reverseUninvestedFunds);
      break;
    default:
      Custom::quickSort(customerList, Custom::customerName);
      break;
    }

    refreshDisplayList(customerList);
    listenForFilterBySearch();
  } else if (containerType == TRADERS) {
    switch (pSortSelection->currentIndex()) {
    case 0:
      Custom::quickSort(employeeList, Custom::employeeName);
      break;
    case 1:
      Custom::quickSort(employeeList, Custom::reverseEmployeeName);
      break;
    case 2:
      Custom::quickSort(employeeList, Custom::employeeId);
      break;
    case 3:
      Custom::quickSort(employeeList, Custom::reverseEmployeeId);
      break;
    case 4:
      Custom::quickSort(employeeList, Custom::numAccountsManaged);
      break;
    case 5:
      Custom::quickSort(employeeList, Custom::reverseNumAccountsManaged);
      break;
    default:
      Custom::quickSort(employeeList, Custom::employeeName);
      break;
    }

    refreshDisplayList(employeeList);
    listenForFilterBySearch();
  } else {
    switch (pSortSelection->currentIndex()) {
    case 0:
      Custom::quickSort(investmentList, Custom::investmentWorth);
      break;
    case 1:
      Custom::quickSort(investmentList, Custom::reverseInvestmentWorth);
      break;
    default:
      Custom::quickSort(investmentList, Custom::investmentWorth);
      break;
    }

    refreshDisplayList(investmentList);
    listenForFilterBySearch();
  }
}
