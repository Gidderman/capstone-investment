#include "AdminService.h"

AdminService::AdminService() {}

AdminService::~AdminService() {}

std::vector<Employee> AdminService::getAllEmployees() {
  return dataManager.getAllEmployees();
}

std::vector<Customer> AdminService::getAllCustomers() {
  return dataManager.getAllCustomers();
}
