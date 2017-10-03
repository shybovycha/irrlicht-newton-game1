add_library(enet STATIC IMPORTED)
find_library(ENet_LIBRARY_PATH enet HINTS "${CMAKE_CURRENT_LIST_DIR}/")
set_target_properties(enet PROPERTIES IMPORTED_LOCATION "${ENET_LIBRARY_PATH}")