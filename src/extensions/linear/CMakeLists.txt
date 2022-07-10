file(GLOB HEADER_FILES ../../include/fst/extensions/linear/*.h)


if(HAVE_SCRIPT)
  add_library(fstlinearscript
    linearscript.cc
    ${HEADER_FILES}
  )
  target_link_libraries(fstlinearscript
    fstscript
    fstscriptutils
    fst
  )
  set_target_properties(fstlinearscript PROPERTIES 
    SOVERSION "${SOVERSION}"
  )
  
  install(TARGETS fstlinearscript
  LIBRARY DESTINATION bin
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  )
endif(HAVE_SCRIPT)

if(HAVE_BIN)
  add_executable(fstlinear
    fstlinear.cc 
    fstlinear-main.cc)
  target_link_libraries(fstlinear
    fstlinearscript
    fstscriptutils
    fstscript 
    fst
    ${CMAKE_DL_LIBS}
 )

  add_executable(fstloglinearapply
    fstloglinearapply.cc 
    fstloglinearapply-main.cc)
  target_link_libraries(fstloglinearapply
    fstlinearscript
    fstscriptutils
    fstscript 
    fst
    ${CMAKE_DL_LIBS}
  )
  install(TARGETS fstlinear fstloglinearapply
    RUNTIME DESTINATION bin
  )
endif(HAVE_BIN)


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

add_module(linear-tagger-fst MODULE
  linear-tagger-fst.cc)

add_module(linear-classifier-fst MODULE
  linear-classifier-fst.cc)
