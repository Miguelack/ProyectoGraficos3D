CXX = g++
CXXFLAGS = -std=c++11 -Wall -I src
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system  # Enlazar SFML

# Directorio fuente
SRC_DIR = src

# Lista de archivos fuente (rutas relativas a SRC_DIR)
SRC = $(SRC_DIR)/principal.cpp \
      $(SRC_DIR)/Cache/Cache.cpp \
      $(SRC_DIR)/Graficos/Graficos.cpp \
      $(SRC_DIR)/DataGenerators/GeneradorDatos.cpp \
      $(SRC_DIR)/DataGenerators/GeneradorModelos3D.cpp \
      $(SRC_DIR)/DataLoaders/CargadorDatos.cpp \
      $(SRC_DIR)/DataLoaders/LectorModelos3D.cpp

# Lista de archivos objeto (se generan automáticamente)
OBJ = $(SRC:.cpp=.o)

# Nombre del ejecutable
TARGET = proyecto_graficos_3d

# Regla principal
all: $(TARGET)

# Regla para construir el ejecutable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Regla para compilar archivos .cpp a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJ) $(TARGET)