macro(add_copy_files_target TARGET_NAME DIR GLOBPAT DESTINATION)
  if (IS_ABSOLUTE ${DIR})
    file(GLOB COPY_FILES
    RELATIVE ${DIR}
    ${DIR}/${GLOBPAT})
  else (IS_ABSOLUTE ${DIR})
    file(GLOB COPY_FILES
      RELATIVE ${CMAKE_SOURCE_DIR}/${DIR}
      ${DIR}/${GLOBPAT})
  endif (IS_ABSOLUTE ${DIR})
  
  if (NOT TARGET ${TARGET_NAME})
    add_custom_target(${TARGET_NAME} ALL
      COMMENT "Copying files: ${GLOBPAT}")
      message("DIR: " ${DIR})
  endif(NOT TARGET ${TARGET_NAME})

  foreach(FILENAME ${COPY_FILES})
    if (IS_ABSOLUTE ${DIR})
      set(SRC "${DIR}/${FILENAME}")
    else (IS_ABSOLUTE ${DIR})
      set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/${FILENAME}")
    endif (IS_ABSOLUTE ${DIR})

    if (IS_ABSOLUTE ${DESTINATION})
      set(DST "${DESTINATION}/${FILENAME}")
    else (IS_ABSOLUTE ${DESTINATION})
      set(DST "${CMAKE_CURRENT_SOURCE_DIR}/${DESTINATION}/${FILENAME}")
    endif(IS_ABSOLUTE ${DESTINATION})

    add_custom_command(
      TARGET ${TARGET_NAME}
      COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST}
      )
  endforeach(FILENAME)
endmacro(add_copy_files_target)
