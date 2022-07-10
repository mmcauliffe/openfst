file(GLOB HEADER_FILES ../../include/fst/extensions/compress/*.h)

add_library(fstcompressscript
  compressscript.cc
  ${HEADER_FILES}
 )

target_link_libraries(fstcompressscript
  fstscriptutils
  fstscript
  fst
  ${ZLIBS}
)
set_target_properties(fstcompressscript PROPERTIES
  SOVERSION "${SOVERSION}"
)
install(TARGETS fstcompressscript
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
 )

if(HAVE_BIN)
  add_executable(fstcompress
    fstcompress.cc
    fstcompress-main.cc)

  target_link_libraries(fstcompress
    fstcompressscript
    fstscriptutils
    fstscript
    fst
    ${ZLIBS}
    ${CMAKE_DL_LIBS}
   )

  install(TARGETS fstcompress
	        LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
		)
endif(HAVE_BIN)