// This class implements LogInView.h, see the header file for more information

#include "LogInView.h"
#include <qboxlayout.h>
#include <qpushbutton.h>

#include <iostream>

// TODO: Format for populating the proper screen size and the various other
// widgets within, and passing data via signals

LogInView::LogInView() : title("Investment Company") {

  // Initialize all member variables
  pTitleLabel = new QLabel(QString(title));
  pUsernameEntryField = new QLineEdit();
  pUsernameEntryField->setPlaceholderText(QString("Username"));
  pNextButton = new QPushButton(QString("Next"));

  // Member variables for password entry
  pPasswordEntryField = new QLineEdit();
  pPasswordEntryField->setPlaceholderText(QString("Password"));
  pPasswordEntryField->setEchoMode(QLineEdit::Password);
  pAttemptLoginButton = new QPushButton(QString("Log In"));

  // Member variables for create password screen
  QString descriptorText = "Welcome, this is your first time logging in.\n"
                           "Passwords must be 8 to 12 characters in length,\n"
                           "and only be comprised of these characters: \n"
                           "'a-z, A-Z, 0-9, ! @ # $ % & *'";
  pDescriptionText = new QLabel(descriptorText);
  pPasswordVerificationField = new QLineEdit();
  pPasswordVerificationField->setPlaceholderText(QString("Re-enter password"));
  pPasswordVerificationField->setEchoMode(QLineEdit::Password);
  pCreatePasswordButton = new QPushButton(QString("Create Password"));

  pBackButton = new QPushButton("Back");

  pWarningText = new QLabel();

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

  pLayout = new QVBoxLayout(this);
  pLayout->addWidget(pTitleLabel);
  pLayout->addWidget(pDescriptionText);
  pLayout->addWidget(pUsernameEntryField);
  pLayout->addWidget(pPasswordEntryField);
  pLayout->addWidget(pPasswordVerificationField);
  pLayout->addWidget(pAttemptLoginButton);
  pLayout->addWidget(pCreatePasswordButton);
  pLayout->addWidget(pNextButton);
  pLayout->addWidget(pBackButton);
  pLayout->addWidget(pWarningText);
}

LogInView::~LogInView() {}

void LogInView::runUsernameScreen() {
  pTitleLabel->show();
  pUsernameEntryField->show();
  pNextButton->show();

  pPasswordEntryField->hide();
  pAttemptLoginButton->hide();
  pBackButton->hide();

  pDescriptionText->hide();
  pPasswordEntryField->hide();
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
  pUsernameEntryField->show();
  pPasswordEntryField->show();
  pAttemptLoginButton->show();
  pBackButton->show();

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
  pUsernameEntryField->show();
  pPasswordEntryField->show();
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
  pWarningText->setText("");
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
