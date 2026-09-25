// This class implements CustomerCreationView.h, refer to that header file for
// more information

#include "CustomerCreationView.h"

#include <iostream>
#include <qcombobox.h>

CustomerCreationView::CustomerCreationView(std::vector<QString> employeeNames)
    : id(-1) {

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

  pEmployeeAssignedEntry = new QComboBox();
  for (QString name : employeeNames) {
    pEmployeeAssignedEntry->addItem(name);
  }

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
  layout->addWidget(pEmployeeAssignedEntry);
  layout->addWidget(pCreateCustomerButton);
  layout->addWidget(pCancelCreationButton);

  // Connect the buttons to the appropriate slots
  connect(pCreateCustomerButton, &QPushButton::clicked, this,
          &CustomerCreationView::listenForCustomerCreation);
  connect(pCancelCreationButton, &QPushButton::clicked, this,
          &CustomerCreationView::listenForCreationCancel);
}

CustomerCreationView::~CustomerCreationView() {}

void CustomerCreationView::run() {
  this->clear();
  this->show();
}

// Called when editing a customer
void CustomerCreationView::run(std::vector<QString> customerData) {
  std::cout << "Customer data size " << customerData.size() << std::endl;

  id = customerData.at(0).toInt();
  pFirstNameEntry->setText(customerData.at(1));
  pLastNameEntry->setText(customerData.at(2));
  pPhoneNumberEntry->setText(customerData.at(3));
  pEmailEntry->setText(customerData.at(4));
  pInitialInvestmentEntry->setText(customerData.at(5));
  pAccountTypeEntry->setCurrentText(customerData.at(6));
  pEmployeeAssignedEntry->setCurrentText(customerData.at(7));

  pCreateCustomerButton->setText(QString("Save Changes"));

  this->show();
}

// Clear all fields and hide the screen
void CustomerCreationView::end() {
  this->clear();
  this->hide();
}

//************************PRIVATE FUNCTIONS***********
// Clears all fields to their defaults
void CustomerCreationView::clear() {
  pFirstNameEntry->clear();
  pLastNameEntry->clear();
  pPhoneNumberEntry->clear();
  pEmailEntry->clear();
  pInitialInvestmentEntry->clear();
  pAccountTypeEntry->setCurrentText("Retirement");
  pEmployeeAssignedEntry->setCurrentIndex(0);
  id = -1;

  pCreateCustomerButton->setText(QString("Create Customer"));
}

//************************SLOTS***********************
// Connected to the Create Customer button
void CustomerCreationView::listenForCustomerCreation() {
  std::cout << "CustomerCreationView::listenForCustomerCreation - entering"
            << std::endl;
  std::vector<QString> customer;
  customer.push_back(QString::number(id));
  customer.push_back(pFirstNameEntry->text());
  customer.push_back(pLastNameEntry->text());
  customer.push_back(pPhoneNumberEntry->text());
  customer.push_back(pEmailEntry->text());
  customer.push_back(pInitialInvestmentEntry->text());
  customer.push_back(pAccountTypeEntry->currentText());
  customer.push_back(pEmployeeAssignedEntry->currentText());

  std::cout << "CustomerCreationView::listenForCustomerCreation - exiting"
            << std::endl;
  emit notifyOfCustomerCreation(customer);
}

// Connected to the cancel button
void CustomerCreationView::listenForCreationCancel() {
  emit notifyOfCreationCancel();
}
