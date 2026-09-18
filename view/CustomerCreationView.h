#ifndef CUSTOMER_CREATION_VIEW_H
#define CUSTOMER_CREATION_VIEW_H

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class CustomerCreationView : public QWidget {
  Q_OBJECT

private:
  QLineEdit *pFirstNameEntry;
  QLineEdit *pLastNameEntry;
  QLineEdit *pPhoneNumberEntry;
  QLineEdit *pEmailEntry;
  QLineEdit *pInitialInvestmentEntry;
  QComboBox *pAccountTypeEntry;

  QPushButton *pCreateCustomerButton;
  QPushButton *pCancelCreationButton;

  QVBoxLayout *layout;

public:
  CustomerCreationView();
  ~CustomerCreationView();
  void run();
  void end();
  QString getFirstName();
  QString getLastName();
  QString getPhoneNumber();
  QString getEmail();
  QString getInitialInvestment();
  int getAccountType();

public slots:
  void listenForCustomerCreation();
  void listenForCreationCancel();

signals:
  void notifyOfCustomerCreation();
  void notifyOfCreationCancel();
};

#endif
