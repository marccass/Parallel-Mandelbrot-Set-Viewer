#include "mandelbrot.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cmath>
#include <omp.h>

MandelbrotGenerator::MandelbrotGenerator(int w, int h, int max_iter)
    : width(w), height(h), max_iterations(max_iter),
      x_min(-2.5), x_max(1.0), y_min(-1.0), y_max(1.0) {
    data.resize(width * height);
}

void MandelbrotGenerator::set_region(double xmin, double xmax, double ymin, double ymax) {
    x_min = xmin;
    x_max = xmax;
    y_min = ymin;
    y_max = ymax;
}

int MandelbrotGenerator::compute_mandelbrot(std::complex<double> c) const {
    std::complex<double> z(0.0, 0.0);
    int iterations = 0;
    
    // Iteración: z = z² + c
    while (std::abs(z) <= 2.0 && iterations < max_iterations) {
        z = z * z + c;
        iterations++;
    }
    
    return iterations;
}

double MandelbrotGenerator::generate_serial() {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Calcular el paso en cada dirección
    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;
    
    // Iterar sobre cada píxel
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Mapear el píxel al plano complejo
            double real = x_min + col * dx;
            double imag = y_min + row * dy;
            std::complex<double> c(real, imag);
            
            // Calcular el número de iteraciones
            int iterations = compute_mandelbrot(c);
            data[row * width + col] = iterations;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    return elapsed.count();
}

double MandelbrotGenerator::generate_parallel(int num_threads) {
    omp_set_num_threads(num_threads);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Calcular el paso en cada dirección
    double dx = (x_max - x_min) / width;
    double dy = (y_max - y_min) / height;
    
    // Paralelizar el bucle exterior con OpenMP
    #pragma omp parallel for schedule(dynamic)
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            // Mapear el píxel al plano complejo
            double real = x_min + col * dx;
            double imag = y_min + row * dy;
            std::complex<double> c(real, imag);
            
            // Calcular el número de iteraciones
            int iterations = compute_mandelbrot(c);
            data[row * width + col] = iterations;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    return elapsed.count();
}

void MandelbrotGenerator::save_image(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return;
    }
    
    // Escribir cabecera PPM (formato P3 - ASCII)
    file << "P3\n";
    file << width << " " << height << "\n";
    file << "255\n";
    
    // Escribir los datos de la imagen
    for (int i = 0; i < width * height; i++) {
        int iterations = data[i];
        
        // Mapear iteraciones a colores (esquema de color simple)
        int r, g, b;
        if (iterations == max_iterations) {
            // Puntos en el conjunto: negro
            r = g = b = 0;
        } else {
            // Puntos fuera del conjunto: gradiente de color
            double t = static_cast<double>(iterations) / max_iterations;
            r = static_cast<int>(9 * (1 - t) * t * t * t * 255);
            g = static_cast<int>(15 * (1 - t) * (1 - t) * t * t * 255);
            b = static_cast<int>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
        }
        
        file << r << " " << g << " " << b << " ";
        
        // Nueva línea cada ciertos píxeles para mejor legibilidad
        if ((i + 1) % width == 0) {
            file << "\n";
        }
    }
    
    file.close();
    std::cout << "Imagen guardada en: " << filename << std::endl;
}
