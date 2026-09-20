// This class handles the display of the log in screen, accepting a username and
// password as inputs.

// TODO:
// THIS CLASS IS NOT COMPLETE, REQUIRES WORK FOR DISPLAY FORMATTING

#ifndef LOG_IN_VIEW_H
#define LOG_IN_VIEW_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <qboxlayout.h>

class LogInView : public QWidget {
  // Necessary macro for using signals and slots
  Q_OBJECT

  // TODO: Refactor to not contain variables and use signals
  // to pass log in information to the controller
private:
  // These variables will be removed
  QString enteredUsername;
  QString enteredPassword;
  QString title;

  // Display componenets
  QLabel *pTitleLabel;
  QLineEdit *pUsernameEntryField;
  QLineEdit *pPasswordEntryField;
  QPushButton *pAttemptLoginButton;
  QVBoxLayout *pScreenLayout;

public:
  LogInView();
  ~LogInView();
  // TODO: remove these functions after implementing signal
  // functionality
  QString getEnteredUsername();
  QString getEnteredPassword();
  void clear();

public slots:
  void logInAttempted(); // Connected to the Log In button

signals:
  void notifyOfLogInAttempt(); // Informs the LogInController that a log in
                               // attempt was made.
};

#endif
