#include "AdminController.h"
#include "AdminMainView.h"
#include "CustomerDisplayItem.h"

#include <string>

AdminController::AdminController(Authorizer *authorizer) {
  if (authorizer->authorizeUser(ADMIN)) {
    // TODO: Throw and exception
  }

  customerList = getAllCustomers();
  employeeList = getAllEmployees();

  formatCustomersForDisplay();
  formatEmployeesForDisplay();

  pAdminMainView =
      new AdminMainView(&customerDisplayList, &employeeDisplayList);
};

AdminController::~AdminController() {}

std::vector<Customer> AdminController::getAllCustomers() {}

std::vector<Employee> AdminController::getAllEmployees() {}

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
