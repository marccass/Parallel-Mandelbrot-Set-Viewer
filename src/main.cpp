#include "mandelbrot.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <omp.h>

void print_system_info() {
    std::cout << "=== Información del Sistema ===" << std::endl;
    std::cout << "Número máximo de hilos disponibles: " << omp_get_max_threads() << std::endl;
    std::cout << "================================" << std::endl << std::endl;
}

void run_benchmark(MandelbrotGenerator& generator, int max_threads) {
    std::cout << "=== Benchmark: Conjunto de Mandelbrot ===" << std::endl;
    std::cout << "Resolución: " << generator.get_width() << "x" << generator.get_height() << std::endl;
    std::cout << std::endl;
    
    // Versión serial
    std::cout << "Ejecutando versión SERIAL..." << std::endl;
    double time_serial = generator.generate_serial();
    std::cout << "Tiempo: " << std::fixed << std::setprecision(3) << time_serial << " segundos" << std::endl;
    generator.save_image("results/mandelbrot_serial.ppm");
    std::cout << std::endl;
    
    // Guardar resultados para análisis
    std::ofstream results_file("benchmarks/results.csv");
    results_file << "threads,time,speedup,efficiency\n";
    results_file << "1," << time_serial << ",1.0,1.0\n";
    
    // Versiones paralelas con diferentes números de hilos
    std::cout << "Ejecutando versiones PARALELAS..." << std::endl;
    std::cout << std::setw(10) << "Hilos" 
              << std::setw(15) << "Tiempo (s)" 
              << std::setw(15) << "Speedup" 
              << std::setw(15) << "Eficiencia" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    for (int threads = 2; threads <= max_threads; threads++) {
        double time_parallel = generator.generate_parallel(threads);
        double speedup = time_serial / time_parallel;
        double efficiency = speedup / threads;
        
        std::cout << std::setw(10) << threads
                  << std::setw(15) << std::fixed << std::setprecision(3) << time_parallel
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup
                  << std::setw(15) << std::fixed << std::setprecision(2) << efficiency
                  << std::endl;
        
        results_file << threads << "," << time_parallel << "," << speedup << "," << efficiency << "\n";
    }
    
    results_file.close();
    
    // Guardar la última imagen paralela
    generator.save_image("results/mandelbrot_parallel.ppm");
    
    std::cout << std::endl;
    std::cout << "Resultados guardados en: benchmarks/results.csv" << std::endl;
    std::cout << "==========================================" << std::endl;
}

int main(int argc, char* argv[]) {
    // Parámetros por defecto
    int width = 1920;
    int height = 1080;
    int max_iterations = 1000;
    int max_threads = omp_get_max_threads();
    
    // Parsear argumentos de línea de comandos
    if (argc > 1) {
        width = std::atoi(argv[1]);
    }
    if (argc > 2) {
        height = std::atoi(argv[2]);
    }
    if (argc > 3) {
        max_iterations = std::atoi(argv[3]);
    }
    if (argc > 4) {
        max_threads = std::atoi(argv[4]);
    }
    
    print_system_info();
    
    // Crear generador
    MandelbrotGenerator generator(width, height, max_iterations);
    
    // Configurar región del plano complejo (vista clásica del conjunto)
    generator.set_region(-2.5, 1.0, -1.0, 1.0);
    
    // Ejecutar benchmark
    run_benchmark(generator, max_threads);
    
    return 0;
}
