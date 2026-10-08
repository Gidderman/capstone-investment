// This class implements AdminMainView.h, see that header file for more
// information

#include "AdminMainView.h"
#include "ScrollableContainer.h"

#include <QPainter>
#include <QString>
#include <QStyle>
#include <QStyleOption>
#include <qnamespace.h>

AdminMainView::AdminMainView(std::vector<Customer> totalCustomers,
                             std::vector<Employee> totalTraders) {
  this->setObjectName("AdminMainView"); // Set for QSS operations

  this->setMinimumSize(1024, 640);

  QSize MIN_BUTTON_SIZE = {185, 30};

  // Add the customers to the scrollable container for display
  pCustomerList = new ScrollableContainer(CUSTOMERS);
  pCustomerList->addDisplayList(totalCustomers);

  // Add the traders to the scrollable list for the display
  pTraderList = new ScrollableContainer(TRADERS);
  pTraderList->addDisplayList(totalTraders);

  // Initializing member variables
  pTitle = new QLabel(QString("ADMINISTRATOR"));
  pTitle->setObjectName("AdminViewTitle");

  pEmployeeLabel = new QLabel(QString("Employee Accounts"));
  pEmployeeLabel->setObjectName("AdminViewEmployeeLabel");

  pCustomerLabel = new QLabel(QString("Customer Accounts"));
  pCustomerLabel->setObjectName("AdminViewCustomerLabel");

  pCreateTraderButton = new QPushButton(QString("Add Trader"));
  pCreateTraderButton->setMinimumSize(MIN_BUTTON_SIZE);

  pDeactivateTraderButton = new QPushButton(QString("Deactivate Trader"));
  pDeactivateTraderButton->setMinimumSize(MIN_BUTTON_SIZE);

  pCreateCustomerButton = new QPushButton(QString("Add Customer"));
  pCreateCustomerButton->setMinimumSize(MIN_BUTTON_SIZE);

  pDeleteCustomerButton = new QPushButton(QString("Remove Customer"));
  pDeleteCustomerButton->setMinimumSize(MIN_BUTTON_SIZE);

  pLogOutButton = new QPushButton(QString("Log Out"));
  pLogOutButton->setMinimumSize(MIN_BUTTON_SIZE);

  pMainLayout = new QVBoxLayout(this);
  pTitleAndLogOutButton = new QHBoxLayout();
  pListsLayout = new QHBoxLayout();
  pEmployeeLayout = new QVBoxLayout();
  pCustomerLayout = new QVBoxLayout();
  pTraderButtonLayout = new QHBoxLayout();
  pCustomerButtonLayout = new QHBoxLayout();

  // Set up the layout, from left to right, top down the screen should display:
  //  Top row:
  //    Screen title
  //    Log out button (in the far right corner)
  //  Next row:
  //    Trader container
  //    Customer container
  //  Bottom row:
  //    Add trader button
  //    Remove trader button
  //    Add customer button
  //    Remove customer button
  pTitleAndLogOutButton->addWidget(pTitle, 0, Qt::AlignLeft);
  pTitleAndLogOutButton->addStretch(10);
  pTitleAndLogOutButton->addWidget(pLogOutButton, 0, Qt::AlignRight);

  pTraderButtonLayout->addStretch(2);
  pTraderButtonLayout->addWidget(pCreateTraderButton, 0);
  pTraderButtonLayout->addStretch(2);
  pTraderButtonLayout->addWidget(pDeactivateTraderButton, 0);
  pTraderButtonLayout->addStretch(2);

  pEmployeeLayout->addWidget(pEmployeeLabel, 0, Qt::AlignLeft);
  pEmployeeLayout->addWidget(pTraderList);
  pEmployeeLayout->addSpacing(10);
  pEmployeeLayout->addLayout(pTraderButtonLayout);

  pCustomerButtonLayout->addStretch(2);
  pCustomerButtonLayout->addWidget(pCreateCustomerButton, 0);
  pCustomerButtonLayout->addStretch(2);
  pCustomerButtonLayout->addWidget(pDeleteCustomerButton, 0);
  pCustomerButtonLayout->addStretch(2);

  pCustomerLayout->addWidget(pCustomerLabel, 0, Qt::AlignLeft);
  pCustomerLayout->addWidget(pCustomerList);
  pCustomerLayout->addSpacing(10);
  pCustomerLayout->addLayout(pCustomerButtonLayout);

  pListsLayout->addLayout(pEmployeeLayout);
  pListsLayout->addSpacing(20);
  pListsLayout->addLayout(pCustomerLayout);

  pMainLayout->addLayout(pTitleAndLogOutButton);
  pMainLayout->addSpacing(15);
  pMainLayout->addLayout(pListsLayout);

  // Connect widgets to appropriate slots
  connect(pLogOutButton, &QPushButton::clicked, this,
          &AdminMainView::logOutInitiated);
  connect(pCreateTraderButton, &QPushButton::clicked, this,
          &AdminMainView::createEmployeeInitiated);
  connect(pTraderList, &ScrollableContainer::notifyOfEmployeeItemSingleClick,
          this, &AdminMainView::employeeSelected);
  connect(pTraderList, &ScrollableContainer::notifyOfEmployeeItemDoubleClick,
          this, &AdminMainView::editEmployeeInitiated);
  connect(pDeactivateTraderButton, &QPushButton::clicked, this,
          &AdminMainView::notifyOfEmployeeDeletion);
  connect(pCreateCustomerButton, &QPushButton::clicked, this,
          &AdminMainView::notifyOfCustomerCreation);
  connect(pCustomerList, &ScrollableContainer::notifyOfCustomerItemSingleClick,
          this, &AdminMainView::customerSelected);
  connect(pCustomerList, &ScrollableContainer::notifyOfCustomerItemDoubleClick,
          this, &AdminMainView::editCustomerInitiated);
  connect(pDeleteCustomerButton, &QPushButton::clicked, this,
          &AdminMainView::notifyOfCustomerDeletion);
}

AdminMainView::~AdminMainView() {}

// Directs the ScrollableContainers to refresh their display
void AdminMainView::refreshEmployees(std::vector<Employee> totalEmployees) {
  pTraderList->refreshDisplayList(totalEmployees);
}

void AdminMainView::refreshCustomers(std::vector<Customer> totalCustomers) {
  pCustomerList->refreshDisplayList(totalCustomers);
}

// This is necessarcy to use style sheets.
void AdminMainView::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// *********************SLOTS*********************************************
// Notifies the AdminController of logout, so it can notify the MasterController
void AdminMainView::logOutInitiated() { emit notifyOfLogOut(); }

// Called when the create employee button is clicked, causing the
// AdminController to then display the CreateEmployeeWindow
void AdminMainView::createEmployeeInitiated() {
  emit notifyOfEmployeeCreation();
}

// Called when a displayed employee is double clicked, causing the
// AdminController to display the CreateEmployeeWindow with the employee
// information populated
void AdminMainView::editEmployeeInitiated(int id) {
  emit notifyOfEmployeeEdit(id);
}

// Called when the delete employee button is clicked, causing the
// AdminController to display the warning window
void AdminMainView::deleteEmployeeInitiated() {
  emit notifyOfEmployeeDeletion();
}

// Called when the create customer button is clicked, causing the
// AdminController to display the create customer window
void AdminMainView::createCustomerInitiated() {
  emit notifyOfCustomerCreation();
}

// Called when a displayed customer is double clicked
void AdminMainView::editCustomerInitiated(int id) {
  emit notifyOfCustomerEdit(id);
}

// Called when the delete customer button is clicked, causing the
// AdminController to display the warning window
void AdminMainView::deleteCustomerInitiated() {
  emit notifyOfCustomerDeletion();
}

void AdminMainView::customerSelected(int id) {
  emit notifyOfCustomerSelection(id);
}

void AdminMainView::employeeSelected(int id) {
  emit notifyOfEmployeeSelection(id);
}
