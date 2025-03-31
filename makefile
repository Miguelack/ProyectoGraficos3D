# Configuración del compilador
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -march=native -I src
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lX11

# Directorios
SRC_DIR = src
BUILD_DIR = build

# Fuentes principales (todos en Graficos/)
SRC = $(SRC_DIR)/principal.cpp \
      $(SRC_DIR)/Cache/Cache.cpp \
      $(SRC_DIR)/Graficos/Graficos.cpp \
      $(SRC_DIR)/Graficos/ModelViewer.cpp \
      $(SRC_DIR)/Graficos/Renderer.cpp \
      $(SRC_DIR)/Graficos/UIHandler.cpp \
      $(SRC_DIR)/Graficos/InputHandler.cpp \
      $(SRC_DIR)/Graficos/CameraController.cpp \
      $(SRC_DIR)/DataGenerators/GeneradorDatos.cpp \
      $(SRC_DIR)/DataGenerators/GeneradorModelos3D.cpp \
      $(SRC_DIR)/DataLoaders/CargadorDatos.cpp \
      $(SRC_DIR)/DataLoaders/LectorModelos3D.cpp

# Generar lista de objetos
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

# Nombre del ejecutable
TARGET = proyecto_graficos_3d

# Regla principal
all: create_dirs $(TARGET)

# Crear directorios (solo Graficos, no subdirectorios)
create_dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/Cache
	@mkdir -p $(BUILD_DIR)/Graficos
	@mkdir -p $(BUILD_DIR)/DataGenerators
	@mkdir -p $(BUILD_DIR)/DataLoaders

# Regla para el ejecutable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Regla para objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependencias principales
$(BUILD_DIR)/principal.o: $(SRC_DIR)/principal.cpp $(SRC_DIR)/Graficos/ModelViewer.hpp
$(BUILD_DIR)/Graficos/ModelViewer.o: $(SRC_DIR)/Graficos/ModelViewer.cpp $(SRC_DIR)/Graficos/ModelViewer.hpp

# Limpieza
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all create_dirs clean