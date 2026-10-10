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

#Necessary for password hashing via libsodium
find_package(PkgConfig REQUIRED)
pkg_check_modules(sodium REQUIRED IMPORTED_TARGET libsodium)

find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Core)

target_link_libraries(Services
  PUBLIC
    Utils
    Qt6::Network

  PRIVATE
    Models
    PkgConfig::sodium
    Qt6::Core
)
