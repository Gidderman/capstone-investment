// This class implements AdminController.h, which contains a summary of the
// purpose of the class.

#include "AdminController.h"
#include "AdminMainView.h"
#include "Authorizer.h"
#include "Customer.h"
#include "CustomerDisplayItem.h"
#include "TraderCreationView.h"
#include "TraderDisplayItem.h"
#include "WarningWindow.h"

#include <iostream>
#include <ostream>
#include <string>

// TODO: Fix bug where editing a newly created customer crashes the program
// TODO: Fix bug where changing managing employee appears to not save
// TODO: Fix issue where the number of accounts managed does not display

AdminController::AdminController() {};

AdminController::~AdminController() {}

// This is the entry point into the class
void AdminController::run(int loggedInAccountId, Authorizer *authorizer) {
  // The authorizer pointer contains the allowed role based on the logged in
  // user. When we call the authorize user function, pass in the role required
  // to access the admin account, and it compares that to its stored role. If
  // they do not match, it returns false.
  if (!authorizer->authorizeUser(ADMIN)) {
    // TODO: Throw and exception
  }

  this->loggedInAccountId = loggedInAccountId;

  // This call is stored in a variable to limit the number of database calls
  // made
  std::vector<Employee> allEmployees = adminService.getAllEmployees();

  // We initialize the three screens that will be accessed, passing in the
  // display lists to the Admin Main View.
  pAdminMainView =
      new AdminMainView(adminService.getAllCustomers(), allEmployees);
  pTraderCreationView = new TraderCreationView();

  // We get a list of all the employee names so customer accounts can be
  // assigned employees to manage them
  std::vector<QString> employeeNames;
  for (Employee employee : allEmployees) {
    employeeNames.push_back(QString::fromStdString(employee.lastName + ", " +
                                                   employee.firstName.at(0)));
  }

  pCustomerCreationView = new CustomerCreationView(employeeNames);

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

  // TODO: Check these signal and slot names and make sure they make sense and
  // aren't redundent.

  pAdminMainView->show(); // Display the main screen
}

// We are creating a new employee.
void AdminController::executeEmployeeCreation() { pTraderCreationView->run(); }

// We are editing a new employee.
void AdminController::executeEmployeeEdit(Employee employeeToBeEdited) {
  pTraderCreationView->run(
      formatIndividualEmployeeForDisplay(employeeToBeEdited));
}

// We are deleteing an existing employee, first throwing a warning window.
void AdminController::executeEmployeeDeletion() {
  pDeleteWarning = new WarningWindow();

  int warningWindowChoice = pDeleteWarning->exec();
  switch (warningWindowChoice) {
  case QMessageBox::Cancel:
    // User cancelled...
    // TODO: Actual functionality
    std::cout << warningWindowChoice << " DID NOT DELETE" << std::endl;
    break;
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
  pCustomerCreationView->run();
}

// We are editing an existing customer
void AdminController::executeCustomerEdit(Customer customer) {
  pCustomerCreationView->run(formatIndividualCustomerForDisplay(customer));
}

// We are deleting an exiting customer, first throwing a waring window
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

  // TODO: OPTIMIZE QUERIES WHEN IMPLEMENTING DATABASE

  Employee employeeToEdit;

  if (employee.accountID != -1) {
    for (Employee existingEmployee : adminService.getAllEmployees()) {
      if (employee.accountID == existingEmployee.accountID) {
        employeeToEdit = existingEmployee;
      }
    }
    if (adminService.editEmployee(employeeToEdit, employee)) {
      // TODO: successful edit
    } else {
      // TODO: unsuccesful edit
    }
  } else {
    if (adminService.createEmployee(employee)) {
      // TODO: sucessful edit
    } else {
      // TODO: unsuccesful edit
    }
  }

  // Refresh display to show up to date information.
  pAdminMainView->refreshEmployees(adminService.getAllEmployees());
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

  // TODO: OPTIMIZE QUERIES WHEN IMPLEMENTING DATABASE

  std::cout << "AdminController::executeCustomerAciton - entered" << std::endl;

  Customer customerToEdit;
  if (customer.customerID != -1) {
    for (Customer existingCustomer : adminService.getAllCustomers()) {
      if (customer.customerID == existingCustomer.customerID) {
        customerToEdit = existingCustomer;
      }
    }
    if (adminService.editCustomer(customerToEdit, customer)) {
      // TODO: successful edit
    } else {
      // TODO: unsuccesful edit
    }
  } else {
    if (adminService.createCustomer(customer)) {
      // TODO: sucessful edit
    } else {
      // TODO: unsuccesful edit
    }
  }

  // Refresh our display to show up to date information.
  pAdminMainView->refreshCustomers(adminService.getAllCustomers());

  std::cout << "AdminController::executeCustomerAciton - exiting" << std::endl;
}

// We have cancelled the creation or edit of an employee
void AdminController::cancelCustomerAction() { pCustomerCreationView->end(); }

//*********************PRIVATE FUNCTIONS*****************************

// The custom display containers are formated with the customer full
// name in the top left, customer id below that, current worth on the right
// and univested funds below the current worth. All these values must be in
// QStrings.
std::vector<CustomerDisplayItem *>
AdminController::formatCustomersForDisplay(std::vector<Customer> customers) {
  // TODO: OPTIMIZE QUERIES WHEN IMPLEMENTING DATABASE
  std::vector<CustomerDisplayItem *> customerDisplayList;
  for (Customer customer : customers) {
    float currentWorth = 0.0f;

    for (Investment investment : customer.investments) {
      currentWorth += investment.currentInvestmentWorth;
    }

    std::string customerDisplayName =
        customer.lastName + ", " + customer.firstName.at(0);

    CustomerDisplayItem *displayItem = new CustomerDisplayItem(
        QString::fromStdString(customerDisplayName),
        QString::number(customer.customerID), QString::number(currentWorth),
        QString::number(customer.uninvestedFunds));

    customerDisplayList.push_back(displayItem);
  }

  return customerDisplayList;
}

// The custom display contianers for employees consist of their full name
// on the left with their account ID below it, and the number of customer
// accounts managed on the right. All in QStrings
std::vector<TraderDisplayItem *>
AdminController::formatEmployeesForDisplay(std::vector<Employee> employees) {
  // TODO: OPTIMIZE QUERIES WHEN IMPLEMENTING DATABASE
  std::vector<TraderDisplayItem *> employeeDisplayList;
  for (Employee employee : employees) {
    int numCustomersManaged = 0;

    for (Customer customer : adminService.getAllCustomers()) {
      if (customer.accountID == employee.accountID) {
        numCustomersManaged++;
      }
    }
    std::string numCustomersManagedDisplayText =
        std::to_string(numCustomersManaged) + " accounts managed.";
    std::string employeeDisplayName =
        employee.lastName + ", " + employee.firstName.at(0);

    TraderDisplayItem *displayItem = new TraderDisplayItem(
        QString::fromStdString(employeeDisplayName),
        QString::number(employee.accountID),
        QString::fromStdString(numCustomersManagedDisplayText));

    employeeDisplayList.push_back(displayItem);
  }

  return employeeDisplayList;
}

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

  pCustomerCreationView->end();

  executeCustomerAction(inputtedCustomer);

  std::cout << "AdminController::listenForCustomerActionConfirmation - exiting"
            << std::endl;
}

// Connected to the CreateCustomerView cancel button.
void AdminController::listenForCustomerActionCancel() {
  cancelCustomerAction();
}
