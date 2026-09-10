add_library(Services)

target_sources(Services
  PRIVATE
    src/AdminService.cpp 
    src/Authorizer.cpp
    src/DataManager.cpp
    src/InitService.cpp
    src/LogInService.cpp
    src/TraderService.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      AdminService.h 
      Authorizer.h 
      DataManager.h
      InitService.h
      LogInService.h
      TraderService.h
)
