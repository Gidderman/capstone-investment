#ifndef ADMIN_MAIN_VIEW_H
#define ADMIN_MAIN_VIEW_H

#include "CustomerDisplayItem.h"
#include "ScrollableContainer.h"
#include "TraderDisplayItem.h"
#include "WarningWindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <vector>

class AdminMainView : public QWidget {
  Q_OBJECT

private:
  ScrollableContainer *pTraderList;
  ScrollableContainer *pCustomerList;

  QLabel *pTitle;
  QPushButton *pCreateTraderButton;
  QPushButton *pDeleteTraderButton;
  QPushButton *pCreateCustomerButton;
  QPushButton *pDeleteCustomerButton;
  QPushButton *pLogOutButton;

  QHBoxLayout *pMainLayout;
  QVBoxLayout *pTitleAndTraderLayout;
  QVBoxLayout *pLogOutAndCustomerLayout;
  QHBoxLayout *pTraderButtonLayout;
  QHBoxLayout *pCustomerButtonLayout;

public:
  AdminMainView(std::vector<CustomerDisplayItem *> *totalCustomers,
                std::vector<TraderDisplayItem *> *totalTraders);
  ~AdminMainView();

public slots:
  void logOutInitiated();
  void createEmployeeInitiated();
  void editEmployeeInitiated(int id);
  void deleteEmployeeInitiated();
  void createCustomerInitiated();
  void editCustomerInitiated();
  void deleteCustomerInitiated();

signals:
  void notifyOfLogOut();
  void notifyOfEmployeeCreation();
  void notifyOfEmployeeEdit(int id);
  void notifyOfEmployeeDeletion();
  void notifyOfCustomerCreation();
  void notifyOfCustomerEdit();
  void notifyOfCustomerDeletion();
};

#endif
