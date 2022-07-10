file(GLOB HEADER_FILES ../../include/fst/extensions/pdt/*.h)

if(HAVE_SCRIPT)
  add_library(fstpdtscript getters.cc pdtscript.cc ${HEADER_FILES})
  target_link_libraries(fstpdtscript fstscript fstscriptutils fst)
  set_target_properties(fstpdtscript PROPERTIES 
    SOVERSION "${SOVERSION}"
  )

  install(TARGETS fstpdtscript
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  )
endif(HAVE_SCRIPT)

if(HAVE_BIN)
  function (add_executable2 _name)
      add_executable(${ARGV})
      if (TARGET ${_name})
          target_link_libraries(${_name} fstpdtscript fstscriptutils fstscript fst ${CMAKE_DL_LIBS})
      endif()
      install(TARGETS ${_name} RUNTIME DESTINATION bin)
  endfunction()

  add_executable2(pdtcompose  pdtcompose.cc pdtcompose-main.cc)
  add_executable2(pdtexpand  pdtexpand.cc pdtexpand-main.cc)
  add_executable2(pdtinfo  pdtinfo.cc pdtinfo-main.cc)
  add_executable2(pdtreplace  pdtreplace.cc pdtreplace-main.cc)
  add_executable2(pdtreverse  pdtreverse.cc pdtreverse-main.cc)
  add_executable2(pdtshortestpath  pdtshortestpath.cc pdtshortestpath-main.cc)
endif(HAVE_BIN)