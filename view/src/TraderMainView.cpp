// This class implements TraderMainView.h, see the header file for more info.

#include "TraderMainView.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <qboxlayout.h>
#include <qnamespace.h>

TraderMainView::TraderMainView(std::vector<QString> employeeDisplayInfo,
                               std::vector<Customer> managedCustomers) {

  this->setObjectName("TraderMainView");

  this->setMinimumSize(1024, 640);

  QSize MIN_BUTTON_SIZE = {185, 30};

  // Initialize the display variables
  pPageTitleDisplay = new QLabel(QString("Trader"));
  pPageTitleDisplay->setObjectName("TraderMainTitle");

  pTraderNameLabel = new QLabel("Current Employee");
  pTraderNameLabel->setObjectName("TraderMainLabel");

  pTraderNameDisplay = new QLabel(employeeDisplayInfo.at(0));
  pTraderNameDisplay->setObjectName("TraderMainContent");

  pTraderAccountNumberLabel = new QLabel("Account Number");
  pTraderAccountNumberLabel->setObjectName("TraderMainLabel");

  if (employeeDisplayInfo.at(1).size() < 8) {
    for (unsigned int i = 0; i < 8 - employeeDisplayInfo.at(1).size(); i++) {
      employeeDisplayInfo.at(1) = "0" + employeeDisplayInfo.at(1);
    }
  }

  pTraderAccountNumber = new QLabel(employeeDisplayInfo.at(1));
  pTraderAccountNumber->setObjectName("TraderMainContent");

  pNumAccountsManagedLabel = new QLabel("Number of Accounts Managed");
  pNumAccountsManagedLabel->setObjectName("TraderMainLabel");

  pNumAccountsManagedDisplay = new QLabel(employeeDisplayInfo.at(2));
  pNumAccountsManagedDisplay->setObjectName("TraderMainContent");

  pLogOutButton = new QPushButton(QString("Log Out"));
  pLogOutButton->setMinimumSize(MIN_BUTTON_SIZE);

  pCustomerListLabel = new QLabel("Managed Customers");
  pCustomerListLabel->setObjectName("TraderMainLabel");

  // Create the custom ScrollableContainer. Once the type of container is
  // defined, it is necessary to pass in the list of items to display
  pManagedCustomersList = new ScrollableContainer(CUSTOMERS);
  pManagedCustomersList->addDisplayList(managedCustomers);

  // Set up the layout:
  // On the left there is the title and the trader information
  // On the right on top there is the log out button, below that is the
  // list of customers that are managed by that employee
  pTitleAndLogOutButtonLayout = new QHBoxLayout();
  pTitleAndLogOutButtonLayout->addWidget(pPageTitleDisplay, 0, Qt::AlignLeft);
  pTitleAndLogOutButtonLayout->addStretch(3);
  pTitleAndLogOutButtonLayout->addWidget(pLogOutButton, 0, Qt::AlignRight);

  pTraderInformationLayout = new QVBoxLayout();
  pTraderInformationLayout->addWidget(pTraderNameLabel, 0, Qt::AlignLeft);
  pTraderInformationLayout->addWidget(pTraderNameDisplay, 0, Qt::AlignCenter);
  pTraderInformationLayout->addStretch(1);
  pTraderInformationLayout->addWidget(pTraderAccountNumberLabel, 0,
                                      Qt::AlignLeft);
  pTraderInformationLayout->addWidget(pTraderAccountNumber, 0, Qt::AlignCenter);
  pTraderInformationLayout->addStretch(1);
  pTraderInformationLayout->addWidget(pNumAccountsManagedLabel, 0,
                                      Qt::AlignLeft);
  pTraderInformationLayout->addWidget(pNumAccountsManagedDisplay, 0,
                                      Qt::AlignCenter);
  pTraderInformationLayout->addStretch(5);

  pCustomerLabelAndList = new QVBoxLayout();
  pCustomerLabelAndList->addWidget(pCustomerListLabel, 0, Qt::AlignLeft);
  pCustomerLabelAndList->addWidget(pManagedCustomersList);

  pTraderInfoAndCustomerList = new QHBoxLayout();
  pTraderInfoAndCustomerList->addLayout(pTraderInformationLayout, 0);
  pTraderInfoAndCustomerList->addSpacing(10);
  pTraderInfoAndCustomerList->addLayout(pCustomerLabelAndList, 1);

  pMainLayout = new QVBoxLayout(this);
  pMainLayout->addLayout(pTitleAndLogOutButtonLayout);
  pMainLayout->addSpacing(20);
  pMainLayout->addLayout(pTraderInfoAndCustomerList);

  // SIGNAL CONNECTIONS
  connect(pLogOutButton, &QPushButton::clicked, this,
          &TraderMainView::listenForLogOut);
  connect(pManagedCustomersList,
          &ScrollableContainer::notifyOfCustomerItemDoubleClick, this,
          &TraderMainView::listenForCustomerSelection);
}

TraderMainView::~TraderMainView() {}

// This is necessarcy to use style sheets.
void TraderMainView::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//**********************SLOTS******************************
// Called when the log out button is clicked, notifying the TraderController
// that a log out is requested
void TraderMainView::listenForLogOut() { emit notifyOfLogOut(); }

// Called when a user double clicks on a displayed customer, informing the
// Trader Controller to pull up the associated CustomerManagerView
void TraderMainView::listenForCustomerSelection(int id) {
  emit notifyOfCustomerSelection(id);
}
