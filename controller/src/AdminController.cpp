#include "AdminController.h"
#include "AdminMainView.h"
#include "Customer.h"
#include "CustomerDisplayItem.h"
#include "TraderCreationView.h"

#include <iostream>
#include <string>

AdminController::AdminController() {
  customerList = getAllCustomers();
  employeeList = getAllEmployees();

  formatCustomersForDisplay();
  formatEmployeesForDisplay();

  pAdminMainView =
      new AdminMainView(&customerDisplayList, &employeeDisplayList);
  pTraderCreationView = new TraderCreationView();

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
};

AdminController::~AdminController() {}

void AdminController::run(Authorizer *authorizer) {
  if (authorizer->authorizeUser(ADMIN)) {
    // TODO: Throw and exception
  }

  pAdminMainView->show();
}

void AdminController::executeEmployeeCreation() { pTraderCreationView->run(); }

void AdminController::executeEmployeeEdit() { pTraderCreationView->run(); }

void AdminController::executeEmployeeDeletion() {
  std::cout << "DELETE EMPLOYEE BUTTON CLICKED" << std::endl;
}

void AdminController::executeCustomerCreation() {
  std::cout << "CREATE CUSTOMER BUTTON CLICKED" << std::endl;
}

void AdminController::executeCustomerEdit() {
  std::cout << "EDIT CUSTOMER BUTTON CLICKED" << std::endl;
}

void AdminController::executeCustomerDeletion() {
  std::cout << "DELETE CUSTOMER BUTTON CLICKED" << std::endl;
}

//*********************PRIVATE FUNCTIONS*****************************
std::vector<Customer> AdminController::getAllCustomers() {
  // TODO: develope actual functionality
  Stock stock = {0001, "Nintendo", "NTD", 340.00f};
  Investment investment = {0001, stock, 4, 1500.00f, 0002};
  std::vector<Investment> investments = {investment};
  Customer tempCustomer = {
      0002,        "Harry",    "Johans", 888888888,   "hjohans@email.com",
      "20Feb2020", RETIREMENT, 1500.00f, investments, 0001};
  std::vector<Customer> customers = {tempCustomer};

  return customers;
}

std::vector<Employee> AdminController::getAllEmployees() {
  // TODO: develope actual functionality

  Employee tempEmployee = {0001, "John", "Doe", ADMIN};

  std::vector<Employee> employees = {tempEmployee};

  return employees;
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

//*********************SLOTS**************************************
void AdminController::listenForLogOut() {
  pAdminMainView->hide();
  emit informMasterControllerOfLogOut();
}
void AdminController::listenForEmployeeCreation() { executeEmployeeCreation(); }
void AdminController::listenForEmployeeEdit() { executeEmployeeEdit(); }
void AdminController::listenForEmployeeDeletion() { executeEmployeeDeletion(); }
void AdminController::listenForCustomerCreation() { executeCustomerCreation(); }
void AdminController::listenForCustomerEdit() { executeCustomerEdit(); }
void AdminController::listenForCustomerDeletion() { executeCustomerDeletion(); }
