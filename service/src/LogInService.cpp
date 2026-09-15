#include "LogInService.h"

LogInService::LogInService() {}

LogInService::~LogInService() {}

ROLE handleLogInAttempt(std::string enteredUsername,
                        std::string enteredPassword) {
  // TODO: LOGIC
}

std::string LogInService::hashPassword(std::string rawPassword) {
  // TODO: LOGIC
}

bool authenticate(std::string enteredPassword, std::string validPassword) {
  // TODO: LOGIC
}
