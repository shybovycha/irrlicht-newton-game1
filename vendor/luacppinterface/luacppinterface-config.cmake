add_library(luacppinterface STATIC IMPORTED)
find_library(LUACPPINTERFACE_LIBRARY_PATH luacppinterface HINTS "${CMAKE_CURRENT_LIST_DIR}")
set_target_properties(luacppinterface PROPERTIES IMPORTED_LOCATION "${LUACPPINTERFACE_LIBRARY_PATH}")