file(GLOB HEADER_FILES ../../include/fst/extensions/mpdt/*.h)

if(HAVE_SCRIPT)
  add_library(fstmpdtscript mpdtscript.cc ${HEADER_FILES})
  target_link_libraries(fstmpdtscript fstscript fstscriptutils fst)
  set_target_properties(fstmpdtscript PROPERTIES 
    SOVERSION "${SOVERSION}"
  )
  install(TARGETS fstmpdtscript 
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  )
endif(HAVE_SCRIPT)

if(HAVE_BIN)
  function (add_executable2 _name)
      add_executable(${ARGV})
      if (TARGET ${_name})
          target_link_libraries(${_name} fstmpdtscript fstpdtscript fstscriptutils fstscript fst ${CMAKE_DL_LIBS})
      endif()
    install(TARGETS ${_name} RUNTIME DESTINATION bin)
  endfunction()
  add_executable2(mpdtcompose  mpdtcompose.cc mpdtcompose-main.cc)
  add_executable2(mpdtexpand  mpdtexpand.cc mpdtexpand-main.cc)
  add_executable2(mpdtinfo  mpdtinfo.cc mpdtinfo-main.cc)
  add_executable2(mpdtreverse  mpdtreverse.cc mpdtreverse-main.cc)
endif(HAVE_BIN)