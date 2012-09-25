cmake_minimum_required( VERSION 2.8 )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

project( eegtopo )

if( UNIX )
	add_definitions( -Wall -Wextra -pedantic -Werror -fPIC )
elseif( MINGW )
	add_definitions( -Wall -Wextra -pedantic -Werror )
elseif( WIN32 )
	add_definitions( -W3 )
endif( UNIX )

find_package( Boost REQUIRED COMPONENTS program_options )

find_package( Eigen3 REQUIRED )

find_package( PkgConfig REQUIRED )

pkg_search_module( CAIROMM REQUIRED cairomm-1.0 )

include_directories(
	../src
	${Boost_INCLUDE_DIR}
	${EIGEN3_INCLUDE_DIR}
	${CAIROMM_INCLUDE_DIRS}
)

set( HEADERS
	../src/colormap.h
	../src/electrodegraphics.h
	../src/electrodes.h
	../src/membermap.h
	../src/painter.h
	../src/parser.h
)

set( SOURCES
	../src/colormap.cpp
	../src/electrodegraphics.cpp
	../src/electrodes.cpp
	../src/main.cpp
	../src/membermap.cpp
	../src/painter.cpp
	../src/parser.cpp
)

add_executable( eegtopo ${HEADERS} ${SOURCES} )
target_link_libraries( eegtopo ${Boost_LIBRARIES} ${CAIROMM_LIBRARIES} )
