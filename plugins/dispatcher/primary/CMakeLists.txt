add_library(dispatcher-primary SHARED)
target_sources(dispatcher-primary PRIVATE src/source.cpp)

find_package(OpenMP)

target_link_libraries(
  dispatcher-primary
  PRIVATE
    mt-plugin monte-toad-core imgui monte-toad-debug-util OpenMP::OpenMP_CXX
)

set_target_properties(
  dispatcher-primary
    PROPERTIES
      COMPILE_FLAGS
        "-Wshadow -Wdouble-promotion -Wall -Wformat=2 -Wextra -Wpedantic \
         -Wundef -fno-exceptions"
      SUFFIX ".mt-plugin"
      PREFIX ""
)

install(
  TARGETS dispatcher-primary
  LIBRARY NAMELINK_SKIP
  LIBRARY
    DESTINATION plugins/
    COMPONENT plugin
)
