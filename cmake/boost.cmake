set(BOOST_DIR lib/boost)
include_directories(${BOOST_DIR})
set(BOOST_SRC
  ${BOOST_DIR}/filesystem/src/codecvt_error_category.cpp
  ${BOOST_DIR}/filesystem/src/operations.cpp
  ${BOOST_DIR}/filesystem/src/path.cpp
  ${BOOST_DIR}/filesystem/src/path_traits.cpp
  ${BOOST_DIR}/filesystem/src/portability.cpp
  ${BOOST_DIR}/filesystem/src/unique_path.cpp
  ${BOOST_DIR}/filesystem/src/utf8_codecvt_facet.cpp
  ${BOOST_DIR}/system/src/error_code.cpp
)

include_directories(
  ${BOOST_DIR}/config/include
  ${BOOST_DIR}/core/include
  ${BOOST_DIR}/detail/include
  ${BOOST_DIR}/exception/include
  ${BOOST_DIR}/filesystem/include
  ${BOOST_DIR}/functional/include
  ${BOOST_DIR}/include
  ${BOOST_DIR}/io/include
  ${BOOST_DIR}/iterator/include
  ${BOOST_DIR}/mpl/include
  ${BOOST_DIR}/predef/include
  ${BOOST_DIR}/preprocessor/include
  ${BOOST_DIR}/range/include
  ${BOOST_DIR}/smart_ptr/include
  ${BOOST_DIR}/system/include
  ${BOOST_DIR}/type_traits/include
  ${BOOST_DIR}/utility/include
)

add_library(${PROJECT_NAME}_boost STATIC ${BOOST_SRC})
