// This class is a custom container of display widgets. It will either
// display Employees, Customers, or stocks depending on the constructed type.
// It is scrollable, ensuring that for large amounts of data to display, it
// will not go past the limits of the window.

// TODO:
// THIS CLASS IS NOT YET FINISHED, REQUIRING FORMATING AND SCROLL FUNCTIONALITY.
// Additionally considering turning this into a template class

#ifndef SCROLLABLE_CONTAINER_H
#define SCROLLABLE_CONTAINER_H

#include "CustomerDisplayItem.h"
#include "InvestmentDisplayItem.h"
#include "TraderDisplayItem.h"

#include "Customer.h"
#include "Employee.h"
#include "Investment.h"

#include <QVBoxLayout>
#include <QWidget>
#include <vector>

// Used to define what type of container an instance of this class is
enum CONTAINER_TYPE { CUSTOMERS, TRADERS, STOCKS };

class ScrollableContainer : public QWidget {
  // Necessary macro to use signals and slots
  Q_OBJECT

private:
  // Member variables
  CONTAINER_TYPE containerType;
  QVBoxLayout *pLayout;

  // Lists for display, only one list will be used depending on the type of
  // container constructed. Pointers are used to prevent unnecessary copying of
  // information
  std::vector<CustomerDisplayItem *> customerDisplayList;
  std::vector<TraderDisplayItem *> employeeDisplayList;
  std::vector<InvestmentDisplayItem *> investmentDisplayList;

  // This private function is used when refreshing the screen to clear
  // out stale displayed widgets.
  void clearDisplay();
  void displayList();

public:
  // Declare the container type upon construction
  ScrollableContainer(CONTAINER_TYPE containerType);
  ~ScrollableContainer();

  // Overloaded functions, each does the same thing of adding the display list
  // to the member variable as appropriate.
  void addDisplayList(std::vector<Customer> customers);
  void addDisplayList(std::vector<Employee> employees);
  void addDisplayList(std::vector<Investment> investments);

  // Used whenever the display list changes
  void refreshDisplayList(std::vector<Customer> customers);
  void refreshDisplayList(std::vector<Employee> employees);
  void refreshDisplayList(std::vector<Investment> investments);

public slots:
  void listenForCustomerItemSelection(
      int id); // Connected to each customer display item
  void listenForEmployeeItemSelection(
      int id); // Connected to each employee display item
  // TODO: Stock display items

signals:
  void notifyOfCustomerItemSelection(
      int id); // Notifies the appropriate controller that an item was selected
  void notifyOfEmployeeItemSelection(
      int id); // Notifies the appropirate controller that an item was selected
  // TODO: stock display items
};

#endif
