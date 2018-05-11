cmake_minimum_required( VERSION 3.3.0 FATAL_ERROR )

set( RANGE_FN_INCLUDE_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}/../" )

add_executable(
   range_fn_tests
   "${CMAKE_CURRENT_LIST_DIR}/tests.cpp"
   "${CMAKE_CURRENT_LIST_DIR}/catch_main.cpp"
)
target_include_directories( range_fn_tests PRIVATE ${RANGE_FN_INCLUDE_DIR_PATH} )


set_target_properties(
   range_fn_tests PROPERTIES
   CXX_EXTENSIONS FALSE
)

target_compile_features(
   range_fn_tests PUBLIC
   cxx_trailing_return_types
   cxx_default_function_template_args
)

add_executable(
   dev_main
   "${CMAKE_CURRENT_LIST_DIR}/dev_main.cpp"
)
target_include_directories( dev_main PRIVATE ${RANGE_FN_INCLUDE_DIR_PATH} )


set_target_properties(
   dev_main PROPERTIES
   CXX_EXTENSIONS FALSE
)

target_compile_features(
   dev_main PUBLIC
   cxx_trailing_return_types
   cxx_default_function_template_args
)

IF (WIN32)
   target_compile_options(
      dev_main PUBLIC
      "/W4"
   )
   target_compile_options(
      range_fn_tests PUBLIC
      "/W4"
   )
ELSE()
   target_compile_options(
      dev_main PUBLIC
      "-Wall"
   )
   target_compile_options(
      range_fn_tests PUBLIC
      "-Wall"
   )
ENDIF()



