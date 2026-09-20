// This class implements AdminMainView.h, see that header file for more
// information

#include "AdminMainView.h"
#include "ScrollableContainer.h"

#include <QString>

AdminMainView::AdminMainView(std::vector<CustomerDisplayItem *> *totalCustomers,
                             std::vector<TraderDisplayItem *> *totalTraders) {
  // Add the customers to the scrollable container for display
  pCustomerList = new ScrollableContainer(CUSTOMERS);
  pCustomerList->addDisplayList(totalCustomers);

  // Add the traders to the scrollable list for the display
  pTraderList = new ScrollableContainer(TRADERS);
  pTraderList->addDisplayList(totalTraders);

  // Initializing member variables
  pTitle = new QLabel(QString("ADMINISTRATOR"));

  pCreateTraderButton = new QPushButton(QString("Add Trader"));
  pDeleteTraderButton = new QPushButton(QString("Remove Trader"));
  pCreateCustomerButton = new QPushButton(QString("Add Customer"));
  pDeleteCustomerButton = new QPushButton(QString("Remove Customer"));
  pLogOutButton = new QPushButton(QString("Log Out"));

  pMainLayout = new QHBoxLayout(this);
  pTitleAndTraderLayout = new QVBoxLayout();
  pLogOutAndCustomerLayout = new QVBoxLayout();
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
  pTitleAndTraderLayout->addWidget(pTitle);
  pTitleAndTraderLayout->addWidget(pTraderList);
  pTraderButtonLayout->addWidget(pCreateTraderButton);
  pTraderButtonLayout->addWidget(pDeleteTraderButton);
  pTitleAndTraderLayout->addLayout(pTraderButtonLayout);

  pLogOutAndCustomerLayout->addWidget(pLogOutButton);
  pLogOutAndCustomerLayout->addWidget(pCustomerList);
  pCustomerButtonLayout->addWidget(pCreateCustomerButton);
  pCustomerButtonLayout->addWidget(pDeleteCustomerButton);
  pLogOutAndCustomerLayout->addLayout(pCustomerButtonLayout);

  pMainLayout->addLayout(pTitleAndTraderLayout);
  pMainLayout->addLayout(pLogOutAndCustomerLayout);

  // Connect widgets to appropriate slots
  connect(pLogOutButton, &QPushButton::clicked, this,
          &AdminMainView::logOutInitiated);
  connect(pCreateTraderButton, &QPushButton::clicked, this,
          &AdminMainView::createEmployeeInitiated);
  connect(pTraderList, &ScrollableContainer::notifyOfEmployeeItemSelection,
          this, &AdminMainView::editEmployeeInitiated);
  connect(pDeleteTraderButton, &QPushButton::clicked, this,
          &AdminMainView::notifyOfEmployeeDeletion);
  connect(pCreateCustomerButton, &QPushButton::clicked, this,
          &AdminMainView::notifyOfCustomerCreation);
  connect(pCustomerList, &ScrollableContainer::notifyOfCustomerItemSelection,
          this, &AdminMainView::editCustomerInitiated);
  connect(pDeleteCustomerButton, &QPushButton::clicked, this,
          &AdminMainView::notifyOfCustomerDeletion);
}

AdminMainView::~AdminMainView() {}

// Directs the ScrollableContainers to refresh their display
void AdminMainView::refreshPage() {

  pTraderList->refreshDisplayList();
  pCustomerList->refreshDisplayList();
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
// TODO: This should accept the selected customers id, and pass that to the
// admin controller
void AdminMainView::editCustomerInitiated() { emit notifyOfCustomerEdit(); }

// Called when the delete customer button is clicked, causing the
// AdminController to display the warning window
void AdminMainView::deleteCustomerInitiated() {
  emit notifyOfCustomerDeletion();
}
