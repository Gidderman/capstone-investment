// This class implements AdminController.h, which contains a summary of the
// purpose of the class.

#include "AdminController.h"
#include "AdminMainView.h"
#include "Authorizer.h"
#include "Customer.h"
#include "CustomerDisplayItem.h"
#include "ErrorWindow.h"
#include "TraderCreationView.h"
#include "TraderDisplayItem.h"
#include "WarningWindow.h"

#include <exception>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

AdminController::AdminController(AdminService &adminService)
    : adminService(adminService) {};

AdminController::~AdminController() {}

// This is the entry point into the class
void AdminController::run(int loggedInAccountId, Authorizer *authorizer) {

  std::cout << "AdminController::run - entering" << std::endl;

  // The authorizer pointer contains the allowed role based on the logged in
  // user. When we call the authorize user function, pass in the role required
  // to access the admin account, and it compares that to its stored role. If
  // they do not match, it returns false.
  if (!authorizer->authorizeUser(ADMIN)) {
    // TODO: Throw and exception
  }

  std::cout << "AdminController::run - authrized user confirmed" << std::endl;

  this->loggedInAccountId = loggedInAccountId;

  std::vector<Employee> allEmployees;
  std::vector<Customer> allCustomers;

  try {
    allEmployees = adminService.getAllEmployees();
    allCustomers = adminService.getAllCustomers();
  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }

  std::cout << "AdminController::run - fetched employees and customers"
            << std::endl;

  // We initialize the three screens that will be accessed, passing in the
  // display lists to the Admin Main View.
  pAdminMainView = new AdminMainView(allCustomers, allEmployees);
  pTraderCreationView = new TraderCreationView();

  std::cout << "AdminController::run - initialized admin main view and trader "
               "creation view"
            << std::endl;

  // We get a list of all the employee names so customer accounts can be
  // assigned employees to manage them
  std::vector<QString> employeeNames;
  for (Employee employee : allEmployees) {
    employeeNames.push_back(QString::fromStdString(employee.lastName + ", " +
                                                   employee.firstName.at(0)));
  }

  pCustomerCreationView = new CustomerCreationView(employeeNames);

  std::cout << "AdminController::run - Initialized customer creation view"
            << std::endl;

  // Connect all the Admin Main View signals to the applicable slots.
  connect(pAdminMainView, &AdminMainView::notifyOfLogOut, this,
          &AdminController::listenForLogOut);
  connect(pAdminMainView, &AdminMainView::notifyOfEmployeeCreation, this,
          &AdminController::listenForEmployeeCreation);
  connect(pAdminMainView, &AdminMainView::notifyOfEmployeeEdit, this,
          &AdminController::listenForEmployeeEdit);
  connect(pAdminMainView, &AdminMainView::notifyOfEmployeeDeletion, this,
          &AdminController::listenForEmployeeDeletion);
  connect(pAdminMainView, &AdminMainView::notifyOfCustomerCreation, this,
          &AdminController::listenForCustomerCreation);
  connect(pAdminMainView, &AdminMainView::notifyOfCustomerEdit, this,
          &AdminController::listenForCustomerEdit);
  connect(pAdminMainView, &AdminMainView::notifyOfCustomerDeletion, this,
          &AdminController::listenForCustomerDeletion);
  connect(pTraderCreationView, &TraderCreationView::notifyOfEmployeeCreation,
          this, &AdminController::listenForEmployeeActionConfirmation);
  connect(pTraderCreationView, &TraderCreationView::notifyOfCancellation, this,
          &AdminController::listenForEmployeeActionCancel);
  connect(pTraderCreationView, &TraderCreationView::notifyOfAccountUnlock, this,
          &AdminController::listenForEmployeeAccountUnlock);
  connect(pCustomerCreationView,
          &CustomerCreationView::notifyOfCustomerCreation, this,
          &AdminController::listenForCustomerActionConfirmation);
  connect(pCustomerCreationView, &CustomerCreationView::notifyOfCreationCancel,
          this, &AdminController::listenForCustomerActionCancel);

  std::cout << "AdminController::run - connected all signals and slots"
            << std::endl;

  // TODO: Check these signal and slot names and make sure they make sense and
  // aren't redundent.

  pAdminMainView->show(); // Display the main screen

  std::cout << "AdminController::run - exiting" << std::endl;
}

// We are creating a new employee.
void AdminController::executeEmployeeCreation() {
  try {
    pTraderCreationView->run();
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// We are editing a employee.
void AdminController::executeEmployeeEdit(Employee employeeToBeEdited) {
  try {
    pTraderCreationView->run(
        formatIndividualEmployeeForDisplay(employeeToBeEdited));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// We are deleteing an existing employee, first throwing a warning window.
void AdminController::executeEmployeeDeletion() {
  pDeleteWarning = new WarningWindow();

  int warningWindowChoice = pDeleteWarning->exec();
  switch (warningWindowChoice) {
  case QMessageBox::Cancel:
    std::cout << warningWindowChoice << " DID NOT DELETE" << std::endl;
    return;
  case QMessageBox::Apply:
    // User confirmed deletion.
    // TODO: Actual functionality
    std::cout << warningWindowChoice << " DELETED" << std::endl;
    break;
  default:
    // TODO: Actual functionality
    std::cout << warningWindowChoice << " HIT THE DEFAULT" << std::endl;
    break;
  }

  // Free the memory
  delete pDeleteWarning;
  pDeleteWarning = nullptr;
}

// We are creating a new customer
void AdminController::executeCustomerCreation() {
  try {
    pCustomerCreationView->run();
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// We are editing an existing customer
void AdminController::executeCustomerEdit(Customer customer) {
  try {
    pCustomerCreationView->run(formatIndividualCustomerForDisplay(customer));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// We are deleting an existing customer, first throwing a warning window
void AdminController::executeCustomerDeletion() {
  pDeleteWarning = new WarningWindow();

  int warningWindowChoice = pDeleteWarning->exec();
  switch (warningWindowChoice) {
  case QMessageBox::Cancel:
    // User cancelled...
    // TODO: actual functionality
    std::cout << warningWindowChoice << " DID NOT DELETE" << std::endl;
    break;
  case QMessageBox::Apply:
    // User confirmed deletion.
    // TODO: actual functionality
    std::cout << warningWindowChoice << " DELETED" << std::endl;
    break;
  default:
    // TODO: actual functionality
    std::cout << warningWindowChoice << " HIT THE DEFAULT" << std::endl;
    break;
  }

  delete pDeleteWarning;
  pDeleteWarning = nullptr;
}

// We have either confirmed the creation of a new employee or
// saved changes to an existing employee
void AdminController::executeEmployeeAction(Employee employee) {
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

    // TODO: Update the list of assignable employees for customer creation after
    // a new employee is created.

    // Refresh display to show up to date information.
    pAdminMainView->refreshEmployees(adminService.getAllEmployees());
  } catch (std::logic_error &e) {
    displayError(QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    displayError(QString::fromStdString(e.what()));
  }
}

// We are unlocking and employee account
void AdminController::executeEmployeeAccountUnlock() {} // TODO: this

// We are cancelling the creation or edit of an employee
void AdminController::cancelEmployeeAction() { pTraderCreationView->end(); }

// We have confirmed the creation of a new customer or saved changes to and
// existing customer
void AdminController::executeCustomerAction(Customer customer) {
  // We determine if this customer is an existing one, or a new one by
  // checking the ID. If it has an existing ID we call the update function,
  // otherwise we create the customer.
  try {
    std::cout << "AdminController::executeCustomerAciton - entered"
              << std::endl;

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
  returnData.push_back(QString::number(
      customer.uninvestedFunds)); // TODO: This is a placeholder, the screen
                                  // should not display initial investment when
                                  // editing an customer
  if (customer.accountType == RETIREMENT) {
    returnData.push_back(QString("Retirement"));
  } else {
    returnData.push_back(QString("Brokerage"));
  }
  Employee managingEmployee = adminService.getEmployeeById(customer.accountID);
  returnData.push_back(QString::fromStdString(
      managingEmployee.lastName + ", " + managingEmployee.firstName.at(0)));

  return returnData;
}

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
void AdminController::listenForEmployeeCreation() { executeEmployeeCreation(); }

// Connected to a double click on an existing employee. We additionally pull the
// existing employee from our employeeList based on the returned ID.
void AdminController::listenForEmployeeEdit(int id) {
  Employee employeeToEdit = adminService.getEmployeeById(id);

  executeEmployeeEdit(employeeToEdit);
}

// Connected to the Admin Main View Delete Employee button
void AdminController::listenForEmployeeDeletion() { executeEmployeeDeletion(); }

// Connected to the AdminMainView create customer button.
void AdminController::listenForCustomerCreation() { executeCustomerCreation(); }

// Connected to a double click on an existing customer
void AdminController::listenForCustomerEdit(int id) {
  Customer customerToEdit = adminService.getCustomerById(id);

  executeCustomerEdit(customerToEdit);
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

  pTraderCreationView->end();

  executeEmployeeAction(inputtedEmployee);
}

// Connected to the CreateTraderView cancel button
void AdminController::listenForEmployeeActionCancel() {
  pTraderCreationView->end();
  cancelEmployeeAction();
}

// Connected to the CreateTraderView unlock account button
void AdminController::listenForEmployeeAccountUnlock() {
  pTraderCreationView->end();
  executeEmployeeAccountUnlock();
}

// Connected to the CreateCustomerView Create Customer/Save Changes button
void AdminController::listenForCustomerActionConfirmation(
    std::vector<QString> customer) {
  std::cout << "AdminController::listenForCustomerActionConfirmation - entering"
            << std::endl;

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

  std::cout << "Name string: " << employeeLastName << std::endl;

  inputtedCustomer.accountID =
      adminService.getEmployeeByName(employeeLastName).accountID;
  std::cout << "Employee ID now assigned: " << inputtedCustomer.accountID
            << std::endl;

  pCustomerCreationView->end();

  executeCustomerAction(inputtedCustomer);

  std::cout << "AdminController::listenForCustomerActionConfirmation - exiting"
            << std::endl;
}

// Connected to the CreateCustomerView cancel button.
void AdminController::listenForCustomerActionCancel() {
  cancelCustomerAction();
}
