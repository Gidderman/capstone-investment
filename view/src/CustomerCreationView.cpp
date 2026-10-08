// This class implements CustomerCreationView.h, refer to that header file for
// more information

#include "CustomerCreationView.h"

#include <QPainter>

CustomerCreationView::CustomerCreationView(std::vector<QString> employeeNames)
    : id(-1) {

  this->setObjectName("CustomerCreationView");

  pCreateCustomerLabel = new QLabel("Create Customer");
  pCreateCustomerLabel->setObjectName("CustomerCreationTitle");

  pFirstNameLabel = new QLabel("First Name");
  pFirstNameLabel->setObjectName("CustomerCreationItemLabel");

  pLastNameLabel = new QLabel("Last Name");
  pLastNameLabel->setObjectName("CustomerCreationItemLabel");

  pPhoneNumberLabel = new QLabel("Phone Number");
  pPhoneNumberLabel->setObjectName("CustomerCreationItemLabel");

  pEmailLabel = new QLabel("Email");
  pEmailLabel->setObjectName("CustomerCreationItemLabel");

  pInitialInvestmentLabel = new QLabel("Initial Investment");
  pInitialInvestmentLabel->setObjectName("CustomerCreationItemLabel");

  pAccountTypeLabel = new QLabel("Type of Account");
  pAccountTypeLabel->setObjectName("CustomerCreationItemLabel");

  pEmployeeAssignedLabel = new QLabel("Trader Assigned");
  pEmployeeAssignedLabel->setObjectName("CustomerCreationItemLabel");

  // Initialize all display items, to include the placeholder text for all entry
  // boxes
  pFirstNameEntry = new QLineEdit();
  pLastNameEntry = new QLineEdit();
  pPhoneNumberEntry = new QLineEdit();
  pEmailEntry = new QLineEdit();
  pInitialInvestmentEntry = new QLineEdit();
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
  layout->addWidget(pCreateCustomerLabel, 0, Qt::AlignCenter);
  layout->addSpacing(10);

  layout->addWidget(pFirstNameLabel, 0, Qt::AlignLeft);
  layout->addWidget(pFirstNameEntry, 1);
  layout->addSpacing(5);

  layout->addWidget(pLastNameLabel, 0, Qt::AlignLeft);
  layout->addWidget(pLastNameEntry, 1);
  layout->addSpacing(5);

  layout->addWidget(pPhoneNumberLabel, 0, Qt::AlignLeft);
  layout->addWidget(pPhoneNumberEntry, 1);
  layout->addSpacing(5);

  layout->addWidget(pEmailLabel, 0, Qt::AlignLeft);
  layout->addWidget(pEmailEntry, 1);
  layout->addSpacing(5);

  layout->addWidget(pInitialInvestmentLabel, 0, Qt::AlignLeft);
  layout->addWidget(pInitialInvestmentEntry, 1);
  layout->addSpacing(5);

  layout->addWidget(pAccountTypeLabel, 0, Qt::AlignLeft);
  layout->addWidget(pAccountTypeEntry, 1);
  layout->addSpacing(5);

  layout->addWidget(pEmployeeAssignedLabel, 0, Qt::AlignLeft);
  layout->addWidget(pEmployeeAssignedEntry, 1);
  layout->addSpacing(10);

  layout->addWidget(pCreateCustomerButton, 0, Qt::AlignCenter);
  layout->addSpacing(5);

  layout->addWidget(pCancelCreationButton, 0, Qt::AlignCenter);

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

// This is necessarcy to use style sheets.
void CustomerCreationView::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
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
  std::vector<QString> customer;
  customer.push_back(QString::number(id));
  customer.push_back(pFirstNameEntry->text());
  customer.push_back(pLastNameEntry->text());
  customer.push_back(pPhoneNumberEntry->text());
  customer.push_back(pEmailEntry->text());
  customer.push_back(pInitialInvestmentEntry->text());
  customer.push_back(pAccountTypeEntry->currentText());
  customer.push_back(pEmployeeAssignedEntry->currentText());

  emit notifyOfCustomerCreation(customer);
}

// Connected to the cancel button
void CustomerCreationView::listenForCreationCancel() {
  emit notifyOfCreationCancel();
}
