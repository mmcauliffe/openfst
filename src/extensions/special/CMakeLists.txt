file(GLOB HEADER_FILES ../../include/fst/extensions/special/*.h)


add_library(fstspecial
  phi-fst.cc
  rho-fst.cc
  sigma-fst.cc
  ${HEADER_FILES}
)
GENERATE_EXPORT_HEADER( fstspecial
             BASE_NAME fstspecial
             EXPORT_MACRO_NAME fstspecial_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/export/fstspecial_Export.h
             STATIC_DEFINE fstspecial_BUILT_AS_STATIC
)
target_compile_definitions(fstspecial PRIVATE fstspecial_EXPORTS)
target_include_directories(fstspecial PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)

set_target_properties(fstspecial PROPERTIES
  SOVERSION "${SOVERSION}"
)
target_link_libraries(fstspecial
  fst
)

if(HAVE_BIN)
  add_executable(fstspecial-bin
    ../../bin/fstconvert.cc 
    ../../bin/fstconvert-main.cc
  )

  set_target_properties(fstspecial-bin PROPERTIES 
    OUTPUT_NAME fstspecial
  )
  if (MSVC)
    target_link_libraries(fstspecial-bin
    fstspecial
      fstscript
      fstscriptutils
      fst
  )
 else()
  target_link_libraries(fstspecial-bin
  fstspecial
    fstscript
    fst
    ${CMAKE_DL_LIBS}
  )
 endif()
endif(HAVE_BIN)


install(TARGETS fstspecial fstspecial-bin
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
)
