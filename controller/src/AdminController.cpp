#include "AdminController.h"
#include "AdminMainView.h"
#include "Customer.h"
#include "CustomerDisplayItem.h"

#include <string>

AdminController::AdminController() {
  customerList = getAllCustomers();
  employeeList = getAllEmployees();

  formatCustomersForDisplay();
  formatEmployeesForDisplay();

  pAdminMainView =
      new AdminMainView(&customerDisplayList, &employeeDisplayList);
};

AdminController::~AdminController() {}

void AdminController::run(Authorizer *authorizer) {
  if (authorizer->authorizeUser(ADMIN)) {
    // TODO: Throw and exception
  }

  pAdminMainView->show();
}

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
