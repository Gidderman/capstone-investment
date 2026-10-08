// This class handles the display of the log in screen, accepting a username and
// password as inputs.

#ifndef LOG_IN_VIEW_H
#define LOG_IN_VIEW_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class LogInView : public QWidget {
  // Necessary macro for using signals and slots
  Q_OBJECT

private:
  // These variables will be removed
  QString title;

  // Display componenets
  QLabel *pTitleLabel;
  QLabel *pDescriptionText;
  QLabel *pWarningText;
  QLabel *pPasswordLabel;
  QLabel *pUsernameLabel;
  QLabel *pPasswordVerificationLabel;
  QLineEdit *pUsernameEntryField;
  QLineEdit *pPasswordEntryField;
  QLineEdit *pPasswordVerificationField;

  QPushButton *pNextButton;
  QPushButton *pAttemptLoginButton;
  QPushButton *pCreatePasswordButton;
  QPushButton *pBackButton;

  QVBoxLayout *pPasswordLayout;
  QVBoxLayout *pUsernameLayout;
  QVBoxLayout *pPasswordVerificationLayout;
  QVBoxLayout *pLayout;

public:
  LogInView();
  ~LogInView();
  void runUsernameScreen();
  void runPasswordScreen();
  void runCreatePasswordScreen();
  void displayWarningText(QString text);
  void clear();

  void paintEvent(QPaintEvent *);

public slots:
  void usernameEntered();
  void logInAttempted(); // Connected to the Log In button
  void initiatePasswordCreation();
  void goBack();

  void passwordVerificationFieldChanged();

signals:
  void notifyOfUsernameEntry(QString username);
  void
  notifyOfLogInAttempt(QString password); // Informs the LogInController that a
                                          // log in attempt was made.

  // This is emmitted when the user clicks create password
  void notifyOfPasswordCreation(QString password, QString passwordVerification);
  void userRequestsGoingBack();

  // Emitted whenever the password verification field changes for a quick
  // verification on if the entered passwords match.
  void notifyOfPasswordVerificationFieldChange(QString password,
                                               QString passwordVerification);
};

#endif
