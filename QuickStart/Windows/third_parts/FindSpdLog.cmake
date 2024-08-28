# spdlog 库
if (NOT TARGET SpdLog)
        add_library(SpdLog SHARED IMPORTED)
        
        # 处理lib文件
        if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/SpdLog/windows/${CLOUDRENDERX_TARGET_ARCHITECTURE}/libs/spdlog.lib)
            set_target_properties(SpdLog PROPERTIES IMPORTED_IMPLIB ${CMAKE_CURRENT_LIST_DIR}/SpdLog/windows/${CLOUDRENDERX_TARGET_ARCHITECTURE}/libs/spdlog.lib)
	        message(STATUS "the lib file is ${CMAKE_CURRENT_LIST_DIR}/SpdLog/windows/${CLOUDRENDERX_TARGET_ARCHITECTURE}/libs/spdlog.lib for the 3rd parts SpdLog")
            file(COPY ${CMAKE_CURRENT_LIST_DIR}/SpdLog/windows/${CLOUDRENDERX_TARGET_ARCHITECTURE}/libs/spdlog.lib DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        else()
            message(FATAL_ERROR "the lib file ${CMAKE_CURRENT_LIST_DIR}/SpdLog/windows/${CLOUDRENDERX_TARGET_ARCHITECTURE}/libs/spdlog.lib for the 3rd parts SpdLog is not exist...")
        endif()

        include_directories(${CMAKE_CURRENT_LIST_DIR}/SpdLog/windows/${CLOUDRENDERX_TARGET_ARCHITECTURE}/includes)

endif ()