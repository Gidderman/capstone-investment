// The CRUDManager is the point of contact with the database. It handles all
// database interfacing and queries, utilizing Qt::SQL to perform those actions.

// TODO:
// THIS CLASS IS NOT COMPLETE. CURRENTLY IT IS SIMULATING A DATABASE USING
// UNORDERED_MAPS FOR TESTING THE REMAINDER OF THE APPLICATION, THERE WILL BE
// MANY CHANGES TO THIS CLASS

#ifndef CRUD_MANAGER_H
#define CRUD_MANAGER_H

#include <optional>
#include <string>        //TODO: REMOVE AFTER TESTING
#include <unordered_map> //TODO: REMOVE AFTER TESTING
#include <vector>        //TODO: REMOVE AFTER TESTING

// This class is largely going to be incomplete until I begin the working on the
// database connectivity, which will happen in a later module. For now, I am
// using some unordered_maps to simulate the database for testing the rest of
// the application.

class CRUDManager {
private:
  int nextEmployeeId;
  int nextCustomerId;
  int nextInvestmentId;
  int nextStockId;

  std::unordered_map<int, std::vector<std::string>> testingEmployeeDatabase;
  std::unordered_map<int, std::vector<std::string>> testingCredentialDatabase;
  std::unordered_map<int, std::string> testingRolesDatabase;
  std::unordered_map<int, std::vector<std::string>> testingCustomerDatabase;
  std::unordered_map<int, std::vector<std::string>> testingInvestmentDatabase;
  std::unordered_map<int, std::vector<std::string>> testingStockDatabase;

  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
  createSelection(
      int id, bool isCustomer,
      std::unordered_map<std::string, std::vector<std::string>> userToCreate);

  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
  readSelection(int id, bool isCustomer);

  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
  updateSelection(
      int id, bool isCustomer,
      std::unordered_map<std::string, std::vector<std::string>> update);
  bool deleteSelection(int id, bool isCustomer);

public:
  CRUDManager();
  ~CRUDManager();
  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
  runLogInQuery(std::string username);
  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
  runQuery(int id, bool isCustomer, std::string queryType);
  std::optional<std::unordered_map<std::string, std::vector<std::string>>>
  runQuery(int id, bool isCustomer, std::string queryType,
           std::unordered_map<std::string, std::vector<std::string>> data);
  std::vector<std::string> getCredentialsByEmployeeId(int employeeId);
  std::unordered_map<int, std::vector<std::string>> getAllEmployees();
  std::tuple<std::unordered_map<int, std::vector<std::string>>,
             std::unordered_map<int, std::vector<std::string>>>
  getAllCustomers();

  std::unordered_map<int, std::vector<std::string>> getAllStocks();
};

#endif
