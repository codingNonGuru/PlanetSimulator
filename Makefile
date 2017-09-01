# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/andrei/Documents/Programming/Projects/PlanetSimulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andrei/Documents/Programming/Projects/PlanetSimulator

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/andrei/Documents/Programming/Projects/PlanetSimulator/CMakeFiles /home/andrei/Documents/Programming/Projects/PlanetSimulator/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/andrei/Documents/Programming/Projects/PlanetSimulator/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named PlanetSimulator

# Build rule for target.
PlanetSimulator: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 PlanetSimulator
.PHONY : PlanetSimulator

# fast build rule for target.
PlanetSimulator/fast:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/build
.PHONY : PlanetSimulator/fast

Source/Buffer.o: Source/Buffer.cpp.o

.PHONY : Source/Buffer.o

# target to build an object file
Source/Buffer.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Buffer.cpp.o
.PHONY : Source/Buffer.cpp.o

Source/Buffer.i: Source/Buffer.cpp.i

.PHONY : Source/Buffer.i

# target to preprocess a source file
Source/Buffer.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Buffer.cpp.i
.PHONY : Source/Buffer.cpp.i

Source/Buffer.s: Source/Buffer.cpp.s

.PHONY : Source/Buffer.s

# target to generate assembly for a file
Source/Buffer.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Buffer.cpp.s
.PHONY : Source/Buffer.cpp.s

Source/Collider.o: Source/Collider.cpp.o

.PHONY : Source/Collider.o

# target to build an object file
Source/Collider.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Collider.cpp.o
.PHONY : Source/Collider.cpp.o

Source/Collider.i: Source/Collider.cpp.i

.PHONY : Source/Collider.i

# target to preprocess a source file
Source/Collider.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Collider.cpp.i
.PHONY : Source/Collider.cpp.i

Source/Collider.s: Source/Collider.cpp.s

.PHONY : Source/Collider.s

# target to generate assembly for a file
Source/Collider.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Collider.cpp.s
.PHONY : Source/Collider.cpp.s

Source/Controller.o: Source/Controller.cpp.o

.PHONY : Source/Controller.o

# target to build an object file
Source/Controller.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Controller.cpp.o
.PHONY : Source/Controller.cpp.o

Source/Controller.i: Source/Controller.cpp.i

.PHONY : Source/Controller.i

# target to preprocess a source file
Source/Controller.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Controller.cpp.i
.PHONY : Source/Controller.cpp.i

Source/Controller.s: Source/Controller.cpp.s

.PHONY : Source/Controller.s

# target to generate assembly for a file
Source/Controller.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Controller.cpp.s
.PHONY : Source/Controller.cpp.s

Source/Engine.o: Source/Engine.cpp.o

.PHONY : Source/Engine.o

# target to build an object file
Source/Engine.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Engine.cpp.o
.PHONY : Source/Engine.cpp.o

Source/Engine.i: Source/Engine.cpp.i

.PHONY : Source/Engine.i

# target to preprocess a source file
Source/Engine.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Engine.cpp.i
.PHONY : Source/Engine.cpp.i

Source/Engine.s: Source/Engine.cpp.s

.PHONY : Source/Engine.s

# target to generate assembly for a file
Source/Engine.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Engine.cpp.s
.PHONY : Source/Engine.cpp.s

Source/EventHandler.o: Source/EventHandler.cpp.o

.PHONY : Source/EventHandler.o

# target to build an object file
Source/EventHandler.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/EventHandler.cpp.o
.PHONY : Source/EventHandler.cpp.o

Source/EventHandler.i: Source/EventHandler.cpp.i

.PHONY : Source/EventHandler.i

# target to preprocess a source file
Source/EventHandler.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/EventHandler.cpp.i
.PHONY : Source/EventHandler.cpp.i

Source/EventHandler.s: Source/EventHandler.cpp.s

.PHONY : Source/EventHandler.s

# target to generate assembly for a file
Source/EventHandler.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/EventHandler.cpp.s
.PHONY : Source/EventHandler.cpp.s

Source/Explosion.o: Source/Explosion.cpp.o

.PHONY : Source/Explosion.o

# target to build an object file
Source/Explosion.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Explosion.cpp.o
.PHONY : Source/Explosion.cpp.o

Source/Explosion.i: Source/Explosion.cpp.i

.PHONY : Source/Explosion.i

# target to preprocess a source file
Source/Explosion.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Explosion.cpp.i
.PHONY : Source/Explosion.cpp.i

Source/Explosion.s: Source/Explosion.cpp.s

.PHONY : Source/Explosion.s

# target to generate assembly for a file
Source/Explosion.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Explosion.cpp.s
.PHONY : Source/Explosion.cpp.s

Source/Fractal.o: Source/Fractal.cpp.o

.PHONY : Source/Fractal.o

# target to build an object file
Source/Fractal.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Fractal.cpp.o
.PHONY : Source/Fractal.cpp.o

Source/Fractal.i: Source/Fractal.cpp.i

.PHONY : Source/Fractal.i

# target to preprocess a source file
Source/Fractal.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Fractal.cpp.i
.PHONY : Source/Fractal.cpp.i

Source/Fractal.s: Source/Fractal.cpp.s

.PHONY : Source/Fractal.s

# target to generate assembly for a file
Source/Fractal.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Fractal.cpp.s
.PHONY : Source/Fractal.cpp.s

Source/Framebuffer.o: Source/Framebuffer.cpp.o

.PHONY : Source/Framebuffer.o

# target to build an object file
Source/Framebuffer.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Framebuffer.cpp.o
.PHONY : Source/Framebuffer.cpp.o

Source/Framebuffer.i: Source/Framebuffer.cpp.i

.PHONY : Source/Framebuffer.i

# target to preprocess a source file
Source/Framebuffer.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Framebuffer.cpp.i
.PHONY : Source/Framebuffer.cpp.i

Source/Framebuffer.s: Source/Framebuffer.cpp.s

.PHONY : Source/Framebuffer.s

# target to generate assembly for a file
Source/Framebuffer.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Framebuffer.cpp.s
.PHONY : Source/Framebuffer.cpp.s

Source/GameObject.o: Source/GameObject.cpp.o

.PHONY : Source/GameObject.o

# target to build an object file
Source/GameObject.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/GameObject.cpp.o
.PHONY : Source/GameObject.cpp.o

Source/GameObject.i: Source/GameObject.cpp.i

.PHONY : Source/GameObject.i

# target to preprocess a source file
Source/GameObject.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/GameObject.cpp.i
.PHONY : Source/GameObject.cpp.i

Source/GameObject.s: Source/GameObject.cpp.s

.PHONY : Source/GameObject.s

# target to generate assembly for a file
Source/GameObject.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/GameObject.cpp.s
.PHONY : Source/GameObject.cpp.s

Source/Memory.o: Source/Memory.cpp.o

.PHONY : Source/Memory.o

# target to build an object file
Source/Memory.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Memory.cpp.o
.PHONY : Source/Memory.cpp.o

Source/Memory.i: Source/Memory.cpp.i

.PHONY : Source/Memory.i

# target to preprocess a source file
Source/Memory.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Memory.cpp.i
.PHONY : Source/Memory.cpp.i

Source/Memory.s: Source/Memory.cpp.s

.PHONY : Source/Memory.s

# target to generate assembly for a file
Source/Memory.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Memory.cpp.s
.PHONY : Source/Memory.cpp.s

Source/Mesh.o: Source/Mesh.cpp.o

.PHONY : Source/Mesh.o

# target to build an object file
Source/Mesh.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Mesh.cpp.o
.PHONY : Source/Mesh.cpp.o

Source/Mesh.i: Source/Mesh.cpp.i

.PHONY : Source/Mesh.i

# target to preprocess a source file
Source/Mesh.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Mesh.cpp.i
.PHONY : Source/Mesh.cpp.i

Source/Mesh.s: Source/Mesh.cpp.s

.PHONY : Source/Mesh.s

# target to generate assembly for a file
Source/Mesh.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Mesh.cpp.s
.PHONY : Source/Mesh.cpp.s

Source/Planet.o: Source/Planet.cpp.o

.PHONY : Source/Planet.o

# target to build an object file
Source/Planet.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Planet.cpp.o
.PHONY : Source/Planet.cpp.o

Source/Planet.i: Source/Planet.cpp.i

.PHONY : Source/Planet.i

# target to preprocess a source file
Source/Planet.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Planet.cpp.i
.PHONY : Source/Planet.cpp.i

Source/Planet.s: Source/Planet.cpp.s

.PHONY : Source/Planet.s

# target to generate assembly for a file
Source/Planet.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Planet.cpp.s
.PHONY : Source/Planet.cpp.s

Source/PlanetSimulator.o: Source/PlanetSimulator.cpp.o

.PHONY : Source/PlanetSimulator.o

# target to build an object file
Source/PlanetSimulator.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/PlanetSimulator.cpp.o
.PHONY : Source/PlanetSimulator.cpp.o

Source/PlanetSimulator.i: Source/PlanetSimulator.cpp.i

.PHONY : Source/PlanetSimulator.i

# target to preprocess a source file
Source/PlanetSimulator.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/PlanetSimulator.cpp.i
.PHONY : Source/PlanetSimulator.cpp.i

Source/PlanetSimulator.s: Source/PlanetSimulator.cpp.s

.PHONY : Source/PlanetSimulator.s

# target to generate assembly for a file
Source/PlanetSimulator.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/PlanetSimulator.cpp.s
.PHONY : Source/PlanetSimulator.cpp.s

Source/Renderer.o: Source/Renderer.cpp.o

.PHONY : Source/Renderer.o

# target to build an object file
Source/Renderer.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Renderer.cpp.o
.PHONY : Source/Renderer.cpp.o

Source/Renderer.i: Source/Renderer.cpp.i

.PHONY : Source/Renderer.i

# target to preprocess a source file
Source/Renderer.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Renderer.cpp.i
.PHONY : Source/Renderer.cpp.i

Source/Renderer.s: Source/Renderer.cpp.s

.PHONY : Source/Renderer.s

# target to generate assembly for a file
Source/Renderer.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Renderer.cpp.s
.PHONY : Source/Renderer.cpp.s

Source/RigidBody.o: Source/RigidBody.cpp.o

.PHONY : Source/RigidBody.o

# target to build an object file
Source/RigidBody.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/RigidBody.cpp.o
.PHONY : Source/RigidBody.cpp.o

Source/RigidBody.i: Source/RigidBody.cpp.i

.PHONY : Source/RigidBody.i

# target to preprocess a source file
Source/RigidBody.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/RigidBody.cpp.i
.PHONY : Source/RigidBody.cpp.i

Source/RigidBody.s: Source/RigidBody.cpp.s

.PHONY : Source/RigidBody.s

# target to generate assembly for a file
Source/RigidBody.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/RigidBody.cpp.s
.PHONY : Source/RigidBody.cpp.s

Source/Scene.o: Source/Scene.cpp.o

.PHONY : Source/Scene.o

# target to build an object file
Source/Scene.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Scene.cpp.o
.PHONY : Source/Scene.cpp.o

Source/Scene.i: Source/Scene.cpp.i

.PHONY : Source/Scene.i

# target to preprocess a source file
Source/Scene.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Scene.cpp.i
.PHONY : Source/Scene.cpp.i

Source/Scene.s: Source/Scene.cpp.s

.PHONY : Source/Scene.s

# target to generate assembly for a file
Source/Scene.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Scene.cpp.s
.PHONY : Source/Scene.cpp.s

Source/Shader.o: Source/Shader.cpp.o

.PHONY : Source/Shader.o

# target to build an object file
Source/Shader.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Shader.cpp.o
.PHONY : Source/Shader.cpp.o

Source/Shader.i: Source/Shader.cpp.i

.PHONY : Source/Shader.i

# target to preprocess a source file
Source/Shader.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Shader.cpp.i
.PHONY : Source/Shader.cpp.i

Source/Shader.s: Source/Shader.cpp.s

.PHONY : Source/Shader.s

# target to generate assembly for a file
Source/Shader.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Shader.cpp.s
.PHONY : Source/Shader.cpp.s

Source/Spaceship.o: Source/Spaceship.cpp.o

.PHONY : Source/Spaceship.o

# target to build an object file
Source/Spaceship.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Spaceship.cpp.o
.PHONY : Source/Spaceship.cpp.o

Source/Spaceship.i: Source/Spaceship.cpp.i

.PHONY : Source/Spaceship.i

# target to preprocess a source file
Source/Spaceship.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Spaceship.cpp.i
.PHONY : Source/Spaceship.cpp.i

Source/Spaceship.s: Source/Spaceship.cpp.s

.PHONY : Source/Spaceship.s

# target to generate assembly for a file
Source/Spaceship.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Spaceship.cpp.s
.PHONY : Source/Spaceship.cpp.s

Source/Sprite.o: Source/Sprite.cpp.o

.PHONY : Source/Sprite.o

# target to build an object file
Source/Sprite.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Sprite.cpp.o
.PHONY : Source/Sprite.cpp.o

Source/Sprite.i: Source/Sprite.cpp.i

.PHONY : Source/Sprite.i

# target to preprocess a source file
Source/Sprite.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Sprite.cpp.i
.PHONY : Source/Sprite.cpp.i

Source/Sprite.s: Source/Sprite.cpp.s

.PHONY : Source/Sprite.s

# target to generate assembly for a file
Source/Sprite.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Sprite.cpp.s
.PHONY : Source/Sprite.cpp.s

Source/Texture.o: Source/Texture.cpp.o

.PHONY : Source/Texture.o

# target to build an object file
Source/Texture.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Texture.cpp.o
.PHONY : Source/Texture.cpp.o

Source/Texture.i: Source/Texture.cpp.i

.PHONY : Source/Texture.i

# target to preprocess a source file
Source/Texture.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Texture.cpp.i
.PHONY : Source/Texture.cpp.i

Source/Texture.s: Source/Texture.cpp.s

.PHONY : Source/Texture.s

# target to generate assembly for a file
Source/Texture.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Texture.cpp.s
.PHONY : Source/Texture.cpp.s

Source/Transform.o: Source/Transform.cpp.o

.PHONY : Source/Transform.o

# target to build an object file
Source/Transform.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Transform.cpp.o
.PHONY : Source/Transform.cpp.o

Source/Transform.i: Source/Transform.cpp.i

.PHONY : Source/Transform.i

# target to preprocess a source file
Source/Transform.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Transform.cpp.i
.PHONY : Source/Transform.cpp.i

Source/Transform.s: Source/Transform.cpp.s

.PHONY : Source/Transform.s

# target to generate assembly for a file
Source/Transform.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Transform.cpp.s
.PHONY : Source/Transform.cpp.s

Source/Utility.o: Source/Utility.cpp.o

.PHONY : Source/Utility.o

# target to build an object file
Source/Utility.cpp.o:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Utility.cpp.o
.PHONY : Source/Utility.cpp.o

Source/Utility.i: Source/Utility.cpp.i

.PHONY : Source/Utility.i

# target to preprocess a source file
Source/Utility.cpp.i:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Utility.cpp.i
.PHONY : Source/Utility.cpp.i

Source/Utility.s: Source/Utility.cpp.s

.PHONY : Source/Utility.s

# target to generate assembly for a file
Source/Utility.cpp.s:
	$(MAKE) -f CMakeFiles/PlanetSimulator.dir/build.make CMakeFiles/PlanetSimulator.dir/Source/Utility.cpp.s
.PHONY : Source/Utility.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... PlanetSimulator"
	@echo "... Source/Buffer.o"
	@echo "... Source/Buffer.i"
	@echo "... Source/Buffer.s"
	@echo "... Source/Collider.o"
	@echo "... Source/Collider.i"
	@echo "... Source/Collider.s"
	@echo "... Source/Controller.o"
	@echo "... Source/Controller.i"
	@echo "... Source/Controller.s"
	@echo "... Source/Engine.o"
	@echo "... Source/Engine.i"
	@echo "... Source/Engine.s"
	@echo "... Source/EventHandler.o"
	@echo "... Source/EventHandler.i"
	@echo "... Source/EventHandler.s"
	@echo "... Source/Explosion.o"
	@echo "... Source/Explosion.i"
	@echo "... Source/Explosion.s"
	@echo "... Source/Fractal.o"
	@echo "... Source/Fractal.i"
	@echo "... Source/Fractal.s"
	@echo "... Source/Framebuffer.o"
	@echo "... Source/Framebuffer.i"
	@echo "... Source/Framebuffer.s"
	@echo "... Source/GameObject.o"
	@echo "... Source/GameObject.i"
	@echo "... Source/GameObject.s"
	@echo "... Source/Memory.o"
	@echo "... Source/Memory.i"
	@echo "... Source/Memory.s"
	@echo "... Source/Mesh.o"
	@echo "... Source/Mesh.i"
	@echo "... Source/Mesh.s"
	@echo "... Source/Planet.o"
	@echo "... Source/Planet.i"
	@echo "... Source/Planet.s"
	@echo "... Source/PlanetSimulator.o"
	@echo "... Source/PlanetSimulator.i"
	@echo "... Source/PlanetSimulator.s"
	@echo "... Source/Renderer.o"
	@echo "... Source/Renderer.i"
	@echo "... Source/Renderer.s"
	@echo "... Source/RigidBody.o"
	@echo "... Source/RigidBody.i"
	@echo "... Source/RigidBody.s"
	@echo "... Source/Scene.o"
	@echo "... Source/Scene.i"
	@echo "... Source/Scene.s"
	@echo "... Source/Shader.o"
	@echo "... Source/Shader.i"
	@echo "... Source/Shader.s"
	@echo "... Source/Spaceship.o"
	@echo "... Source/Spaceship.i"
	@echo "... Source/Spaceship.s"
	@echo "... Source/Sprite.o"
	@echo "... Source/Sprite.i"
	@echo "... Source/Sprite.s"
	@echo "... Source/Texture.o"
	@echo "... Source/Texture.i"
	@echo "... Source/Texture.s"
	@echo "... Source/Transform.o"
	@echo "... Source/Transform.i"
	@echo "... Source/Transform.s"
	@echo "... Source/Utility.o"
	@echo "... Source/Utility.i"
	@echo "... Source/Utility.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

