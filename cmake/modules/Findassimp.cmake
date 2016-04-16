# From https://github.com/assimp/assimp/blob/master/cmake-modules/Findassimp.cmake

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ASSIMP_ARCHITECTURE "64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(ASSIMP_ARCHITECTURE "32")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
	
if(WIN32)
	#set(ASSIMP_ROOT_DIR CACHE PATH "ASSIMP root directory")

	if (ASSIMP_INCLUDE_DIR)
	  # Already in cache, be silent
	  SET(ASSIMP_FIND_QUIETLY TRUE)
	ENDIF (ASSIMP_INCLUDE_DIR)

	message(${ASSIMP_ROOT_DIR}/include)

	# Find path of each library
	find_path(ASSIMP_INCLUDE_DIR
		NAMES
			assimp/anim.h
		HINTS
			${ASSIMP_ROOT_DIR}/include
	)

	if(MSVC12)
		set(ASSIMP_MSVC_VERSION "vc120")
	elseif(MSVC14)	
		set(ASSIMP_MSVC_VERSION "vc140")
	endif(MSVC12)
	
	if(MSVC12 OR MSVC14)

		find_path(ASSIMP_LIBRARY_DIR
			NAMES
				assimp-${ASSIMP_MSVC_VERSION}-mt.lib
			HINTS
				${ASSIMP_ROOT_DIR}/lib${ASSIMP_ARCHITECTURE}
				${ASSIMP_ROOT_DIR}/lib
		)

		message("ASSIMP LIBRARY DIR: " ${ASSIMP_LIBRARY_DIR})
		
		find_library(ASSIMP_LIBRARY_RELEASE				assimp-${ASSIMP_MSVC_VERSION}-mt.lib 			PATHS ${ASSIMP_LIBRARY_DIR})
		find_library(ASSIMP_LIBRARY_DEBUG				assimp-${ASSIMP_MSVC_VERSION}-mtd.lib			PATHS ${ASSIMP_LIBRARY_DIR})
		
		set(ASSIMP_LIBRARY 
			optimized 	${ASSIMP_LIBRARY_RELEASE}
			debug		${ASSIMP_LIBRARY_DEBUG}
		)
		
		set(ASSIMP_LIBRARIES)
		if (NOT ${ASSIMP_LIBRARY_RELEASE} STREQUAL "ASSIMP_LIBRARY_RELEASE-NOTFOUND")
			list(APPEND ASSIMP_LIBRARIES ${ASSIMP_LIBRARY_RELEASE})
		endif (NOT ${ASSIMP_LIBRARY_RELEASE} STREQUAL "ASSIMP_LIBRARY_RELEASE-NOTFOUND")
		if (NOT ${ASSIMP_LIBRARY_DEBUG} STREQUAL "ASSIMP_LIBRARY_DEBUG-NOTFOUND")
			list(APPEND ASSIMP_LIBRARIES ${ASSIMP_LIBRARY_DEBUG})
		endif (NOT ${ASSIMP_LIBRARY_DEBUG} STREQUAL "ASSIMP_LIBRARY_DEBUG-NOTFOUND")

		FUNCTION(ASSIMP_COPY_BINARIES TargetDirectory)
			ADD_CUSTOM_TARGET(AssimpCopyBinaries
				COMMAND ${CMAKE_COMMAND} -E copy ${ASSIMP_ROOT_DIR}/bin${ASSIMP_ARCHITECTURE}/assimp-${ASSIMP_MSVC_VERSION}-mtd.dll 	${TargetDirectory}/Debug/assimp-${ASSIMP_MSVC_VERSION}-mtd.dll
				COMMAND ${CMAKE_COMMAND} -E copy ${ASSIMP_ROOT_DIR}/bin${ASSIMP_ARCHITECTURE}/assimp-${ASSIMP_MSVC_VERSION}-mt.dll 		${TargetDirectory}/Release/assimp-${ASSIMP_MSVC_VERSION}-mt.dll
			COMMENT "Copying Assimp binaries to '${TargetDirectory}'"
			VERBATIM)
		ENDFUNCTION(ASSIMP_COPY_BINARIES)

		set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR})

		# handle the QUIETLY and REQUIRED arguments and set SOIL_FOUND to TRUE if
		# all listed variables are TRUE
		INCLUDE(FindPackageHandleStandardArgs)
		FIND_PACKAGE_HANDLE_STANDARD_ARGS(assimp DEFAULT_MSG ASSIMP_LIBRARIES ASSIMP_INCLUDE_DIRS)
	
	endif(MSVC12 OR MSVC14)
	
else(WIN32)

	find_path(
	  ASSIMP_INCLUDE_DIRS
	  NAMES assimp/postprocess.h assimp/scene.h assimp/version.h assimp/config.h assimp/cimport.h
	  PATHS /usr/local/include/ ${ASSIMP_ROOT_DIR}/include
	)

	find_library(
	  ASSIMP_LIBRARIES
	  NAMES assimp
	  PATHS /usr/local/lib/ ${ASSIMP_ROOT_DIR}/lib
	)
	
  # handle the QUIETLY and REQUIRED arguments and set ASSIMP_FOUND to TRUE if # all listed variables are TRUE
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(assimp DEFAULT_MSG ASSIMP_LIBRARIES ASSIMP_INCLUDE_DIRS)

endif(WIN32)
