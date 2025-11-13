# Makefile para el proyecto Mandelbrot HPC

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -Wextra -fopenmp
INCLUDES = -Iinclude
LDFLAGS = -fopenmp

# Directorios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
RESULTS_DIR = results
BENCHMARKS_DIR = benchmarks

# Archivos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/mandelbrot

# Colores para output
COLOR_RESET = \033[0m
COLOR_BOLD = \033[1m
COLOR_GREEN = \033[32m
COLOR_BLUE = \033[34m

# Regla por defecto
all: directories $(TARGET)

# Crear directorios necesarios
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(RESULTS_DIR) $(BENCHMARKS_DIR)

# Compilar el ejecutable
$(TARGET): $(OBJECTS)
	@echo "$(COLOR_BLUE)$(COLOR_BOLD)Enlazando ejecutable...$(COLOR_RESET)"
	@$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)✓ Compilación exitosa: $(TARGET)$(COLOR_RESET)"

# Compilar archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "$(COLOR_BLUE)Compilando $<...$(COLOR_RESET)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Ejecutar el programa
run: $(TARGET)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)Ejecutando Mandelbrot HPC...$(COLOR_RESET)"
	@$(TARGET)

# Ejecutar con parámetros personalizados
# Uso: make run-custom WIDTH=800 HEIGHT=600 ITER=500 THREADS=4
run-custom: $(TARGET)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)Ejecutando con parámetros personalizados...$(COLOR_RESET)"
	@$(TARGET) $(WIDTH) $(HEIGHT) $(ITER) $(THREADS)

# Ejecutar benchmark completo
benchmark: $(TARGET)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)Ejecutando benchmark completo...$(COLOR_RESET)"
	@$(TARGET)
	@echo ""
	@echo "$(COLOR_GREEN)Generando gráficas de rendimiento...$(COLOR_RESET)"
	@python3 benchmarks/plot_results.py

# Limpiar archivos generados
clean:
	@echo "$(COLOR_BLUE)Limpiando archivos generados...$(COLOR_RESET)"
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "$(COLOR_GREEN)✓ Limpieza completada$(COLOR_RESET)"

# Limpiar todo incluyendo resultados
clean-all: clean
	@rm -rf $(RESULTS_DIR)/* $(BENCHMARKS_DIR)/*.csv $(BENCHMARKS_DIR)/*.png
	@echo "$(COLOR_GREEN)✓ Limpieza completa$(COLOR_RESET)"

# Mostrar información del sistema
info:
	@echo "$(COLOR_BOLD)=== Información del Sistema ===$(COLOR_RESET)"
	@echo "Compilador: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo "OpenMP disponible: $(shell $(CXX) -fopenmp -dM -E - < /dev/null 2>/dev/null | grep -c _OPENMP)"
	@echo "Núcleos de CPU: $(shell nproc 2>/dev/null || echo 'N/A')"

# Ayuda
help:
	@echo "$(COLOR_BOLD)Mandelbrot HPC - Comandos disponibles:$(COLOR_RESET)"
	@echo ""
	@echo "  $(COLOR_GREEN)make$(COLOR_RESET)              - Compilar el proyecto"
	@echo "  $(COLOR_GREEN)make run$(COLOR_RESET)          - Ejecutar con parámetros por defecto"
	@echo "  $(COLOR_GREEN)make run-custom$(COLOR_RESET)   - Ejecutar con parámetros personalizados"
	@echo "                      Ejemplo: make run-custom WIDTH=800 HEIGHT=600"
	@echo "  $(COLOR_GREEN)make benchmark$(COLOR_RESET)    - Ejecutar benchmark completo con gráficas"
	@echo "  $(COLOR_GREEN)make clean$(COLOR_RESET)        - Limpiar archivos compilados"
	@echo "  $(COLOR_GREEN)make clean-all$(COLOR_RESET)    - Limpiar todo incluyendo resultados"
	@echo "  $(COLOR_GREEN)make info$(COLOR_RESET)         - Mostrar información del sistema"
	@echo "  $(COLOR_GREEN)make help$(COLOR_RESET)         - Mostrar esta ayuda"

.PHONY: all directories run run-custom benchmark clean clean-all info help
