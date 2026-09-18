#include "CRUDManager.h"

CRUDManager::CRUDManager() {
  // TODO: DELETE FOLLOWING TESTING VECTOR
  testingVector.push_back("00001");
  testingVector.push_back("John");
  testingVector.push_back("Doe");
  testingVector.push_back("jdoe");
  testingVector.push_back("pass123");
  testingVector.push_back("321");
  testingVector.push_back("ADMIN");
}

CRUDManager::~CRUDManager() {}

std::vector<std::string> CRUDManager::runQuery(std::string username) {
  // TODO: Actual logic
  if (username == "jdoe") {
    return testingVector;
  } else if (username == "hdoe") {
    testingVector.at(3) = "hdoe";
    testingVector.at(6) = "TRADER";
  }

  return testingVector;
}
