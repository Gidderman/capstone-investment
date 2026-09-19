#include "CustomerCreationView.h"
#include <qboxlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>

CustomerCreationView::CustomerCreationView() {
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

  layout = new QVBoxLayout(this);
  layout->addWidget(pFirstNameEntry);
  layout->addWidget(pLastNameEntry);
  layout->addWidget(pPhoneNumberEntry);
  layout->addWidget(pEmailEntry);
  layout->addWidget(pInitialInvestmentEntry);
  layout->addWidget(pAccountTypeEntry);
  layout->addWidget(pCreateCustomerButton);
  layout->addWidget(pCancelCreationButton);

  connect(pCreateCustomerButton, &QPushButton::clicked, this,
          &CustomerCreationView::listenForCustomerCreation);
  connect(pCancelCreationButton, &QPushButton::clicked, this,
          &CustomerCreationView::listenForCreationCancel);
}

CustomerCreationView::~CustomerCreationView() {}

void CustomerCreationView::run() { this->show(); }

void CustomerCreationView::end() { this->hide(); } // TODO: clear all fields

QString CustomerCreationView::getFirstName() { return pFirstNameEntry->text(); }

QString CustomerCreationView::getLastName() { return pLastNameEntry->text(); }

QString CustomerCreationView::getPhoneNumber() {
  return pPhoneNumberEntry->text();
}

QString CustomerCreationView::getEmail() { return pEmailEntry->text(); }

QString CustomerCreationView::getInitialInvestment() {
  return pInitialInvestmentEntry->text();
}

int CustomerCreationView::getAccountType() {
  // TODO: this
}

//************************SLOTS***********************
void CustomerCreationView::listenForCustomerCreation() {
  emit notifyOfCustomerCreation();
}

void CustomerCreationView::listenForCreationCancel() {
  emit notifyOfCreationCancel();
}
