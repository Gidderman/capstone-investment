#include "TraderMainView.h"
#include "ScrollableContainer.h"

TraderMainView::TraderMainView(
    std::vector<QString> employeeDisplayInfo,
    std::vector<CustomerDisplayItem *> *managedCustomers) {
  pPageTitleDisplay = new QLabel(QString("Trader"));
  pTraderNameDisplay = new QLabel("Welcome " + employeeDisplayInfo.at(0));
  pTraderAccountNumber = new QLabel(employeeDisplayInfo.at(1));
  pNumAccountsManagedDisplay = new QLabel(employeeDisplayInfo.at(2));

  pLogOutButton = new QPushButton(QString("Log Out"));

  pManagedCustomersList = new ScrollableContainer(CUSTOMERS);
  pManagedCustomersList->addDisplayList(managedCustomers);

  pTraderInformationLayout = new QVBoxLayout();
  pTraderInformationLayout->addWidget(pPageTitleDisplay);
  pTraderInformationLayout->addWidget(pTraderNameDisplay);
  pTraderInformationLayout->addWidget(pTraderAccountNumber);
  pTraderInformationLayout->addWidget(pNumAccountsManagedDisplay);

  pLogOutButtonAndCustomerListLayout = new QVBoxLayout();
  pLogOutButtonAndCustomerListLayout->addWidget(pLogOutButton);
  pLogOutButtonAndCustomerListLayout->addWidget(pManagedCustomersList);

  pMainLayout = new QHBoxLayout(this);
  pMainLayout->addLayout(pTraderInformationLayout);
  pMainLayout->addLayout(pLogOutButtonAndCustomerListLayout);

  // SIGNAL CONNECTIONS
  connect(pLogOutButton, &QPushButton::clicked, this,
          &TraderMainView::listenForLogOut);
  connect(pManagedCustomersList,
          &ScrollableContainer::notifyOfCustomerItemSelection, this,
          &TraderMainView::listenForCustomerSelection);
}

TraderMainView::~TraderMainView() {}

//**********************SLOTS******************************
void TraderMainView::listenForLogOut() { emit notifyOfLogOut(); }

void TraderMainView::listenForCustomerSelection(int id) {
  emit notifyOfCustomerSelection(id);
}
