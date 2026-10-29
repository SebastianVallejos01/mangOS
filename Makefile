# 1. Nombres de ejecutables finales
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

# 3. Fuentes para el binario principal mangOS
# Incluye los .cpp de nivel 1 dentro de SRC/ (SistOpe, login, mainMenu, mates, texto, userManager, index, juego/menuJuego)
MANGOS_SRCS = $(filter-out SRC/mates/matrices.cpp, $(wildcard SRC/*/*.cpp))
MANGOS_OBJS = $(MANGOS_SRCS:.cpp=.o)

# Fuentes para el Servidor del Juego
SERVER_SRCS = SRC/juego/Server/serverStart.cpp \
              SRC/juego/Server/server.cpp \
              SRC/juego/Classes/ClassManager.cpp \
              SRC/juego/Classes/GameState.cpp \
              SRC/juego/Classes/player.cpp

# Fuentes para el Cliente del Juego
CLIENT_SRCS = SRC/juego/Cliente/cliente.cpp

# Reglas de Compilación

# Regla principal: se ejecuta al escribir simplemente 'make'
ifeq ($(OS),Windows_NT)
all: $(TARGET) $(MULTI_TARGET)
else
all: $(TARGET) $(MULTI_TARGET) $(SERVER_TARGET) $(CLIENT_TARGET)
endif

# Cómo construir el ejecutable principal enlazando los objetos
$(TARGET): $(MANGOS_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MANGOS_OBJS)

# Cómo compilar el multiplicador de matrices (ejecutable independiente)
$(MULTI_TARGET): SRC/mates/matrices.cpp
	$(CXX) $(CXXFLAGS) SRC/mates/matrices.cpp -o $(MULTI_TARGET)

# Cómo compilar el Servidor del Juego (Requiere entorno Linux / sockets POSIX)
$(SERVER_TARGET): $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) $(SERVER_SRCS) -o $(SERVER_TARGET)

# Cómo compilar el Cliente del Juego (Requiere entorno Linux / sockets POSIX)
$(CLIENT_TARGET): $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRCS) -o $(CLIENT_TARGET)

# Cómo compilar cada archivo fuente (.cpp) a un archivo objeto (.o)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados al escribir 'make clean'
clean:
ifeq ($(OS),Windows_NT)
	-del /f /q $(subst /,\,$(MANGOS_OBJS)) $(TARGET) $(TARGET).exe $(MULTI_TARGET) $(MULTI_TARGET).exe $(SERVER_TARGET) $(SERVER_TARGET).exe $(CLIENT_TARGET) $(CLIENT_TARGET).exe 2>nul || rm -f $(MANGOS_OBJS) $(TARGET) $(MULTI_TARGET) $(SERVER_TARGET) $(CLIENT_TARGET) $(TARGET).exe $(MULTI_TARGET).exe $(SERVER_TARGET).exe $(CLIENT_TARGET).exe
else
	rm -f $(MANGOS_OBJS) $(TARGET) $(MULTI_TARGET) $(SERVER_TARGET) $(CLIENT_TARGET)
endif
