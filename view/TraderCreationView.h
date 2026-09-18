#ifndef TRADER_CREATION_VIEW_H
#define TRADER_CREATION_VIEW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class TraderCreationView : public QWidget {
  Q_OBJECT

private:
  QLineEdit *pFirstNameEntry;
  QLineEdit *pLastNameEntry;
  QComboBox *pRoleSelection;

  QPushButton *pCreateEmployeeButton;
  QPushButton *pCancelCreationButton;

  QPushButton *pUnlockAccountButton;

  QVBoxLayout *layout;

public:
  TraderCreationView();
  ~TraderCreationView();
  void run();
  QString getFirstName();
  QString getLastName();
  int getRole();

public slots:
  void listenForEmployeeCreation();
  void listenForCreationCancellation();
  void listenForAccountUnlock();

signals:
  void notifyOfEmployeeCreation();
  void notifyOfCancellation();
  void notifyOfAccountUnlock();
};

#endif
