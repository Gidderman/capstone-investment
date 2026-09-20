#include "TraderCreationView.h"

#include <iostream>

TraderCreationView::TraderCreationView() : id(-1) {
  pFirstNameEntry = new QLineEdit();
  pFirstNameEntry->setPlaceholderText("First Name");
  pLastNameEntry = new QLineEdit();
  pLastNameEntry->setPlaceholderText("Last Name");
  pRoleSelection = new QComboBox();
  pRoleSelection->addItem(QString("Trader"));
  pRoleSelection->addItem(QString("Admin"));

  pCreateEmployeeButton = new QPushButton(QString("Create Employee"));
  pCancelCreationButton = new QPushButton(QString("Cancel"));

  pUnlockAccountButton = new QPushButton(QString("Unlock Account"));

  layout = new QVBoxLayout(this);

  layout->addWidget(pFirstNameEntry);
  layout->addWidget(pLastNameEntry);
  layout->addWidget(pRoleSelection);
  layout->addWidget(pCreateEmployeeButton);
  layout->addWidget(pCancelCreationButton);
  // TODO: configure so this is only shown for existing accounts and greyed out
  // if locked
  layout->addWidget(pUnlockAccountButton);

  connect(pCreateEmployeeButton, &QPushButton::clicked, this,
          &TraderCreationView::listenForEmployeeCreation);
  connect(pCancelCreationButton, &QPushButton::clicked, this,
          &TraderCreationView::listenForCreationCancellation);
  connect(pUnlockAccountButton, &QPushButton::clicked, this,
          &TraderCreationView::listenForAccountUnlock);
}

TraderCreationView::~TraderCreationView() {}

void TraderCreationView::run() { this->show(); }

void TraderCreationView::run(std::vector<QString> employee) {
  pFirstNameEntry->setText(employee.at(0));
  pLastNameEntry->setText(employee.at(1));
  pRoleSelection->setCurrentText(employee.at(2));
  std::cout << "Setting role to " << employee.at(2).toStdString() << " for "
            << employee.at(1).toStdString() << std::endl;
  if (employee.size() == 4) {
    id = employee.at(3).toInt();
  }

  pCreateEmployeeButton->setText("Save Changes");

  this->show();
}

void TraderCreationView::end() {
  id = -1;
  pFirstNameEntry->clear();
  pLastNameEntry->clear();
  pRoleSelection->setCurrentText("Trader");
  pCreateEmployeeButton->setText("Create Employee");
  this->hide();
}

//************************SLOTS****************************
void TraderCreationView::listenForEmployeeCreation() {
  std::vector<QString> employee;
  employee.push_back(pFirstNameEntry->text());
  employee.push_back(pLastNameEntry->text());
  employee.push_back(pRoleSelection->currentText());
  employee.push_back(QString::number(id));

  emit notifyOfEmployeeCreation(
      employee); // Connected to
                 // AdminController::listenForEmployeeActionConfirmation
}

void TraderCreationView::listenForCreationCancellation() {
  emit notifyOfCancellation();
}

void TraderCreationView::listenForAccountUnlock() {
  emit notifyOfAccountUnlock();
}
