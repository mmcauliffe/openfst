file(GLOB HEADER_FILES ../../include/fst/extensions/special/*.h)

if(HAVE_BIN)
  add_executable(fstspecial-bin
    ../../bin/fstconvert.cc 
    ../../bin/fstconvert-main.cc
    phi-fst.cc
    rho-fst.cc
    sigma-fst.cc
  )

  set_target_properties(fstspecial-bin PROPERTIES 
    OUTPUT_NAME fstspecial
  )

  target_link_libraries(fstspecial-bin
    fstscript
    fstscriptutils
    fst
    ${CMAKE_DL_LIBS}
 )
endif(HAVE_BIN)


add_library(fstspecial
  phi-fst.cc
  rho-fst.cc
  sigma-fst.cc
  ${HEADER_FILES}
)

set_target_properties(fstspecial PROPERTIES
  SOVERSION "${SOVERSION}"
)
target_link_libraries(fstspecial
  fst
)

install(TARGETS fstspecial fstspecial-bin
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
)

function (add_module _name)
    add_library(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} fst)
    endif()

    install(TARGETS ${_name}
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION bin
    RUNTIME DESTINATION bin)
endfunction()

add_module(phi-fst MODULE phi-fst.cc)
add_module(rho-fst MODULE rho-fst.cc)
add_module(sigma-fst MODULE sigma-fst.cc)