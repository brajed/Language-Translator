# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/LanguageTranslator_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/LanguageTranslator_autogen.dir/ParseCache.txt"
  "LanguageTranslator_autogen"
  )
endif()
