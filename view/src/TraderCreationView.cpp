#include "TraderCreationView.h"

TraderCreationView::TraderCreationView() {
  pFirstNameEntry = new QLineEdit();
  pLastNameEntry = new QLineEdit();
  pRoleSelection = new QComboBox();

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

QString TraderCreationView::getFirstName() { return pFirstNameEntry->text(); }

QString TraderCreationView::getLastName() { return pLastNameEntry->text(); }

int TraderCreationView::getRole() {}

//************************SLOTS****************************
void TraderCreationView::listenForEmployeeCreation() {
  emit notifyOfEmployeeCreation();
}

void TraderCreationView::listenForCreationCancellation() {
  emit notifyOfCancellation();
}

void TraderCreationView::listenForAccountUnlock() {
  emit notifyOfAccountUnlock();
}
