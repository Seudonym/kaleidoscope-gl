CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lglfw -lGLEW -lGL

SOURCES=Main.cpp ./utils/ShaderProgram.cpp ./imgui/imgui.cpp ./imgui/imgui_draw.cpp ./imgui/imgui_tables.cpp ./imgui/imgui_widgets.cpp ./imgui/imgui_impl_glfw.cpp ./imgui/imgui_impl_opengl3.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=../build/dev

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	$(EXECUTABLE)
