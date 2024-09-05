# Determine OS
if (WIN32)
    include(${CMAKE_CURRENT_LIST_DIR}/deploy-win.cmake)
# elseif (APPLE)
#     message(FATAL_ERROR "Not implemented!")
elseif (UNIX AND NOT APPLE)
    include(${CMAKE_CURRENT_LIST_DIR}/deploy-linux.cmake)
endif()
