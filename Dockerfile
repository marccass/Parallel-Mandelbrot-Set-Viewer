# Dockerfile para Mandelbrot HPC
# Este contenedor proporciona un entorno reproducible para compilar y ejecutar el proyecto

FROM ubuntu:22.04

# Evitar prompts interactivos durante la instalación
ENV DEBIAN_FRONTEND=noninteractive

# Información del mantenedor
LABEL maintainer="HPC Developer"
LABEL description="Mandelbrot Set Visualizer with OpenMP parallelization"
LABEL version="1.0"

# Actualizar sistema e instalar dependencias
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    make \
    python3 \
    python3-pip \
    python3-matplotlib \
    python3-numpy \
    python3-pandas \
    && rm -rf /var/lib/apt/lists/*

# Crear directorio de trabajo
WORKDIR /app

# Copiar archivos del proyecto
COPY include/ /app/include/
COPY src/ /app/src/
COPY benchmarks/ /app/benchmarks/
COPY Makefile /app/

# Crear directorios necesarios
RUN mkdir -p /app/results /app/obj /app/bin

# Compilar el proyecto
RUN make

# Comando por defecto: ejecutar el benchmark
CMD ["make", "benchmark"]

# Notas de uso:
# 
# Construir la imagen:
#   docker build -t mandelbrot-hpc .
#
# Ejecutar el contenedor:
#   docker run --rm -v $(pwd)/results:/app/results -v $(pwd)/benchmarks:/app/benchmarks mandelbrot-hpc
#
# Ejecutar con parámetros personalizados:
#   docker run --rm -v $(pwd)/results:/app/results mandelbrot-hpc ./bin/mandelbrot 800 600 500
#
# Ejecutar de forma interactiva:
#   docker run --rm -it -v $(pwd)/results:/app/results mandelbrot-hpc /bin/bash
