// This class is the Main Screen for the Administrator user. It only handles
// displaying the information and accepting user input. Functions displayed on
// this screen include a list of customers and a list of employees, as well as
// buttons to remove and add employees and customers. Additionally, double
// clicking on a displayed employee or customer will allow the user to edit
// them.

// TODO:
// THERE IS STILL A LOT OF WORK TO DO ON THIS CLASS, SUCH AS HIGHLIGHTING A
// SELECTED CUSTOMER OR EMPLOYEE AND GENERAL DISPLAY FORMATING

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
  // This macro is required to be able to use SLOTS and SIGNALS
  Q_OBJECT

private:
  // The ScrollableContainer is a custom widget used to display customers,
  // employees, or stocks with the ability to scroll.
  ScrollableContainer *pTraderList;
  ScrollableContainer *pCustomerList;

  // Below are the various components of the screen
  QLabel *pTitle;
  QPushButton *pCreateTraderButton;
  QPushButton *pDeleteTraderButton;
  QPushButton *pCreateCustomerButton;
  QPushButton *pDeleteCustomerButton;
  QPushButton *pLogOutButton;

  // Layouts used to format the screen
  QHBoxLayout *pMainLayout;           // Overall layout
  QVBoxLayout *pTitleAndTraderLayout; // Holds the page title and pTraderList
  QVBoxLayout
      *pLogOutAndCustomerLayout; // Holds the log out button and pCustomerList
  QHBoxLayout *pTraderButtonLayout; // Holds the trader create and delete
                                    // buttons
  QHBoxLayout
      *pCustomerButtonLayout; // Holds the customer trader and delete buttons

public:
  // We pass in the two lists used to populate the pTraderList and pCustomerList
  // upon construction. To avoid unnecessary duplicatation of the display data,
  // we pass in pointers.
  AdminMainView(std::vector<CustomerDisplayItem *> totalCustomers,
                std::vector<TraderDisplayItem *> totalTraders);
  ~AdminMainView();
  // This function is used whenever there is a change to the displayed customers
  // or employees.
  void refreshEmployees(std::vector<TraderDisplayItem *> totalTraders);
  void refreshCustomers(std::vector<CustomerDisplayItem *> totalCustomers);

public slots:
  void logOutInitiated();         // Connected to the log out button on the
                                  // MainAdminView
  void createEmployeeInitiated(); // Connected to the Create Employee button in
                                  // the MainAdminView
  void editEmployeeInitiated(
      int id); // Called when the user double clicks on an existing employee
  void deleteEmployeeInitiated(); // Connected to the Delete Employee button in
                                  // the MainAdminView
  void createCustomerInitiated(); // Connected to the Create Customer button in
                                  // the MainAdminView
  void editCustomerInitiated(
      int id); // called when the user double clicks an existing customer
  void deleteCustomerInitiated(); // Connected to the Delete Customer button in
                                  // the MainAdminView

signals:
  void
  notifyOfLogOut(); // Connected to AdminController to notify of log out request
  void notifyOfEmployeeCreation(); // Informs the AdminController that the user
                                   // wants to create a new employee
  void notifyOfEmployeeEdit(int id); // Informs the AdminController that the
                                     // user wants to edit an existing employee
  void notifyOfEmployeeDeletion(); // Informs the AdminController that the user
                                   // is trying to delete an employee
  void notifyOfCustomerCreation(); // Informs the AdminController that the user
                                   // wants to create a new employee
  void notifyOfCustomerEdit(
      int id); // Informs adminController that we are editing a customer
  void notifyOfCustomerDeletion(); // informs the AdminController that the user
                                   // is trying to delete a customer
};

#endif
