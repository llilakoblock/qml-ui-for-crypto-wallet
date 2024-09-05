set(EXCLUDED_FILES_AND_DIRS
    ${RUNTIME_OUTPUT_DIRECTORY}/server.cert
    ${RUNTIME_OUTPUT_DIRECTORY}/server.key
    ${RUNTIME_OUTPUT_DIRECTORY}/freedom.keys
    ${RUNTIME_OUTPUT_DIRECTORY}/config.ini
    ${RUNTIME_OUTPUT_DIRECTORY}/tor)

file(
    GLOB_RECURSE
        FILES_TO_REMOVE
    LIST_DIRECTORIES false
    ${BINARY_DIR}/*
)

foreach(FILE ${FILES_TO_REMOVE})
    set(IS_REMOVE true)

    foreach(SUBSTR ${EXCLUDED_FILES_AND_DIRS})
        string(FIND ${FILE} ${SUBSTR} POS)
        if (POS GREATER -1)
            set(IS_REMOVE false)
            break()
        endif()
    endforeach(SUBSTR EXCLUDED_FILES_AND_DIRS)

    if (IS_REMOVE)
        file(REMOVE_RECURSE ${FILE})
    endif()
endforeach(FILE FILES_TO_REMOVE)

file(
    GLOB_RECURSE
        DIRS_TO_REMOVE
    LIST_DIRECTORIES true
    ${BINARY_DIR}/*
)

foreach(DIR ${DIRS_TO_REMOVE})
    set(IS_REMOVE true)

    foreach(SUBSTR ${EXCLUDED_FILES_AND_DIRS})
        string(FIND ${DIR} ${SUBSTR} POS)
        if (POS GREATER -1)
            set(IS_REMOVE false)
            break()
        endif()
        if (DIR STREQUAL RUNTIME_OUTPUT_DIRECTORY)
            set(IS_REMOVE false)
            break()
        endif()
    endforeach(SUBSTR EXCLUDED_FILES_AND_DIRS)

    if (IS_REMOVE)
        file(REMOVE_RECURSE ${DIR})
    endif()
endforeach(DIR DIRS_TO_REMOVE)
