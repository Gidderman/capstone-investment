add_library(Models)

target_sources(Models
  PRIVATE
    src/CRUDManager.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      CRUDManager.h
)

find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Core)

target_link_libraries(Models
  PUBLIC
    Qt6::Sql
    Qt6::Core
)
