#include "AdminController.h"
#include "AdminMainView.h"
#include "Authorizer.h"
#include "Customer.h"
#include "CustomerDisplayItem.h"
#include "TraderCreationView.h"
#include "WarningWindow.h"

#include <iostream>
#include <string>

AdminController::AdminController() {};

AdminController::~AdminController() {}

void AdminController::run(Authorizer *authorizer) {
  if (!authorizer->authorizeUser(ADMIN)) {
    // TODO: Throw and exception
  }

  customerList = getAllCustomers();
  employeeList = getAllEmployees();

  formatCustomersForDisplay();
  formatEmployeesForDisplay();

  pAdminMainView =
      new AdminMainView(&customerDisplayList, &employeeDisplayList);
  pTraderCreationView = new TraderCreationView();
  pCustomerCreationView = new CustomerCreationView();

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
          this, &AdminController::listenForEmployeeCreation);
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

  pAdminMainView->show();
}

void AdminController::executeEmployeeCreation() { pTraderCreationView->run(); }

void AdminController::executeEmployeeEdit(Employee employeeToBeEdited) {
  pTraderCreationView->run(
      formatIndividualEmployeeForDisplay(employeeToBeEdited));
}

void AdminController::executeEmployeeDeletion() {
  pDeleteWarning = new WarningWindow();

  int warningWindowChoice = pDeleteWarning->exec();
  switch (warningWindowChoice) {
  case QMessageBox::Cancel:
    // User cancelled...
    std::cout << warningWindowChoice << " DID NOT DELETE" << std::endl;
    break;
  case QMessageBox::Apply:
    // User confirmed deletion.
    std::cout << warningWindowChoice << " DELETED" << std::endl;
    break;
  default:
    std::cout << warningWindowChoice << " HIT THE DEFAULT" << std::endl;
    break;
  }

  delete pDeleteWarning;
  pDeleteWarning = nullptr;
}

void AdminController::executeCustomerCreation() {
  pCustomerCreationView->run();
}

void AdminController::executeCustomerEdit() { pCustomerCreationView->run(); }

void AdminController::executeCustomerDeletion() {
  pDeleteWarning = new WarningWindow();

  int warningWindowChoice = pDeleteWarning->exec();
  switch (warningWindowChoice) {
  case QMessageBox::Cancel:
    // User cancelled...
    std::cout << warningWindowChoice << " DID NOT DELETE" << std::endl;
    break;
  case QMessageBox::Apply:
    // User confirmed deletion.
    std::cout << warningWindowChoice << " DELETED" << std::endl;
    break;
  default:
    std::cout << warningWindowChoice << " HIT THE DEFAULT" << std::endl;
    break;
  }

  delete pDeleteWarning;
  pDeleteWarning = nullptr;
}

void AdminController::executeEmployeeAction(Employee employee) {
  employeeList = getAllEmployees();
  for (Employee existingEmployee : employeeList) {
    if (employee.firstName == existingEmployee.firstName &&
        employee.lastName == existingEmployee.lastName &&
        employee.role == existingEmployee.role) {
      // TODO: PERFORM AN UPDATE EVOLUTION FOR EXISTING EMPLOYEE
      return;
    }
  }

  // TODO: PERFORM A CREATION EVOLUTION FOR EXITING EMPLOYEE
}

void AdminController::executeEmployeeAccountUnlock() {} // TODO: this

void AdminController::cancelEmployeeAction() { pTraderCreationView->end(); }

void AdminController::executeCustomerAction() {} // TODO: this

void AdminController::cancelCustomerAction() { pCustomerCreationView->end(); }

//*********************PRIVATE FUNCTIONS*****************************
std::vector<Customer> AdminController::getAllCustomers() {
  return adminService.getAllCustomers();
}

std::vector<Employee> AdminController::getAllEmployees() {
  return adminService.getAllEmployees();
}

void AdminController::formatCustomersForDisplay() {
  customerDisplayList.clear();

  for (Customer customer : customerList) {
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
}

void AdminController::formatEmployeesForDisplay() {
  employeeDisplayList.clear();

  for (Employee employee : employeeList) {
    int numCustomersManaged = 0;

    for (Customer customer : customerList) {
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
}

std::vector<QString>
AdminController::formatIndividualEmployeeForDisplay(Employee employee) {

  std::vector<QString> returnData;
  returnData.push_back(QString::fromStdString(employee.firstName));
  returnData.push_back(QString::fromStdString(employee.lastName));
  std::cout << "Roll prior to writing to vector is " << employee.role
            << " ADMIN is " << ADMIN << std::endl;
  if (employee.role == ADMIN) {
    returnData.push_back(QString("Admin"));
  } else {
    returnData.push_back(QString("Trader"));
  }
  returnData.push_back(QString::number(employee.accountID));

  std::cout << "Current role is " << returnData.at(2).toStdString()
            << std::endl;

  return returnData;
}

//*********************SLOTS**************************************
void AdminController::listenForLogOut() {
  pAdminMainView->hide();
  emit informMasterControllerOfLogOut();
}

void AdminController::listenForEmployeeCreation() { executeEmployeeCreation(); }

void AdminController::listenForEmployeeEdit(int id) {
  Employee employeeToEdit;
  for (Employee employee : employeeList) {
    if (employee.accountID == id) {
      employeeToEdit = employee;
    }
  }
  executeEmployeeEdit(employeeToEdit);
}

void AdminController::listenForEmployeeDeletion() { executeEmployeeDeletion(); }

void AdminController::listenForCustomerCreation() { executeCustomerCreation(); }

void AdminController::listenForCustomerEdit() { executeCustomerEdit(); }

void AdminController::listenForCustomerDeletion() { executeCustomerDeletion(); }

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
  executeEmployeeAction(inputtedEmployee);
}

void AdminController::listenForEmployeeActionCancel() {
  cancelEmployeeAction();
}

void AdminController::listenForEmployeeAccountUnlock() {
  executeEmployeeAccountUnlock();
}

void AdminController::listenForCustomerActionConfirmation() {
  executeCustomerAction();
}

void AdminController::listenForCustomerActionCancel() {
  cancelCustomerAction();
}
