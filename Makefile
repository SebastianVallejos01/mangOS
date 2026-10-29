# 1. Nombre del ejecutable final
TARGET = mangOS
MULTI_TARGET = multi
SERVER_TARGET = servidor_juego
CLIENT_TARGET = cliente_juego

# 2. Compilador y banderas (flags)
ifeq ($(OS),Windows_NT)
    # En Windows, si existe clang++ moderno con soporte C++17 en GHCup, se utiliza preferentemente
    CLANG_PATH = C:/ghcup/ghc/9.10.3/mingw/bin/clang++.exe
    ifneq ($(wildcard $(CLANG_PATH)),)
        CXX = $(CLANG_PATH)
    else
        CXX = g++
    endif
else
    CXX = g++
endif

CXXFLAGS = -Wall -Wextra -std=c++17 -pthread -static -I./SRC

# 3. Búsqueda automática de archivos fuente
# Esto buscará cualquier archivo .cpp dentro de cualquier subcarpeta de SRC, excluyendo matrices.cpp y los del juego
ALL_SRCS = $(wildcard SRC/*/*.cpp)
SRCS = $(filter-out SRC/mates/matrices.cpp SRC/juego/Server/serverStart.cpp SRC/juego/Server/server.cpp SRC/juego/Classes/ClassManager.cpp SRC/juego/Classes/GameState.cpp SRC/juego/Classes/player.cpp SRC/juego/Cliente/cliente.cpp, $(ALL_SRCS))

# 4. Transformar la lista de .cpp a una lista de objetos .o
OBJS = $(SRCS:.cpp=.o)


# Reglas de Compilación

# Regla principal: se ejecuta al escribir simplemente 'make'
all: $(TARGET) $(MULTI_TARGET) $(SERVER_TARGET) $(CLIENT_TARGET)

# Cómo construir el ejecutable final enlazando los objetos
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Cómo compilar el multiplicador de matrices (ejecutable independiente)
$(MULTI_TARGET): SRC/mates/matrices.cpp
	$(CXX) $(CXXFLAGS) SRC/mates/matrices.cpp -o $(MULTI_TARGET)

# Cómo compilar el servidor del juego (incluye userManager.cpp para getEnvFile y getEnvVar)
$(SERVER_TARGET): SRC/juego/Server/serverStart.cpp SRC/juego/Server/server.cpp SRC/juego/Classes/ClassManager.cpp SRC/juego/Classes/GameState.cpp SRC/juego/Classes/player.cpp SRC/userManager/userManager.cpp
	$(CXX) $(CXXFLAGS) SRC/juego/Server/serverStart.cpp SRC/juego/Server/server.cpp SRC/juego/Classes/ClassManager.cpp SRC/juego/Classes/GameState.cpp SRC/juego/Classes/player.cpp SRC/userManager/userManager.cpp -o $(SERVER_TARGET)

# Cómo compilar el cliente del juego
$(CLIENT_TARGET): SRC/juego/Cliente/cliente.cpp
	$(CXX) $(CXXFLAGS) SRC/juego/Cliente/cliente.cpp -o $(CLIENT_TARGET)

# Cómo compilar cada archivo fuente (.cpp) a un archivo objeto (.o)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados al escribir 'make clean'
clean:
ifeq ($(OS),Windows_NT)
	-del /f /q $(subst /,\,$(OBJS)) $(TARGET) $(TARGET).exe $(MULTI_TARGET) $(MULTI_TARGET).exe $(SERVER_TARGET) $(SERVER_TARGET).exe $(CLIENT_TARGET) $(CLIENT_TARGET).exe 2>nul || rm -f $(OBJS) $(TARGET) $(MULTI_TARGET) $(TARGET).exe $(MULTI_TARGET).exe $(SERVER_TARGET) $(SERVER_TARGET).exe $(CLIENT_TARGET) $(CLIENT_TARGET).exe
else
	rm -f $(OBJS) $(TARGET) $(MULTI_TARGET) $(SERVER_TARGET) $(CLIENT_TARGET)
endif