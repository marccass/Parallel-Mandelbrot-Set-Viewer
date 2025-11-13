#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <complex>
#include <vector>
#include <string>

/**
 * @brief Clase para generar el Conjunto de Mandelbrot
 * 
 * Esta clase implementa tanto la versión serial como paralela
 * del algoritmo de generación del Conjunto de Mandelbrot.
 */
class MandelbrotGenerator {
private:
    int width;           // Ancho de la imagen en píxeles
    int height;          // Alto de la imagen en píxeles
    int max_iterations;  // Número máximo de iteraciones
    double x_min, x_max; // Rango en el eje X (parte real)
    double y_min, y_max; // Rango en el eje Y (parte imaginaria)
    
    std::vector<int> data; // Almacena los valores de iteración para cada píxel

    /**
     * @brief Calcula el número de iteraciones para un punto complejo
     * @param c Número complejo a evaluar
     * @return Número de iteraciones antes de divergir
     */
    int compute_mandelbrot(std::complex<double> c) const;

public:
    /**
     * @brief Constructor con parámetros por defecto
     */
    MandelbrotGenerator(int w = 1920, int h = 1080, int max_iter = 1000);

    /**
     * @brief Establece la región del plano complejo a visualizar
     */
    void set_region(double xmin, double xmax, double ymin, double ymax);

    /**
     * @brief Genera el conjunto de Mandelbrot de forma serial
     * @return Tiempo de ejecución en segundos
     */
    double generate_serial();

    /**
     * @brief Genera el conjunto de Mandelbrot de forma paralela con OpenMP
     * @param num_threads Número de hilos a utilizar
     * @return Tiempo de ejecución en segundos
     */
    double generate_parallel(int num_threads);

    /**
     * @brief Guarda la imagen en formato PPM
     * @param filename Nombre del archivo de salida
     */
    void save_image(const std::string& filename) const;

    /**
     * @brief Obtiene el ancho de la imagen
     */
    int get_width() const { return width; }

    /**
     * @brief Obtiene el alto de la imagen
     */
    int get_height() const { return height; }
};

#endif // MANDELBROT_H
