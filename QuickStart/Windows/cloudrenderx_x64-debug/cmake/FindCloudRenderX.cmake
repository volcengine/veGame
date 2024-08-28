# 本文件可在源码编译及作为第三方库依赖的两种场景中复用
# 本文件主要提供以下2个变量:
# CLOUD_RENDER_X_ROOT   :  CloudRenderX项目根目录
# CloudRenderXLibraries :  所有用来link的cmake library

if (NOT DEFINED CLOUD_RENDER_X_ROOT)
	set(CLOUD_RENDER_X_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
endif()

# 预加载cloudrenderx相关库
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    file(GLOB BIN_FILES ${CLOUD_RENDER_X_ROOT}/bin/*.*)
    foreach (BIN_FILE ${BIN_FILES})
	    file(COPY ${BIN_FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        message(STATUS " [CloudRenderX_Debug] copy the prebuilt file[${BIN_FILE}] to ${CMAKE_CURRENT_BINARY_DIR}")
    endforeach()
endif()
 file(GLOB BIN_FILES ${CLOUD_RENDER_X_ROOT}/lib/*.*)
foreach (BIN_FILE ${BIN_FILES})
	file(COPY ${BIN_FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

# 增加头文件搜索路径
include_directories("${CLOUD_RENDER_X_ROOT}/include")
include_directories("${CLOUD_RENDER_X_ROOT}/include/cloudrenderx")

# 提供CloudRenderX Cmake target
if (NOT DEFINED CloudRenderXLibraries)
    set(CloudRenderXLibraries "")
    if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
       file(GLOB BIN_FILES ${CLOUD_RENDER_X_ROOT}/bin/*.dll)
       foreach (BIN_FILE ${BIN_FILES})
           STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${BIN_FILE})
           set(LIB_FILE ${CLOUD_RENDER_X_ROOT}/lib/${FILE_NAME}.lib)

           if(NOT EXISTS ${LIB_FILE})
               continue()
           endif()

           if (NOT TARGET ${FILE_NAME})
               add_library(${FILE_NAME} SHARED IMPORTED)
               list(APPEND CloudRenderXLibraries "${FILE_NAME}")           
               message(STATUS " [CloudRenderX_Debug] add the target for the cloudrenderx module ${FILE_NAME}")
           endif()

	       set_target_properties(${FILE_NAME} PROPERTIES IMPORTED_LOCATION ${BIN_FILE})
           message(STATUS " [CloudRenderX_Debug] add the location file[${BIN_FILE}] for the cloudrenderx target ${FILE_NAME}")

	       set_target_properties(${FILE_NAME} PROPERTIES IMPORTED_IMPLIB ${LIB_FILE})
           message(STATUS " [CloudRenderX_Debug] add the location file[${LIB_FILE}] for the cloudrenderx target ${FILE_NAME}")
       endforeach()
    endif()
    
     message(STATUS " [CloudRenderX_Debug] the libraries which is provided by CloudRenderX is ${CloudRenderXLibraries}")

endif ()