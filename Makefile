OPTIMIZATIONS = -fpermissive #-O3 -funroll-loops 
INCLUDE_PATHS = -I/home/andrei/Documents/Programming/Libraries/glm/glm/ -I/usr/local/include/
MACROS = -DGRAVITY -DGRAVITY_VALUE=3
OBJECT_DIR = Objects
SOURCES := $(shell find ./ -name '*.cpp')
OBJECTS = $(patsubst %.cpp, %.o, $(notdir $(SOURCES)))
LIBRARIES = -lGL -lGLU -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lSDL2 -lSDL2_image

PlanetSimulator: $(addprefix Objects/, $(OBJECTS))
	g++ $(OPTIMIZATIONS) $^ -o $@ $(LIBRARIES)

$(OBJECT_DIR)/%.o: %.cpp
	g++ $(OPTIMIZATIONS) $(MACROS) $(INCLUDE_PATHS) $< -c -o $@

clean:
	rm $(OBJECT_DIR)/*.o
