// This class implements LogInView.h, see the header file for more information

#include "LogInView.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <qboxlayout.h>
#include <qnamespace.h>

LogInView::LogInView() : title("Investment Company") {
  QSize MIN_LINE_EDIT_SIZE = {300, 50};
  QSize MAX_LINE_EDIT_SIZE = {500, 100};

  QSize MIN_BUTTON_SIZE = {100, 40};
  QSize MAX_BUTTON_SIZE = {200, 40};

  this->setObjectName("LogInView");
  this->setMinimumSize(1024, 640);

  // Initialize all member variables
  pTitleLabel = new QLabel(QString(title));
  pTitleLabel->setObjectName("LogInViewTitleLabel");

  pUsernameLabel = new QLabel(QString("Username"));
  pUsernameLabel->setObjectName("LogInViewUsernameLabel");

  pUsernameEntryField = new QLineEdit();
  pUsernameEntryField->setObjectName("LogInViewUsernameEntryEdit");
  pUsernameEntryField->setTextMargins(10, 0, 0, 0);
  pUsernameEntryField->setMinimumSize(MIN_LINE_EDIT_SIZE);
  pUsernameEntryField->setMaximumSize(MIN_LINE_EDIT_SIZE);

  pNextButton = new QPushButton(QString("Next"));
  pNextButton->setObjectName("LogInViewNextButton");
  pNextButton->setMinimumSize(MIN_BUTTON_SIZE);
  pNextButton->setMaximumSize(MAX_BUTTON_SIZE);

  // Member variables for password entry

  pPasswordLabel = new QLabel("Password");
  pPasswordLabel->setObjectName("LogInViewPasswordLabel");

  pPasswordEntryField = new QLineEdit();
  pPasswordEntryField->setEchoMode(QLineEdit::Password);
  pPasswordEntryField->setObjectName("LogInViewPasswordEntryEdit");
  pPasswordEntryField->setTextMargins(10, 0, 0, 0);
  pPasswordEntryField->setMinimumSize(MIN_LINE_EDIT_SIZE);
  pPasswordEntryField->setMaximumSize(MIN_LINE_EDIT_SIZE);

  pAttemptLoginButton = new QPushButton(QString("Log In"));
  pAttemptLoginButton->setObjectName("LogInViewAttemptLoginButton");
  pAttemptLoginButton->setMinimumSize(MIN_BUTTON_SIZE);
  pAttemptLoginButton->setMaximumSize(MAX_BUTTON_SIZE);

  // Member variables for create password screen
  QString descriptorText = "Welcome, this is your first time logging in.\n"
                           "Passwords must be 8 to 12 characters in length,\n"
                           "and only be comprised of these characters: \n"
                           "'a-z, A-Z, 0-9, ! @ # $ % & *'";
  pDescriptionText = new QLabel(descriptorText);
  pDescriptionText->setObjectName("LogInViewDescriptionText");

  pPasswordVerificationLabel = new QLabel("Re-Enter Password");
  pPasswordVerificationLabel->setObjectName(
      "LogInViewPasswordVerificationLabel");

  pPasswordVerificationField = new QLineEdit();
  pPasswordVerificationField->setEchoMode(QLineEdit::Password);
  pPasswordVerificationField->setObjectName(
      "LogInViewPasswordVerificationEntry");
  pPasswordVerificationField->setTextMargins(10, 0, 0, 0);
  pPasswordVerificationField->setMinimumSize(MIN_LINE_EDIT_SIZE);
  pPasswordVerificationField->setMaximumSize(MIN_LINE_EDIT_SIZE);

  pCreatePasswordButton = new QPushButton(QString("Create Password"));
  pCreatePasswordButton->setObjectName("LogInViewCreatePasswordButton");
  pCreatePasswordButton->setMinimumSize(MIN_BUTTON_SIZE);
  pCreatePasswordButton->setMaximumSize(MAX_BUTTON_SIZE);

  pBackButton = new QPushButton("Back");
  pBackButton->setObjectName("LogInViewBackButton");
  pBackButton->setMinimumSize(MIN_BUTTON_SIZE);
  pBackButton->setMaximumSize(MAX_BUTTON_SIZE);

  pWarningText = new QLabel();
  pWarningText->setObjectName("LogInViewWarningText");

  connect(pNextButton, &QPushButton::clicked, this,
          &LogInView::usernameEntered);
  // Connect the log in pushbutton to the notifyOfLogInAttempt signal to allow
  // the Master Controller to know that a log in attempt is happening.
  connect(pAttemptLoginButton, &QPushButton::clicked, this,
          &LogInView::logInAttempted);
  connect(pCreatePasswordButton, &QPushButton::clicked, this,
          &LogInView::initiatePasswordCreation);
  connect(pBackButton, &QPushButton::clicked, this, &LogInView::goBack);

  connect(pPasswordVerificationField, &QLineEdit::textChanged, this,
          &LogInView::passwordVerificationFieldChanged);

  pUsernameLayout = new QVBoxLayout();
  pUsernameLayout->addWidget(pUsernameLabel, 0, Qt::AlignLeft);
  pUsernameLayout->addWidget(pUsernameEntryField, 0, Qt::AlignCenter);
  pUsernameLayout->setAlignment(Qt::AlignCenter);

  pPasswordLayout = new QVBoxLayout();
  pPasswordLayout->addWidget(pPasswordLabel, 0, Qt::AlignLeft);
  pPasswordLayout->addWidget(pPasswordEntryField, 0, Qt::AlignCenter);
  pPasswordLayout->setAlignment(Qt::AlignCenter);

  pPasswordVerificationLayout = new QVBoxLayout();
  pPasswordVerificationLayout->addWidget(pPasswordVerificationLabel, 0,
                                         Qt::AlignLeft);
  pPasswordVerificationLayout->addWidget(pPasswordVerificationField, 0,
                                         Qt::AlignLeft);
  pPasswordVerificationLayout->setAlignment(Qt::AlignCenter);

  pLayout = new QVBoxLayout(this);
  pLayout->addSpacing(50);
  pLayout->addWidget(pTitleLabel, 0, Qt::AlignCenter);
  pLayout->addSpacing(45);
  pLayout->addWidget(pDescriptionText, 0, Qt::AlignCenter);
  pLayout->addLayout(pUsernameLayout, 0);
  pLayout->addLayout(pPasswordLayout, 0);
  pLayout->addLayout(pPasswordVerificationLayout, 0);
  pLayout->addSpacing(20);
  pLayout->addWidget(pAttemptLoginButton, 0, Qt::AlignCenter);
  pLayout->addWidget(pCreatePasswordButton, 0, Qt::AlignCenter);
  pLayout->addWidget(pNextButton, 0, Qt::AlignCenter);
  pLayout->addWidget(pBackButton, 0, Qt::AlignCenter);
  pLayout->addSpacing(20);
  pLayout->addWidget(pWarningText, 0, Qt::AlignCenter);
  pLayout->addStretch(10);
}

LogInView::~LogInView() {}

void LogInView::runUsernameScreen() {
  pTitleLabel->show();
  pUsernameLabel->show();
  pUsernameEntryField->show();
  pNextButton->show();

  pPasswordEntryField->hide();
  pPasswordLabel->hide();
  pAttemptLoginButton->hide();
  pBackButton->hide();

  pDescriptionText->hide();
  pPasswordEntryField->hide();
  pPasswordVerificationLabel->hide();
  pPasswordVerificationField->hide();
  pCreatePasswordButton->hide();
  pBackButton->hide();
  pWarningText->hide();

  pUsernameEntryField->setReadOnly(false);
  adjustSize();
  this->show();
}

void LogInView::runPasswordScreen() {

  pNextButton->hide();

  pTitleLabel->show();
  pUsernameLabel->show();
  pUsernameEntryField->show();
  pPasswordLabel->show();
  pPasswordEntryField->show();
  pAttemptLoginButton->show();
  pBackButton->show();

  pPasswordVerificationLabel->hide();
  pPasswordVerificationField->hide();
  pCreatePasswordButton->hide();
  pWarningText->hide();

  pUsernameEntryField->setReadOnly(true);
  pPasswordEntryField->setFocus();

  adjustSize();
  this->show();
}

void LogInView::runCreatePasswordScreen() {

  pNextButton->hide();

  pAttemptLoginButton->hide();

  pTitleLabel->show();
  pDescriptionText->show();
  pUsernameLabel->show();
  pUsernameEntryField->show();
  pPasswordLabel->show();
  pPasswordEntryField->show();
  pPasswordVerificationLabel->show();
  pPasswordVerificationField->show();
  pCreatePasswordButton->show();
  pBackButton->show();
  pWarningText->hide();

  pUsernameEntryField->setReadOnly(true);
  pPasswordEntryField->setFocus();
  adjustSize();
  this->show();
}

void LogInView::displayWarningText(QString text) {
  pWarningText->setText(text);
  pWarningText->show();
}

void LogInView::clear() {
  pUsernameEntryField->setText("");
  pPasswordEntryField->setText("");
  pPasswordVerificationField->setText("");
  pWarningText->setText("");
}

// This is necessarcy to use style sheets.
void LogInView::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//*******************PRIVATE FUNCTIONS***************************

//**********************SLOTS************************************
void LogInView::usernameEntered() {
  emit notifyOfUsernameEntry(pUsernameEntryField->text());
}

void LogInView::logInAttempted() {
  emit notifyOfLogInAttempt(pPasswordEntryField->text());
}

void LogInView::initiatePasswordCreation() {
  emit notifyOfPasswordCreation(pPasswordEntryField->text(),
                                pPasswordVerificationField->text());
}

void LogInView::goBack() { emit userRequestsGoingBack(); }

void LogInView::passwordVerificationFieldChanged() {
  emit notifyOfPasswordVerificationFieldChange(
      pPasswordEntryField->text(), pPasswordVerificationField->text());
}
