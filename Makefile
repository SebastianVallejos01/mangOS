

# 1. Nombre del ejecutable final
TARGET = mangOS

# 2. Compilador y banderas (flags)
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I./SRC

# 3. Búsqueda automática de archivos fuente
# Esto buscará cualquier archivo .cpp dentro de cualquier subcarpeta de SRC
SRCS = $(wildcard SRC/*/*.cpp)

# 4. Transformar la lista de .cpp a una lista de objetos .o
OBJS = $(SRCS:.cpp=.o)


# Reglas de Compilación

# Regla principal: se ejecuta al escribir simplemente 'make'
all: $(TARGET)

# Cómo construir el ejecutable final enlazando los objetos
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Cómo compilar cada archivo fuente (.cpp) a un archivo objeto (.o)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados al escribir 'make clean'
clean:
	rm -f $(OBJS) $(TARGET)