if(WIN32)
    set(DATA_PATH "$ENV{APPDATA}")
elseif(UNIX)
    set(DATA_PATH "$ENV{HOME}/.local/share")
elseif(APPLE)
    set(DATA_PATH "$ENV{HOME}/Library/Application Support")
endif()

set(FIMA_DATA_PATH "${DATA_PATH}/fima")

file(MAKE_DIRECTORY "${FIMA_DATA_PATH}")

message(STATUS "Creating the fima data directory: ${FIMA_DATA_PATH}")

file(COPY_FILE ./assets/languages.json "${FIMA_DATA_PATH}/languages.json")
file(
    COPY_FILE ./assets/map_language_family.json
    "${FIMA_DATA_PATH}/map_language_family.json"
)
file(
    COPY_FILE ./assets/map_language_name.json
    "${FIMA_DATA_PATH}/map_language_name.json"
)
file(
    COPY_FILE ./assets/map_language_icon.json
    "${FIMA_DATA_PATH}/map_language_icon.json"
)
file(
    COPY_FILE ./assets/map_directory_icon.json
    "${FIMA_DATA_PATH}/map_directory_icon.json"
)

message(STATUS "Copied files")
message(
    STATUS
    "  assets/languages.json           -> ${FIMA_DATA_PATH}/languages.json"
)
message(
    STATUS
    "  assets/map_language_family.json -> ${FIMA_DATA_PATH}/map_language_family.json"
)
message(
    STATUS
    "  assets/map_language_name.json   -> ${FIMA_DATA_PATH}/map_language_name.json"
)
message(
    STATUS
    "  assets/map_language_icon.json   -> ${FIMA_DATA_PATH}/map_language_icon.json"
)
message(
    STATUS
    "  assets/map_directory_icon.json  -> ${FIMA_DATA_PATH}/map_directory_icon.json"
)
