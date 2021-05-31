# --- For testing ---
set(GCSTR_ENABLED, True)

# --- Set information about where the library is ---
find_path(GCSTR_INCLUDE_DIR "include/gcstr")
find_library(GCSTR_LIBRARIES NAMES gcstr)
