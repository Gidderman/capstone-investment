// This class handles the display of the main Trader Window, which is accessed
// from the log in screen. It handles all input, which includes logging out and
// selecting a given displayed customer.

// TODO:
// THIS CLASS IS INCOMPLETE, REQUIRING DISPLAY FORMATTING

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
  // This macro is necessary to utilize signals and slots
  Q_OBJECT

private:
  // Display components
  QLabel *pPageTitleDisplay;
  QLabel *pTraderNameDisplay;
  QLabel *pTraderAccountNumber;
  QLabel *pNumAccountsManagedDisplay;

  QPushButton *pLogOutButton;

  // This is a custom widget that displays customers in a specified
  // format and being scrollable.
  ScrollableContainer *pManagedCustomersList;

  // Layouts
  QVBoxLayout *pTraderInformationLayout;
  QVBoxLayout *pLogOutButtonAndCustomerListLayout;
  QHBoxLayout *pMainLayout;

public:
  // We pass in the info that we want displayed with the constructor
  TraderMainView(std::vector<QString> employeeDisplayInfo,
                 std::vector<Customer> managedCustomers);
  ~TraderMainView();

public slots:
  void
  listenForLogOut(); // Connected to the log out button on the TraderMainView
  void listenForCustomerSelection(
      int id); // Connected to individual customers that are displayed on screen

signals:
  void notifyOfLogOut(); // Notifies the TraderController of a log out
  void notifyOfCustomerSelection(
      int id); // Notifies the trader controller that a customer was selected
};

#endif
