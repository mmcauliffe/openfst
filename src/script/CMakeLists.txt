

if (MSVC) # Fix for too many exported symbols in fstscript
  add_library(fstscriptutils
    SHARED
    arciterator-class.cc
    stateiterator-class.cc
    weight-class.cc
    draw.cc
    getters.cc
    print.cc
    text-io.cc
    arcsort.cc
    closure.cc
    fst-class.cc
    info.cc
    info-impl.cc
    verify.cc
    map.cc
    compile.cc
    compose.cc
    concat.cc
    connect.cc
    convert.cc
    randequivalent.cc
    randgen.cc
    topsort.cc
    union.cc
    equal.cc
    equivalent.cc
    intersect.cc
    invert.cc
    encode.cc
    decode.cc
    encodemapper-class.cc
      determinize.cc
      difference.cc
      disambiguate.cc
  )
GENERATE_EXPORT_HEADER( fstscriptutils
             BASE_NAME fstscriptutils
             EXPORT_MACRO_NAME fstscriptutils_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/export/fstscriptutils_Export.h
             STATIC_DEFINE fstscriptutils_BUILT_AS_STATIC
)
  target_link_libraries(fstscriptutils PRIVATE fst)
  target_include_directories(fstscriptutils PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:include>)
target_include_directories(fstscriptutils PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_compile_definitions(fstscriptutils PRIVATE fstscriptutils_EXPORT)
  set_target_properties(fstscriptutils PROPERTIES
    SOVERSION "${SOVERSION}"
  )
  install(TARGETS fstscriptutils
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)
    add_library(fstscript
      epsnormalize.cc
      isomorphic.cc
      minimize.cc
      project.cc
      prune.cc
      push.cc
      relabel.cc
      replace.cc
      reverse.cc
      reweight.cc
      rmepsilon.cc
      shortest-distance.cc
      shortest-path.cc
      synchronize.cc
    )
GENERATE_EXPORT_HEADER( fstscript
             BASE_NAME fstscript
             EXPORT_MACRO_NAME fstscript_EXPORT
             EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/fst/export/fstscript_Export.h
             STATIC_DEFINE fstscript_BUILT_AS_STATIC
)
target_include_directories(fstscript PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_compile_definitions(fstscript PRIVATE fstscript_EXPORTS)
    target_link_libraries(fstscript PRIVATE fst fstscriptutils)
    else()
    add_library(fstscript
      arciterator-class.cc
      stateiterator-class.cc
      weight-class.cc
      draw.cc
      getters.cc
      print.cc
      text-io.cc
      arcsort.cc
      closure.cc
      fst-class.cc
      info.cc
      info-impl.cc
      verify.cc
      map.cc
      compile.cc
      compose.cc
      concat.cc
      connect.cc
      convert.cc
      randequivalent.cc
      randgen.cc
      topsort.cc
      union.cc
      equal.cc
      equivalent.cc
      intersect.cc
      invert.cc
      encode.cc
      decode.cc
      encodemapper-class.cc
      determinize.cc
      difference.cc
      disambiguate.cc
      epsnormalize.cc
      isomorphic.cc
      minimize.cc
      project.cc
      prune.cc
      push.cc
      relabel.cc
      replace.cc
      reverse.cc
      reweight.cc
      rmepsilon.cc
      shortest-distance.cc
      shortest-path.cc
      synchronize.cc
    )

    target_link_libraries(fstscript PUBLIC fst)
endif()
target_include_directories(fstscript PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
set_target_properties(fstscript PROPERTIES
  SOVERSION "${SOVERSION}"
)
install(TARGETS fstscript
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin)