macro(utils_relativify_path path)
    string(REGEX REPLACE [[^\.]] ${PROJECT_SOURCE_DIR} ${path} "${${path}}")
endmacro()