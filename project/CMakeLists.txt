cmake_minimum_required( VERSION 2.8 )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

project( eegtopo )

if( UNIX )
	if( APPLE )
		message( STATUS "Apple OS detected." )
		add_definitions( -Wall -Wextra -pedantic )
	else( APPLE )
		message( STATUS "Unix-like OS detected." )
		add_definitions( -Wall -Wextra -pedantic -Werror )
	endif( APPLE )
elseif( WIN32 )
	if( MINGW )
		message( STATUS "MinGW detected." )
		add_definitions( -Wall -Wextra -pedantic -Werror ) 
	else( MINGW )
		message( STATUS "Windows OS detected." )
		add_definitions( -W3 )
	endif( MINGW )
endif( UNIX )

find_package( Boost REQUIRED COMPONENTS program_options )

find_package( Eigen3 REQUIRED )

find_package( PkgConfig REQUIRED )

pkg_search_module( CAIROMM REQUIRED cairomm-1.0 )

pkg_search_module( GTKMM REQUIRED gtkmm-3.0 )

if( ${GTKMM_FOUND} )
	message( STATUS "GTKMM found. Compiling GUI." )
	message( STATUS ${GTKMM_INCLUDE_DIRS} )
	add_definitions(-DGUI_ENABLED )

	set( GUI_HEADERS
		../src/gui/elselwidget.h
		../src/gui/guimain.h
		../src/gui/mainwindow.h
		../src/gui/previewwidget.h
	)
	
	set( GUI_SOURCES
		../src/gui/elselwidget.cpp
		../src/gui/guimain.cpp
		../src/gui/mainwindow.cpp
		../src/gui/previewwidget.cpp
	)

endif( ${GTKMM_FOUND} )

include_directories(
	../src
	${Boost_INCLUDE_DIR}
	${EIGEN3_INCLUDE_DIR}
	${CAIROMM_INCLUDE_DIRS}
	${GTKMM_INCLUDE_DIRS}
)

set( HEADERS
	../src/colormap.h
	../src/electrodegraphics.h
	../src/electrodes.h
	../src/membermap.h
	../src/painter.h
	../src/parser.h
	${GUI_HEADERS}
)

set( SOURCES
	../src/colormap.cpp
	../src/electrodegraphics.cpp
	../src/electrodes.cpp
	../src/main.cpp
	../src/membermap.cpp
	../src/painter.cpp
	../src/parser.cpp
	${GUI_SOURCES}
)

add_executable( eegtopo ${HEADERS} ${SOURCES} )
target_link_libraries( eegtopo ${Boost_LIBRARIES} ${CAIROMM_LIBRARIES} ${GTKMM_LIBRARIES} )
