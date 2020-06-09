# define cmake minimum requirement
cmake_minimum_required(VERSION 3.0)

# project information
project(itkBinaryImageToConvexHullFilter)

# Global Libraries
# ##############################################################################

# ITK Libraries
# set(ITK_DIR /home/stephane/Libraries/ITK/release)
find_package(ITK REQUIRED)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
endif()

# Paths
# ##############################################################################
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

# Include directories
# ##############################################################################

include_directories(includes)

# Executable
# ##############################################################################

add_executable(itkBinaryImageToConvexHullFilterTest itkBinaryImageToConvexHullFilterTest.cpp)
add_executable(itkBinaryImageToConvexHullFilterTest2 itkBinaryImageToConvexHullFilterTest2.cpp)
add_executable(itkLabelImageToConvexHullFilterTest itkLabelImageToConvexHullFilterTest.cpp)


target_link_libraries(itkBinaryImageToConvexHullFilterTest ${ITK_LIBRARIES})
target_link_libraries(itkBinaryImageToConvexHullFilterTest2 ${ITK_LIBRARIES})
target_link_libraries(itkLabelImageToConvexHullFilterTest ${ITK_LIBRARIES})


set_target_properties(itkBinaryImageToConvexHullFilterTest
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
                      
set_target_properties(itkBinaryImageToConvexHullFilterTest2
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})


set_target_properties(itkLabelImageToConvexHullFilterTest
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Tests
# ##############################################################################

include(CTest)

add_test(NAME ConvexHullToBinaryImageTest1
         COMMAND ${BIN_DIR}/itkBinaryImageToConvexHullFilterTest ${DATA_DIR}/test1.tif 
        ${DATA_DIR}/result1.tif)

add_test(NAME ConvexHullToBinaryImageTest2
         COMMAND ${BIN_DIR}/itkBinaryImageToConvexHullFilterTest ${DATA_DIR}/test2.tif 
        ${DATA_DIR}/result2.tif 146)

add_test(NAME ConvexHullToBinaryImageTest3
         COMMAND ${BIN_DIR}/itkBinaryImageToConvexHullFilterTest2 ${DATA_DIR}/test2.tif 
        ${DATA_DIR}/result3.tif)

add_test(NAME ConvexHullToBinaryImageTest4
        COMMAND ${BIN_DIR}/itkBinaryImageToConvexHullFilterTest2 ${DATA_DIR}/test3.tif 
       ${DATA_DIR}/result4.tif)

add_test(NAME ConvexHullToLabelImageTest1
        COMMAND ${BIN_DIR}/itkLabelImageToConvexHullFilterTest ${DATA_DIR}/test3.tif 
       ${DATA_DIR}/result5.tif)

