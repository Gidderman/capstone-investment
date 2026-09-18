#include "AdminMainView.h"

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
}

AdminMainView::~AdminMainView() {}

// *********************SLOTS*********************************************
void AdminMainView::logOutInitiated() { emit notifyOfLogOut(); }
void AdminMainView::createEmployeeInitiated() {
  emit notifyOfEmployeeCreation();
}
void AdminMainView::editEmployeeInitiated() { emit notifyOfEmployeeEdit(); }
void AdminMainView::deleteEmployeeInitiated() {
  emit notifyOfEmployeeDeletion();
}
void AdminMainView::createCustomerInitiated() {
  emit notifyOfCustomerCreation();
}
void AdminMainView::editCustomerInitiated() { emit notifyOfCustomerEdit(); }
void AdminMainView::deleteCustomerInitiated() {
  emit notifyOfCustomerDeletion();
}
