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

private:
  QString enteredUsername;
  QString enteredPassword;
  QString title;
  QLabel *pTitleLabel;
  QLineEdit *pUsernameEntryField;
  QLineEdit *pPasswordEntryField;
  QPushButton *pAttemptLoginButton;
  QVBoxLayout *pScreenLayout;

public:
  LogInView();
  ~LogInView();
  QString getEnteredUsername();
  QString getEnteredPassword();
  void clear();

public slots:
  void logInAttempted();

signals:
  void notifyOfLogInAttempt();
};

#endif
