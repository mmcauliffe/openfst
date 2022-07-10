

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
)
target_link_libraries(fstscriptutils PUBLIC fst)
target_include_directories(fstscriptutils PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>)

set_target_properties(fstscriptutils PROPERTIES
  SOVERSION "${SOVERSION}"
)
install(TARGETS fstscriptutils
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin)

add_library(fstscript
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
target_link_libraries(fstscript PUBLIC fst fstscriptutils)
target_include_directories(fstscriptutils PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
set_target_properties(fstscript PROPERTIES
  SOVERSION "${SOVERSION}"
)
install(TARGETS fstscript
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin)