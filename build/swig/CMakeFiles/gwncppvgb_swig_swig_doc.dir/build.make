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
CMAKE_SOURCE_DIR = /home/victor/IIE/GNURadio/gr-gwncppvgb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/victor/IIE/GNURadio/gr-gwncppvgb/build

# Utility rule file for gwncppvgb_swig_swig_doc.

# Include the progress variables for this target.
include swig/CMakeFiles/gwncppvgb_swig_swig_doc.dir/progress.make

swig/CMakeFiles/gwncppvgb_swig_swig_doc: swig/gwncppvgb_swig_doc.i


swig/gwncppvgb_swig_doc.i: swig/gwncppvgb_swig_doc_swig_docs/xml/index.xml
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/victor/IIE/GNURadio/gr-gwncppvgb/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating python docstrings for gwncppvgb_swig_doc"
	cd /home/victor/IIE/GNURadio/gr-gwncppvgb/docs/doxygen && /usr/bin/python2 -B /home/victor/IIE/GNURadio/gr-gwncppvgb/docs/doxygen/swig_doc.py /home/victor/IIE/GNURadio/gr-gwncppvgb/build/swig/gwncppvgb_swig_doc_swig_docs/xml /home/victor/IIE/GNURadio/gr-gwncppvgb/build/swig/gwncppvgb_swig_doc.i

swig/gwncppvgb_swig_doc_swig_docs/xml/index.xml: swig/_gwncppvgb_swig_doc_tag
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/victor/IIE/GNURadio/gr-gwncppvgb/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating doxygen xml for gwncppvgb_swig_doc docs"
	cd /home/victor/IIE/GNURadio/gr-gwncppvgb/build/swig && ./_gwncppvgb_swig_doc_tag
	cd /home/victor/IIE/GNURadio/gr-gwncppvgb/build/swig && /usr/bin/doxygen /home/victor/IIE/GNURadio/gr-gwncppvgb/build/swig/gwncppvgb_swig_doc_swig_docs/Doxyfile

gwncppvgb_swig_swig_doc: swig/CMakeFiles/gwncppvgb_swig_swig_doc
gwncppvgb_swig_swig_doc: swig/gwncppvgb_swig_doc.i
gwncppvgb_swig_swig_doc: swig/gwncppvgb_swig_doc_swig_docs/xml/index.xml
gwncppvgb_swig_swig_doc: swig/CMakeFiles/gwncppvgb_swig_swig_doc.dir/build.make

.PHONY : gwncppvgb_swig_swig_doc

# Rule to build all files generated by this target.
swig/CMakeFiles/gwncppvgb_swig_swig_doc.dir/build: gwncppvgb_swig_swig_doc

.PHONY : swig/CMakeFiles/gwncppvgb_swig_swig_doc.dir/build

swig/CMakeFiles/gwncppvgb_swig_swig_doc.dir/clean:
	cd /home/victor/IIE/GNURadio/gr-gwncppvgb/build/swig && $(CMAKE_COMMAND) -P CMakeFiles/gwncppvgb_swig_swig_doc.dir/cmake_clean.cmake
.PHONY : swig/CMakeFiles/gwncppvgb_swig_swig_doc.dir/clean

swig/CMakeFiles/gwncppvgb_swig_swig_doc.dir/depend:
	cd /home/victor/IIE/GNURadio/gr-gwncppvgb/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victor/IIE/GNURadio/gr-gwncppvgb /home/victor/IIE/GNURadio/gr-gwncppvgb/swig /home/victor/IIE/GNURadio/gr-gwncppvgb/build /home/victor/IIE/GNURadio/gr-gwncppvgb/build/swig /home/victor/IIE/GNURadio/gr-gwncppvgb/build/swig/CMakeFiles/gwncppvgb_swig_swig_doc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : swig/CMakeFiles/gwncppvgb_swig_swig_doc.dir/depend

