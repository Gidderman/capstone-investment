// This class implements TraderMainView.h, see the header file for more info.

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING

#include "TraderMainView.h"
#include "ScrollableContainer.h"

TraderMainView::TraderMainView(
    std::vector<QString> employeeDisplayInfo,
    std::vector<CustomerDisplayItem *> *managedCustomers) {
  // Initialize the display variables
  pPageTitleDisplay = new QLabel(QString("Trader"));
  pTraderNameDisplay = new QLabel("Welcome " + employeeDisplayInfo.at(0));
  pTraderAccountNumber = new QLabel(employeeDisplayInfo.at(1));
  pNumAccountsManagedDisplay = new QLabel(employeeDisplayInfo.at(2));

  pLogOutButton = new QPushButton(QString("Log Out"));

  // Create the custom ScrollableContainer. Once the type of container is
  // defined, it is necessary to pass in the list of items to display
  pManagedCustomersList = new ScrollableContainer(CUSTOMERS);
  pManagedCustomersList->addDisplayList(managedCustomers);

  // Set up the layout:
  // On the left there is the title and the trader information
  // On the right on top there is the log out button, below that is the
  // list of customers that are managed by that employee
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
// Called when the log out button is clicked, notifying the TraderController
// that a log out is requested
void TraderMainView::listenForLogOut() { emit notifyOfLogOut(); }

// Called when a user double clicks on a displayed customer, informing the
// Trader Controller to pull up the associated CustomerManagerView
void TraderMainView::listenForCustomerSelection(int id) {
  emit notifyOfCustomerSelection(id);
}
