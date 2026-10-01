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
#include <qpushbutton.h>

class LogInView : public QWidget {
  // Necessary macro for using signals and slots
  Q_OBJECT

  // TODO: Refactor to not contain variables and use signals
  // to pass log in information to the controller
private:
  // These variables will be removed
  QString title;

  // Display componenets
  QLabel *pTitleLabel;
  QLabel *pDescriptionText;
  QLabel *pWarningText;
  QLineEdit *pUsernameEntryField;
  QLineEdit *pPasswordEntryField;
  QLineEdit *pPasswordVerificationField;

  QPushButton *pNextButton;
  QPushButton *pAttemptLoginButton;
  QPushButton *pCreatePasswordButton;

  QVBoxLayout *pUsernameScreenLayout;
  QVBoxLayout *pPasswordEntryLayout;
  QVBoxLayout *pCreatePasswordLayout;

public:
  LogInView();
  ~LogInView();
  // TODO: remove these functions after implementing signal
  // functionality
  void runUsernameScreen();
  void runPasswordScreen();
  void runCreatePasswordScreen();
  void displayWarningText(QString text);
  void clear();

public slots:
  void usernameEntered();
  void logInAttempted(); // Connected to the Log In button
  void initiatePasswordCreation();

signals:
  void notifyOfUsernameEntry(QString username);
  void
  notifyOfLogInAttempt(QString password); // Informs the LogInController that a
                                          // log in attempt was made.
  void notifyOfPasswordCreation(QString password, QString passwordVerification);
};

#endif
