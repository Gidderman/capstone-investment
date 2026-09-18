#ifndef CRUD_MANAGER_H
#define CRUD_MANAGER_H

#include <string> //TODO: REMOVE AFTER TESTING
#include <vector> //TODO: REMOVE AFTER TESTING

class CRUDManager {
private:
  std::vector<std::string> testingVector; // TODO: REMOVE AFTER TESTING

public:
  CRUDManager();
  ~CRUDManager();
  std::vector<std::string> runQuery(std::string username);
};

#endif
