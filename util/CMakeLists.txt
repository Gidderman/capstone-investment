add_library(Utils)

set_target_properties(Utils
  PROPERTIES 
    LINKER_LANGUAGE CXX 
)

target_sources(Utils
  PRIVATE
    src/HashTable.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES 
      Customer.h 
      Investment.h 
      Role.h 
      Stock.h 
      Employee.h 
      Credentials.h
      QuickSort.h
      QuickSort.tpp
      HashTable.h
)

