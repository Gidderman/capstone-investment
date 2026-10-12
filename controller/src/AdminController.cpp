// This class implements AdminController.h, which contains a summary of the
// its purpose.

#include "AdminController.h"
#include "AdminMainView.h"
#include "Authorizer.h"
#include "Customer.h"
#include "EmployeeCreationView.h"
#include "ErrorWindow.h"

#include <exception>
#include <stdexcept>
#include <string>

// To prevent duplicated connections to the database from multiple copies
// of the DataManager and CRUDManager, we pass by reference an already
// instantiated adminService class. The MasterController has the responsibility
// of creating all services, the DataManager, and the CRUDManager.
AdminController::AdminController(AdminService &adminService)
    : adminService(adminService) {};

AdminController::~AdminController() {}

// This is the entry point into the class
void AdminController::run(Authorizer *authorizer) {

  // The authorizer pointer contains the allowed role based on the logged in
  // user. When we call the authorize user function, pass in the role required
  // to access the admin account, and it compares that to its stored role. If
  // they do not match, it returns false.
  if (!authorizer->authorizeUser(ADMIN)) {
    throw std::logic_error(
        "Unauthorized user attempted access to the Admin Page");
  }

  std::vector<Employee> allEmployees;
  std::vector<Customer> allCustomers;

  try {
    allEmployees = adminService.getAllEmployees();
    allCustomers = adminService.getAllCustomers();
  } catch (std::logic_error &e) {
    // std::logic_error is the standard exception thrown when there is
    // an issue in the service layer or below
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }

  // We initialize the three screens that will be accessed, passing in the
  // display lists to the Admin Main View.
  pAdminMainView = new AdminMainView(allCustomers, allEmployees);
  pEmployeeCreationView = new EmployeeCreationView();

  // We get a list of all the employee names so customer accounts can be
  // assigned employees to manage them
  std::vector<QString> employeeNames;
  for (Employee employee : allEmployees) {
    if (employee.role == TRADER) {
      employeeNames.push_back(QString::fromStdString(employee.lastName + ", " +
                                                     employee.firstName.at(0)));
    }
  }

  pCustomerCreationView = new CustomerCreationView(employeeNames);

  // Connect all the Admin Main View signals to the applicable slots.
  connect(pAdminMainView, &AdminMainView::notifyOfLogOut, this,
          &AdminController::listenForLogOut);
  connect(pAdminMainView, &AdminMainView::notifyOfCreateEmployeeButtonPressed,
          this, &AdminController::listenForInitiationOfEmployeeCreation);
  connect(pAdminMainView, &AdminMainView::notifyOfEmployeeSelection, this,
          &AdminController::listenForEmployeeSelection);
  connect(pAdminMainView, &AdminMainView::notifyOfEmployeeEdit, this,
          &AdminController::listenForEmployeeEdit);
  connect(pAdminMainView, &AdminMainView::notifyOfEmployeeDeactivation, this,
          &AdminController::listenForEmployeeDeactivation);
  connect(pAdminMainView, &AdminMainView::notifyOfCreateCustomerButtonPressed,
          this, &AdminController::listenForInitiationOfCustomerCreation);
  connect(pAdminMainView, &AdminMainView::notifyOfCustomerSelection, this,
          &AdminController::listenForCustomerSelection);
  connect(pAdminMainView, &AdminMainView::notifyOfCustomerEdit, this,
          &AdminController::listenForCustomerEdit);
  connect(pAdminMainView, &AdminMainView::notifyOfCustomerDeletion, this,
          &AdminController::listenForCustomerDeletion);

  connect(pEmployeeCreationView,
          &EmployeeCreationView::notifyOfEmployeeCreation, this,
          &AdminController::listenForEmployeeActionConfirmation);
  connect(pEmployeeCreationView, &EmployeeCreationView::notifyOfCancellation,
          this, &AdminController::listenForEmployeeActionCancel);
  connect(pEmployeeCreationView, &EmployeeCreationView::notifyOfAccountUnlock,
          this, &AdminController::listenForEmployeeAccountUnlock);
  connect(pEmployeeCreationView, &EmployeeCreationView::notifyOfPasswordReset,
          this, &AdminController::listenForEmployeePasswordReset);

  connect(pCustomerCreationView,
          &CustomerCreationView::notifyOfCustomerCreation, this,
          &AdminController::listenForCustomerActionConfirmation);
  connect(pCustomerCreationView, &CustomerCreationView::notifyOfCreationCancel,
          this, &AdminController::listenForCustomerActionCancel);

  pAdminMainView->show(); // Display the main screen
}

// Opens the Create Employee window
void AdminController::initiateEmployeeCreation() {
  try {
    pEmployeeCreationView->run();
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// Opens the Create Employee window with the selected employee information
// prepopulated
void AdminController::initiateEmployeeEdit(Employee employeeToBeEdited) {
  try {
    pEmployeeCreationView->run(
        formatIndividualEmployeeForDisplay(employeeToBeEdited));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// We are deactivating an existing employee, first throwing a warning window.
// We deactivate instead of deleting to maintain any logs that may be
// implemented as well as historic integrity of actions taken by this employee.
void AdminController::executeEmployeeDeactivation() {
  pDeleteWarning = new WarningWindow(
      QString("WARNING: You are about to deactivate an employee account. They "
              "will be unable to log in after this operation."));

  int warningWindowChoice = pDeleteWarning->exec();
  switch (warningWindowChoice) {
  case QMessageBox::Cancel:
    return;

  case QMessageBox::Apply:
    // User confirmed deactivation.
    try {
      adminService.deactivateSelectedEmployee();
    } catch (std::logic_error &e) {
      displayError(e.what());
    } catch (std::exception &e) {
      displayError(e.what());
    }
    break;

  default:
    break;
  }

  // Free the memory
  delete pDeleteWarning;
  pDeleteWarning = nullptr;
}

// Open the Customer Creation Window
void AdminController::initiateCustomerCreation() {
  try {
    pCustomerCreationView->run();
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// Open the Customer Creation window with customer information
// prepopulated for editing
void AdminController::initiateCustomerEdit(Customer customer) {
  try {
    pCustomerCreationView->run(formatIndividualCustomerForDisplay(customer));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// We are deleting an existing customer, first throwing a warning window
void AdminController::executeCustomerDeletion() {
  pDeleteWarning =
      new WarningWindow(QString("WARNING: You are about to permanently delete "
                                "a customer. This action cannot be undone."));

  int warningWindowChoice = pDeleteWarning->exec();
  switch (warningWindowChoice) {
  case QMessageBox::Cancel:
    // User cancelled...
    break;

  case QMessageBox::Apply:
    // User confirmed deletion.
    try {
      adminService.deleteSelectedCustomer();

      // Employee display needs to be refreshed to update the number of managed
      // customers per employee.
      pAdminMainView->refreshEmployees(adminService.getAllEmployees());
      pAdminMainView->refreshCustomers(adminService.getAllCustomers());
    } catch (std::logic_error &e) {
      displayError(e.what());
    } catch (std::exception &e) {
      displayError(e.what());
    }
    break;

  default:
    break;
  }

  delete pDeleteWarning;
  pDeleteWarning = nullptr;
}

// We have either confirmed the creation of a new employee or
// saved changes to an existing employee from the Employee creation window
void AdminController::executeEmployeeAction(Employee employee) {
  // Verify there is input
  if (employee.firstName.size() == 0 || employee.lastName.size() == 0) {
    displayError(
        QString("Please complete all fields. Requested action rejected."));
    return;
  }

  // We determine if this employee is an existing one, or a new one by
  // checking the ID. If it has an existing ID we call the update function,
  // otherwise we create the employee.
  try {
    Employee employeeToEdit;

    if (employee.accountID != -1) {
      employeeToEdit = adminService.getEmployeeById(employee.accountID);
      adminService.editEmployee(employeeToEdit, employee);

    } else {
      adminService.createEmployee(employee);
    }

    // Refresh display to show up to date information.
    pAdminMainView->refreshEmployees(adminService.getAllEmployees());
    // We refresh customer display as well so it will display updated lists of
    // available employees to assign to the account
    pAdminMainView->refreshCustomers(adminService.getAllCustomers());
  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

void AdminController::executeEmployeeAccountUnlock(int id) {
  try {
    adminService.unlockAccount(id);
  } catch (std::logic_error &e) {
    displayError(e.what());
  } catch (std::exception &e) {
    displayError(e.what());
  }
}

void AdminController::executeEmployeePasswordReset(int id) {
  try {
    adminService.resetPassword(id);
  } catch (std::logic_error &e) {
    displayError(e.what());
  } catch (std::exception &e) {
    displayError(e.what());
  }
}

// We are cancelling the creation or edit of an employee
void AdminController::cancelEmployeeAction() { pEmployeeCreationView->end(); }

// We have confirmed the creation of a new customer or saved changes to and
// existing customer
void AdminController::executeCustomerAction(Customer customer) {
  if (customer.firstName.empty() || customer.lastName.empty() ||
      customer.phoneNum.empty() || customer.email.empty() ||
      customer.uninvestedFunds <= 0.0f) {
    displayError(
        QString("Please complete all fields. Requested action rejected."));
    return;
  }

  // We determine if this customer is an existing one, or a new one by
  // checking the ID. If it has an existing ID we call the update function,
  // otherwise we create the customer.
  try {
    Customer customerToEdit;
    if (customer.customerID != -1) {
      customerToEdit = adminService.getCustomerById(customer.customerID);
      adminService.editCustomer(customerToEdit, customer);
    } else {
      adminService.createCustomer(customer);
    }

    // Refresh our display to show up to date information. Employee display must
    // be updated as well to update the number of managed accounts
    pAdminMainView->refreshCustomers(adminService.getAllCustomers());
    pAdminMainView->refreshEmployees(adminService.getAllEmployees());

  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// We have cancelled the creation or edit of an employee
void AdminController::cancelCustomerAction() { pCustomerCreationView->end(); }

//*********************PRIVATE FUNCTIONS*****************************

// We format an individual employee for display within the Employee Creation
// Window. All values must be in QStrings.
std::vector<QString>
AdminController::formatIndividualEmployeeForDisplay(Employee employee) {
  std::vector<QString> returnData;
  returnData.push_back(QString::fromStdString(employee.firstName));
  returnData.push_back(QString::fromStdString(employee.lastName));

  if (employee.role == ADMIN) {
    returnData.push_back(QString("Admin"));
  } else {
    returnData.push_back(QString("Trader"));
  }
  returnData.push_back(QString::number(employee.accountID));

  return returnData;
}

// We format an individual customer for display within the customer Creation
// Window. All values must be in QStrings.
std::vector<QString>
AdminController::formatIndividualCustomerForDisplay(Customer customer) {
  std::vector<QString> returnData;
  returnData.push_back(QString::number(customer.customerID));
  returnData.push_back(QString::fromStdString(customer.firstName));
  returnData.push_back(QString::fromStdString(customer.lastName));
  returnData.push_back(QString::fromStdString(customer.phoneNum));
  returnData.push_back(QString::fromStdString(customer.email));
  returnData.push_back(QString::number(customer.uninvestedFunds, 'f', 2));
  if (customer.accountType == RETIREMENT) {
    returnData.push_back(QString("Retirement"));
  } else {
    returnData.push_back(QString("Brokerage"));
  }
  if (customer.accountID != 0) {
    Employee managingEmployee =
        adminService.getEmployeeById(customer.accountID);
    returnData.push_back(QString::fromStdString(
        managingEmployee.lastName + ", " + managingEmployee.firstName.at(0)));
  } else {
    returnData.push_back(QString("None"));
  }

  return returnData;
}

// Displays a pop up error window with the passed in message.
void AdminController::displayError(QString errorText) {
  ErrorWindow *errorWindow = new ErrorWindow(errorText);
  int windowAcknowledged = errorWindow->exec();
  delete errorWindow;
}

//*********************SLOTS**************************************
// Connected to the AdminMainView logout button
void AdminController::listenForLogOut() {
  pAdminMainView->hide();
  emit informMasterControllerOfLogOut();
}

// Connected to the AdminMainView Create Employee button
void AdminController::listenForInitiationOfEmployeeCreation() {
  initiateEmployeeCreation();
}

// Connected to a double click on an existing employee. We additionally pull the
// existing employee from our employeeList based on the returned ID.
void AdminController::listenForEmployeeEdit(int id) {
  Employee employeeToEdit = adminService.getEmployeeById(id);

  initiateEmployeeEdit(employeeToEdit);
}

// Connected to the Admin Main View Delete Employee button
void AdminController::listenForEmployeeDeactivation() {
  executeEmployeeDeactivation();
}

// Connected to the AdminMainView create customer button.
void AdminController::listenForInitiationOfCustomerCreation() {
  initiateCustomerCreation();
}

// Connected to a double click on an existing customer
void AdminController::listenForCustomerEdit(int id) {
  Customer customerToEdit = adminService.getCustomerById(id);

  initiateCustomerEdit(customerToEdit);
}

// Connected to the AdminMainView delete customer button
void AdminController::listenForCustomerDeletion() { executeCustomerDeletion(); }

// Connected to the CreateTraderView Create Employee/Save Changes button
// If the passed in vector last index is not negative 1 that means it is an
// existing employee that has been edited
void AdminController::listenForEmployeeActionConfirmation(
    std::vector<QString> employee) {
  Employee inputtedEmployee;

  inputtedEmployee.firstName = employee.at(0).toStdString();
  inputtedEmployee.lastName = employee.at(1).toStdString();

  if (employee.at(2) == "Admin" || employee.at(2) == "ADMIN") {
    inputtedEmployee.role = ADMIN;
  } else {
    inputtedEmployee.role = TRADER;
  }
  inputtedEmployee.accountID = employee.at(3).toInt();

  pEmployeeCreationView->end();

  executeEmployeeAction(inputtedEmployee);
}

// Connected to the CreateTraderView cancel button
void AdminController::listenForEmployeeActionCancel() {
  pEmployeeCreationView->end();
  cancelEmployeeAction();
}

// Connected to the CreateTraderView unlock account button
void AdminController::listenForEmployeeAccountUnlock(int id) {
  pEmployeeCreationView->end();
  executeEmployeeAccountUnlock(id);
}

// Connected to the reset password button in CreateTraderView
void AdminController::listenForEmployeePasswordReset(int id) {
  pEmployeeCreationView->end();
  executeEmployeePasswordReset(id);
}

// Connected to the CreateCustomerView Create Customer/Save Changes button
void AdminController::listenForCustomerActionConfirmation(
    std::vector<QString> customer) {

  Customer inputtedCustomer;
  inputtedCustomer.customerID = customer.at(0).toInt();
  inputtedCustomer.firstName = customer.at(1).toStdString();
  inputtedCustomer.lastName = customer.at(2).toStdString();
  inputtedCustomer.phoneNum = customer.at(3).toStdString();
  inputtedCustomer.email = customer.at(4).toStdString();
  inputtedCustomer.uninvestedFunds = customer.at(5).toFloat();
  if (customer.at(6).toStdString() == "Retirement") {
    inputtedCustomer.accountType = RETIREMENT;
  } else {
    inputtedCustomer.accountType = BROKERAGE;
  }
  std::string employeeLastName = customer.at(7).toStdString().substr(
      0, customer.at(7).toStdString().find(','));

  inputtedCustomer.accountID =
      adminService.getEmployeeByName(employeeLastName).accountID;

  pCustomerCreationView->end();

  executeCustomerAction(inputtedCustomer);
}

// Connected to the CreateCustomerView cancel button.
void AdminController::listenForCustomerActionCancel() {
  cancelCustomerAction();
}

// Connected to selecting a single click on a displayed customer
void AdminController::listenForCustomerSelection(int id) {
  adminService.setCurrentlySelectedId(id);
}

// Connected to selecting a single click on a displayed employee
void AdminController::listenForEmployeeSelection(int id) {
  adminService.setCurrentlySelectedId(id);
}
