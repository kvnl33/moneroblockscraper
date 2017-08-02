file(REMOVE_RECURSE
  "libcrypto.pdb"
  "libcrypto.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/crypto.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
