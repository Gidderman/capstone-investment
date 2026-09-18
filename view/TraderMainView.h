#ifndef TRADER_MAIN_VIEW_H
#define TRADER_MAIN_VIEW_H

#include "CustomerDisplayItem.h"
#include "ScrollableContainer.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

class TraderMainView : public QWidget {
  Q_OBJECT

private:
  QLabel *pPageTitleDisplay;
  QLabel *pTraderNameDisplay;
  QLabel *pTraderAccountNumber;
  QLabel *pNumAccountsManagedDisplay;

  QPushButton *pLogOutButton;

  ScrollableContainer *pManagedCustomersList;

  QVBoxLayout *pTraderInformationLayout;
  QVBoxLayout *pLogOutButtonAndCustomerListLayout;
  QHBoxLayout *pMainLayout;

public:
  TraderMainView(std::vector<CustomerDisplayItem *> *managedCustomers);
  ~TraderMainView();

public slots:
  void listenForLogOut();
  void listenForCustomerSelection();

signals:
  void notifyOfLogOut();
  void notifyOfCustomerSelection();
};

#endif
