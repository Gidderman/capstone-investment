add_library(Utils)

set_target_properties(Utils
  PROPERTIES 
    LINKER_LANGUAGE CXX 
)

target_sources(Utils
  PUBLIC
    FILE_SET HEADERS
    FILES 
      Customer.h 
      Investment.h 
      Role.h 
      Stock.h 
      Trader.h 
)
