// This class implements CustomerCreationView.h, refer to that header file for
// more information

#include "CustomerCreationView.h"
#include <qboxlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>

CustomerCreationView::CustomerCreationView() {

  // Initialize all display items, to include the placeholder text for all entry
  // boxes
  pFirstNameEntry = new QLineEdit();
  pFirstNameEntry->setPlaceholderText("First Name");
  pLastNameEntry = new QLineEdit();
  pLastNameEntry->setPlaceholderText("Last Name");
  pPhoneNumberEntry = new QLineEdit();
  pPhoneNumberEntry->setPlaceholderText("Phone Number");
  pEmailEntry = new QLineEdit();
  pEmailEntry->setPlaceholderText("E-Mail");
  pInitialInvestmentEntry = new QLineEdit();
  pInitialInvestmentEntry->setPlaceholderText("Initial Money Invested");
  pAccountTypeEntry = new QComboBox();
  pAccountTypeEntry->addItem("Brokerage");
  pAccountTypeEntry->addItem("Retirement");

  pCreateCustomerButton = new QPushButton(QString("Create Customer"));
  pCancelCreationButton = new QPushButton(QString("Cancel"));

  // Add components to the layout, they are displayed from top to bottom in the
  // order they are added to the layout
  layout = new QVBoxLayout(this);
  layout->addWidget(pFirstNameEntry);
  layout->addWidget(pLastNameEntry);
  layout->addWidget(pPhoneNumberEntry);
  layout->addWidget(pEmailEntry);
  layout->addWidget(pInitialInvestmentEntry);
  layout->addWidget(pAccountTypeEntry);
  layout->addWidget(pCreateCustomerButton);
  layout->addWidget(pCancelCreationButton);

  // Connect the buttons to the appropriate slots
  connect(pCreateCustomerButton, &QPushButton::clicked, this,
          &CustomerCreationView::listenForCustomerCreation);
  connect(pCancelCreationButton, &QPushButton::clicked, this,
          &CustomerCreationView::listenForCreationCancel);
}

CustomerCreationView::~CustomerCreationView() {}

void CustomerCreationView::run() { this->show(); }

void CustomerCreationView::end() { this->hide(); } // TODO: clear all fields

//************************SLOTS***********************
// Connected to the Create Customer button
void CustomerCreationView::listenForCustomerCreation() {
  emit notifyOfCustomerCreation();
}

// Connected to the cancel button
void CustomerCreationView::listenForCreationCancel() {
  emit notifyOfCreationCancel();
}
