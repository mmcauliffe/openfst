file(GLOB HEADER_FILES ../../include/fst/extensions/ngram/*.h)

add_library(fstngram
    bitmap-index.cc 
    ngram-fst.cc 
    nthbit.cc
    ${HEADER_FILES}
)

target_link_libraries(fstngram
    fst
)

set_target_properties(fstngram PROPERTIES
  SOVERSION "${SOVERSION}"
)

install(TARGETS fstngram 
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
)

add_library(ngram_fst MODULE
    bitmap-index.cc 
    ngram-fst.cc 
    nthbit.cc
)

target_link_libraries(ngram_fst
    fst
)

install(TARGETS ngram_fst
LIBRARY DESTINATION bin
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
)