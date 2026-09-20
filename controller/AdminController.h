// The AdminController serves the purpose of formating all inputs and outputs
// for the Admin screen, which consists of AdminMainView, TraderCreationView,
// and CustomerCreationView, as well as a seperate warning window that provides
// the user an opportunity to go back when deleting an employee or customer. In
// addition to the formatting function, the AdminController also directs the
// flow of data between the screens and the AdminService class.

#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include "AdminMainView.h"
#include "AdminService.h"
#include "Authorizer.h"
#include "CustomerCreationView.h"
#include "TraderCreationView.h"

#include <QObject>

class AdminController : public QObject {
  // Including the Q_OBJECT macro is necessary for this function to be able to
  // use the Qt SLOTS and SIGNALS functionalities, which is the primary mode of
  // communication from the view to the controller.
  Q_OBJECT

private:
  // Screen declarations. All screens are pointers for ease of interface with
  // Qt, which expects pointers for essentially all operations.
  AdminMainView *pAdminMainView;
  CustomerCreationView *pCustomerCreationView;
  TraderCreationView *pTraderCreationView;

  WarningWindow *pDeleteWarning;

  // Service declaration
  AdminService adminService;

  // The customer and employee list hold a local copy of all employees and
  // customers. This prevents frequent calls to the database, allowing us to
  // pull in all the data in one or two calls that then gets formated for
  // display.
  std::vector<Customer> customerList;
  std::vector<Employee> employeeList;

  // These lists are the customer and employee lists. They hold the same
  // customers and employees as the above two lists, but formatted for display
  // on screen. This is necessary due to the fact that a Customer or Employee
  // structure uses multiple data types within, but the Qt display system
  // expects almost exclusively QStrings. Additionally this places the data in a
  // format that is appropriate for my custom display widget.
  std::vector<CustomerDisplayItem *> customerDisplayList;
  std::vector<TraderDisplayItem *> employeeDisplayList;

  // These are the private functions that the AdminController uses to populate
  // the customerList and employeeList
  std::vector<Customer> getAllCustomers();
  std::vector<Employee> getAllEmployees();

  // These private functions are used to format the data within the customerList
  // and employeeList for display, storing the results in the applicable
  // displayList.
  void formatCustomersForDisplay();
  void formatEmployeesForDisplay();

  // This formats an individual employee or customer for display. This is
  // primarily used when editing an employee or customer and it is needed to
  // display the existing data.
  std::vector<QString> formatIndividualEmployeeForDisplay(Employee employee);
  std::vector<QString> formatIndividualCustomerForDisplay(Customer customer);

  // TODO: Create a function for refreshing displays. One for each list
public:
  AdminController();
  ~AdminController();
  // This function is the entry into the AdminController and is called by the
  // Master Controller after a successful admin log in.
  void run(Authorizer *authorizer);

  // The below functions communicate with the AdminService to send data back to
  // the database.
  void executeEmployeeCreation();
  void executeEmployeeEdit(Employee employeeToBeEdited);
  void executeEmployeeDeletion();
  void executeCustomerCreation();
  void executeCustomerEdit(Customer customer);
  void executeCustomerDeletion();

  // These actions are called from the TraderCreationView and correspond to the
  // buttons there: 'Create Employee/Save Changes' for executeEmployeeAction(),
  //'Unlock Account' for executeEmployeeAccountUnlock(), and 'Cancel' for
  // cancelEmployeeAction()
  void executeEmployeeAction(Employee employee);
  void executeEmployeeAccountUnlock();
  void cancelEmployeeAction();

  // These functions perform the same as above but for the CustomerCreationView.
  void executeCustomerAction(Customer customer);
  void cancelCustomerAction();

public slots:
  // These slots are the communication points between the various screens and
  // the controller.
  void listenForLogOut(); // Connected to the AdminMainView log out button.
  void listenForEmployeeCreation(); // Connected to the AdminMainView Create
                                    // Employee button.
  void listenForEmployeeEdit(
      int id); // Connected a double click on an existing employee
  void listenForEmployeeDeletion();   // Connected to the AdminMainView Remove
                                      // Employee button.
  void listenForCustomerCreation();   // Connected to the AdminMainView Create
                                      // Customer button.
  void listenForCustomerEdit(int id); // Connected to a double click on an
                                      // existing customer.
  void listenForCustomerDeletion();   // Connected to the AdminMainView Remove
                                      // Customer button.

  // While the above slots were related to the AdminMainView, these are related
  // to the CreateTraderView
  void listenForEmployeeActionConfirmation(
      std::vector<QString> employee);   // Connected to the confirm button
  void listenForEmployeeActionCancel(); // Connected to the cancel button
  void
  listenForEmployeeAccountUnlock(); // Connected to the Unlock account button

  // These slots are for the CreateCustomerView
  void listenForCustomerActionConfirmation(
      std::vector<QString> customer);   // Connected to the confirm button
  void listenForCustomerActionCancel(); // Connected to the cancel button

signals:
  void informMasterControllerOfLogOut();
};

#endif
