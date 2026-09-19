#include "LogInView.h"

// TODO: Format for populating the proper screen size and the various other
// widgets within.

LogInView::LogInView()
    : enteredUsername(""), enteredPassword(""), title("Investment Company") {

  // Initialize all member variables
  pTitleLabel = new QLabel(QString(title));
  pUsernameEntryField = new QLineEdit();
  pUsernameEntryField->setPlaceholderText(QString("Username"));
  pPasswordEntryField = new QLineEdit();
  pPasswordEntryField->setPlaceholderText(QString("Password"));
  pPasswordEntryField->setEchoMode(QLineEdit::Password);
  pAttemptLoginButton = new QPushButton(QString("Log In"));

  // Connect the log in pushbutton to the notifyOfLogInAttempt signal to allow
  // the Master Controller to know that a log in attempt is happening.
  connect(pAttemptLoginButton, &QPushButton::clicked, this,
          &LogInView::logInAttempted);

  // Set up the layout for the screen.
  pScreenLayout = new QVBoxLayout(this);
  pScreenLayout->addWidget(pTitleLabel);
  pScreenLayout->addWidget(pUsernameEntryField);
  pScreenLayout->addWidget(pPasswordEntryField);
  pScreenLayout->addWidget(pAttemptLoginButton);
}

LogInView::~LogInView() {}

void LogInView::logInAttempted() { emit notifyOfLogInAttempt(); }

QString LogInView::getEnteredUsername() { return pUsernameEntryField->text(); }

QString LogInView::getEnteredPassword() { return pPasswordEntryField->text(); }

void LogInView::clear() {
  pUsernameEntryField->setText("");
  pPasswordEntryField->setText("");
}
