// This class implements LogInView.h, see the header file for more information

#include "LogInView.h"
#include <qboxlayout.h>
#include <qpushbutton.h>

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
                           "'a-z, A-Z, 1-9, ! @ # $ % & *'";
  pDescriptionText = new QLabel(descriptorText);
  pPasswordVerificationField = new QLineEdit();
  pPasswordVerificationField->setPlaceholderText(QString("Re-enter password"));
  pPasswordVerificationField->setEchoMode(QLineEdit::Password);
  pCreatePasswordButton = new QPushButton(QString("Create Password"));

  pWarningText = new QLabel();

  connect(pNextButton, &QPushButton::clicked, this,
          &LogInView::usernameEntered);
  // Connect the log in pushbutton to the notifyOfLogInAttempt signal to allow
  // the Master Controller to know that a log in attempt is happening.
  connect(pAttemptLoginButton, &QPushButton::clicked, this,
          &LogInView::logInAttempted);
  connect(pCreatePasswordButton, &QPushButton::clicked, this,
          &LogInView::initiatePasswordCreation);

  // Set up the layout for the enter username screen.
  pUsernameScreenLayout = new QVBoxLayout(this);
  pUsernameScreenLayout->addWidget(pTitleLabel);
  pUsernameScreenLayout->addWidget(pUsernameEntryField);
  pUsernameScreenLayout->addWidget(pNextButton);
  pUsernameScreenLayout->addWidget(pWarningText);

  pPasswordEntryLayout = new QVBoxLayout(this);
  pPasswordEntryLayout->addWidget(pTitleLabel);
  pPasswordEntryLayout->addWidget(pPasswordEntryField);
  pPasswordEntryLayout->addWidget(pAttemptLoginButton);
  pPasswordEntryLayout->addWidget(pWarningText);

  pCreatePasswordLayout = new QVBoxLayout(this);
  pCreatePasswordLayout->addWidget(pTitleLabel);
  pCreatePasswordLayout->addWidget(pDescriptionText);
  pCreatePasswordLayout->addWidget(pPasswordEntryField);
  pCreatePasswordLayout->addWidget(pPasswordVerificationField);
  pCreatePasswordLayout->addWidget(pCreatePasswordButton);
  pCreatePasswordLayout->addWidget(pWarningText);

  setLayout(pUsernameScreenLayout);
}

LogInView::~LogInView() {}

void LogInView::runUsernameScreen() {
  setLayout(pUsernameScreenLayout);
  this->show();
}

void LogInView::runPasswordScreen() {
  setLayout(pPasswordEntryLayout);
  this->show();
}

void LogInView::runCreatePasswordScreen() {
  setLayout(pCreatePasswordLayout);
  this->show();
}

void LogInView::displayWarningText(QString text) {
  pWarningText->setText(text);
}

void LogInView::clear() {
  pUsernameEntryField->setText("");
  pPasswordEntryField->setText("");
  pWarningText->setText("");
}

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
