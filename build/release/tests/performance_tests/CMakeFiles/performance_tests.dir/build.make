# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yorozuya/monero

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yorozuya/monero/build/release

# Include any dependencies generated for this target.
include tests/performance_tests/CMakeFiles/performance_tests.dir/depend.make

# Include the progress variables for this target.
include tests/performance_tests/CMakeFiles/performance_tests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/performance_tests/CMakeFiles/performance_tests.dir/flags.make

tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o: tests/performance_tests/CMakeFiles/performance_tests.dir/flags.make
tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o: ../../tests/performance_tests/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yorozuya/monero/build/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o"
	cd /home/yorozuya/monero/build/release/tests/performance_tests && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/performance_tests.dir/main.cpp.o -c /home/yorozuya/monero/tests/performance_tests/main.cpp

tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/performance_tests.dir/main.cpp.i"
	cd /home/yorozuya/monero/build/release/tests/performance_tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yorozuya/monero/tests/performance_tests/main.cpp > CMakeFiles/performance_tests.dir/main.cpp.i

tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/performance_tests.dir/main.cpp.s"
	cd /home/yorozuya/monero/build/release/tests/performance_tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yorozuya/monero/tests/performance_tests/main.cpp -o CMakeFiles/performance_tests.dir/main.cpp.s

tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o.requires:

.PHONY : tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o.requires

tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o.provides: tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o.requires
	$(MAKE) -f tests/performance_tests/CMakeFiles/performance_tests.dir/build.make tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o.provides.build
.PHONY : tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o.provides

tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o.provides.build: tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o


# Object files for target performance_tests
performance_tests_OBJECTS = \
"CMakeFiles/performance_tests.dir/main.cpp.o"

# External object files for target performance_tests
performance_tests_EXTERNAL_OBJECTS =

tests/performance_tests/performance_tests: tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o
tests/performance_tests/performance_tests: tests/performance_tests/CMakeFiles/performance_tests.dir/build.make
tests/performance_tests/performance_tests: src/cryptonote_core/libcryptonote_core.a
tests/performance_tests/performance_tests: src/common/libcommon.a
tests/performance_tests/performance_tests: src/crypto/libcrypto.a
tests/performance_tests/performance_tests: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
tests/performance_tests/performance_tests: /usr/lib/x86_64-linux-gnu/librt.so
tests/performance_tests/performance_tests: contrib/otshell_utils/libotshell_utils.a
tests/performance_tests/performance_tests: src/blockchain_db/libblockchain_db.a
tests/performance_tests/performance_tests: external/db_drivers/liblmdb/liblmdb.a
tests/performance_tests/performance_tests: src/ringct/libringct.a
tests/performance_tests/performance_tests: src/common/libcommon.a
tests/performance_tests/performance_tests: /usr/lib/x86_64-linux-gnu/libunbound.so
tests/performance_tests/performance_tests: src/crypto/libcrypto.a
tests/performance_tests/performance_tests: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
tests/performance_tests/performance_tests: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
tests/performance_tests/performance_tests: /usr/lib/x86_64-linux-gnu/libboost_serialization.so
tests/performance_tests/performance_tests: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
tests/performance_tests/performance_tests: /usr/lib/x86_64-linux-gnu/libboost_system.so
tests/performance_tests/performance_tests: /usr/lib/x86_64-linux-gnu/libboost_thread.so
tests/performance_tests/performance_tests: src/blocks/libblocks.a
tests/performance_tests/performance_tests: /usr/lib/x86_64-linux-gnu/librt.so
tests/performance_tests/performance_tests: tests/performance_tests/CMakeFiles/performance_tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yorozuya/monero/build/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable performance_tests"
	cd /home/yorozuya/monero/build/release/tests/performance_tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/performance_tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/performance_tests/CMakeFiles/performance_tests.dir/build: tests/performance_tests/performance_tests

.PHONY : tests/performance_tests/CMakeFiles/performance_tests.dir/build

tests/performance_tests/CMakeFiles/performance_tests.dir/requires: tests/performance_tests/CMakeFiles/performance_tests.dir/main.cpp.o.requires

.PHONY : tests/performance_tests/CMakeFiles/performance_tests.dir/requires

tests/performance_tests/CMakeFiles/performance_tests.dir/clean:
	cd /home/yorozuya/monero/build/release/tests/performance_tests && $(CMAKE_COMMAND) -P CMakeFiles/performance_tests.dir/cmake_clean.cmake
.PHONY : tests/performance_tests/CMakeFiles/performance_tests.dir/clean

tests/performance_tests/CMakeFiles/performance_tests.dir/depend:
	cd /home/yorozuya/monero/build/release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yorozuya/monero /home/yorozuya/monero/tests/performance_tests /home/yorozuya/monero/build/release /home/yorozuya/monero/build/release/tests/performance_tests /home/yorozuya/monero/build/release/tests/performance_tests/CMakeFiles/performance_tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/performance_tests/CMakeFiles/performance_tests.dir/depend

