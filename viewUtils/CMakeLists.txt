add_library(ViewUtils)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

target_sources(ViewUtils
  PRIVATE
    src/ScrollableContainer.cpp
    src/TraderDisplayItem.cpp
    src/CustomerDisplayItem.cpp
    src/InvestmentDisplayItem.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      ScrollableContainer.h
      TraderDisplayItem.h
      CustomerDisplayItem.h
      InvestmentDisplayItem.h
)

target_link_libraries(ViewUtils
  PUBLIC
    Qt6::Widgets
    Utils
)
