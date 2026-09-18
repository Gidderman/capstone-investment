#include "TraderMainView.h"
#include "ScrollableContainer.h"
#include <qboxlayout.h>
#include <qcoreevent.h>
#include <qpushbutton.h>

TraderMainView::TraderMainView(
    std::vector<CustomerDisplayItem *> *managedCustomers) {
  pPageTitleDisplay = new QLabel(QString("Trader"));
  pTraderNameDisplay = new QLabel(
      QString("Temp Value For Page Format")); // TODO: Populate page label
  pTraderAccountNumber = new QLabel(QString("XXXXXX"));
  pNumAccountsManagedDisplay = new QLabel(QString("XX Accounts Managed"));

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

void TraderMainView::listenForCustomerSelection() {
  emit notifyOfCustomerSelection();
}
