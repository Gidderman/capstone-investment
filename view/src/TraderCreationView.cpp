// This class implements TraderCreationView.h, see header file for further
// information

#include "TraderCreationView.h"

#include <QPainter>

TraderCreationView::TraderCreationView() : id(-1) {

  this->setObjectName("TraderCreationView");

  // Initialize display components
  pCreateEmployeeTitle = new QLabel("Create Employee");
  pCreateEmployeeTitle->setObjectName("CreateEmployeeTitle");

  pFirstNameLabel = new QLabel("First Name");
  pFirstNameLabel->setObjectName("CreateEmployeeItemLabel");

  pLastNameLabel = new QLabel("Last Name");
  pLastNameLabel->setObjectName("CreateEmployeeItemLabel");

  pRoleSelectionLabel = new QLabel("Role");
  pRoleSelectionLabel->setObjectName("CreateEmployeeItemLabel");

  pFirstNameEntry = new QLineEdit();
  pLastNameEntry = new QLineEdit();
  pRoleSelection = new QComboBox();
  pRoleSelection->addItem(QString("Trader"));
  pRoleSelection->addItem(QString("Admin"));

  pCreateEmployeeButton = new QPushButton(QString("Create Employee"));
  pCancelCreationButton = new QPushButton(QString("Cancel"));

  pResetPasswordButton = new QPushButton(QString("Reset Password"));
  pUnlockAccountButton = new QPushButton(QString("Unlock Account"));

  // Set up the layout. Components are displayed from top to bottom in the order
  // they are added to the layout
  layout = new QVBoxLayout(this);

  layout->addWidget(pCreateEmployeeTitle, 0, Qt::AlignCenter);
  layout->addSpacing(10);

  layout->addWidget(pFirstNameLabel, 0, Qt::AlignLeft);
  layout->addWidget(pFirstNameEntry, 1);
  layout->addSpacing(5);

  layout->addWidget(pLastNameLabel, 0, Qt::AlignLeft);
  layout->addWidget(pLastNameEntry, 1);
  layout->addSpacing(5);

  layout->addWidget(pRoleSelectionLabel, 0, Qt::AlignLeft);
  layout->addWidget(pRoleSelection, 1);
  layout->addSpacing(10);

  layout->addWidget(pCreateEmployeeButton, 0, Qt::AlignCenter);
  layout->addSpacing(5);
  layout->addWidget(pCancelCreationButton, 0, Qt::AlignCenter);
  layout->addSpacing(5);
  layout->addWidget(pResetPasswordButton, 0, Qt::AlignCenter);
  layout->addSpacing(5);
  layout->addWidget(pUnlockAccountButton, 0, Qt::AlignCenter);

  // Make connections
  connect(pCreateEmployeeButton, &QPushButton::clicked, this,
          &TraderCreationView::listenForEmployeeCreation);
  connect(pCancelCreationButton, &QPushButton::clicked, this,
          &TraderCreationView::listenForCreationCancellation);
  connect(pUnlockAccountButton, &QPushButton::clicked, this,
          &TraderCreationView::listenForAccountUnlock);
  connect(pResetPasswordButton, &QPushButton::clicked, this,
          &TraderCreationView::listenForPasswordReset);
}

TraderCreationView::~TraderCreationView() {}

// Called when creating a new employee
void TraderCreationView::run() {
  this->clear();

  // Neither of these buttons are applicable to creating an account
  pResetPasswordButton->hide();
  pUnlockAccountButton->hide();

  adjustSize();

  this->show();
}

// Called when editing an existing employee. Prepopulates all the fiels with the
// employee data.
void TraderCreationView::run(std::vector<QString> employee) {
  pFirstNameEntry->setText(employee.at(0));
  pLastNameEntry->setText(employee.at(1));
  pRoleSelection->setCurrentText(employee.at(2));
  if (employee.size() == 4) {
    id = employee.at(3).toInt();
  }

  pCreateEmployeeButton->setText("Save Changes");

  pResetPasswordButton->show();
  pUnlockAccountButton->show();

  adjustSize();

  this->show();
}

void TraderCreationView::end() {
  this->clear();
  this->hide();
}

// This is necessarcy to use style sheets.
void TraderCreationView::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//************************PRIVATE FUNCTIONS****************
// Clears all entry fields to their defaults
void TraderCreationView::clear() {
  id = -1;
  pFirstNameEntry->clear();
  pLastNameEntry->clear();
  pRoleSelection->setCurrentText("Trader");
  pCreateEmployeeButton->setText("Create Employee");
}

//************************SLOTS****************************
// Called when the Create Employee/Save Employee button is clicked,
// lets AdminController know that there is some action that needs to be taken
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

// Called when the cancel button is clicked, lets the AdminController know
// to close the screen
void TraderCreationView::listenForCreationCancellation() {
  emit notifyOfCancellation();
}

// Called when the Unlock account button is clicked. Informs the Admin
// Controller
void TraderCreationView::listenForAccountUnlock() {
  emit notifyOfAccountUnlock(this->id);
}

void TraderCreationView::listenForPasswordReset() {
  emit notifyOfPasswordReset(this->id);
}
