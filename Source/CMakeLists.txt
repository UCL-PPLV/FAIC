set(LLVM_LINK_COMPONENTS support)

add_clang_executable(FAIC
  FAIC.cpp
  )
target_link_libraries(FAIC
  clangTooling
  clangBasic
  clangASTMatchers
  )
