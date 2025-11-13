#!/usr/bin/env python3
"""
Script para generar gráficas de análisis de rendimiento del Mandelbrot HPC
Genera visualizaciones de speedup, eficiencia y tiempo de ejecución
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

def load_results(filename='benchmarks/results.csv'):
    """Carga los resultados del benchmark desde CSV"""
    if not os.path.exists(filename):
        print(f"Error: No se encontró el archivo {filename}")
        return None
    return pd.read_csv(filename)

def plot_speedup(df, output_file='benchmarks/speedup.png'):
    """Genera gráfica de speedup vs número de hilos"""
    plt.figure(figsize=(10, 6))
    
    # Speedup real
    plt.plot(df['threads'], df['speedup'], 'o-', linewidth=2, 
             markersize=8, label='Speedup Real', color='#2E86AB')
    
    # Speedup ideal (lineal)
    ideal_speedup = df['threads']
    plt.plot(df['threads'], ideal_speedup, '--', linewidth=2, 
             label='Speedup Ideal (Lineal)', color='#A23B72', alpha=0.7)
    
    plt.xlabel('Número de Hilos', fontsize=12, fontweight='bold')
    plt.ylabel('Speedup', fontsize=12, fontweight='bold')
    plt.title('Análisis de Speedup - Conjunto de Mandelbrot', 
              fontsize=14, fontweight='bold', pad=20)
    plt.legend(fontsize=10, loc='upper left')
    plt.grid(True, alpha=0.3, linestyle='--')
    plt.tight_layout()
    
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"✓ Gráfica de speedup guardada en: {output_file}")
    plt.close()

def plot_execution_time(df, output_file='benchmarks/execution_time.png'):
    """Genera gráfica de tiempo de ejecución vs número de hilos"""
    plt.figure(figsize=(10, 6))
    
    plt.plot(df['threads'], df['time'], 'o-', linewidth=2, 
             markersize=8, color='#F18F01')
    
    plt.xlabel('Número de Hilos', fontsize=12, fontweight='bold')
    plt.ylabel('Tiempo de Ejecución (segundos)', fontsize=12, fontweight='bold')
    plt.title('Tiempo de Ejecución vs Número de Hilos', 
              fontsize=14, fontweight='bold', pad=20)
    plt.grid(True, alpha=0.3, linestyle='--')
    plt.tight_layout()
    
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"✓ Gráfica de tiempo de ejecución guardada en: {output_file}")
    plt.close()

def plot_efficiency(df, output_file='benchmarks/efficiency.png'):
    """Genera gráfica de eficiencia vs número de hilos"""
    plt.figure(figsize=(10, 6))
    
    plt.plot(df['threads'], df['efficiency'] * 100, 'o-', linewidth=2, 
             markersize=8, color='#06A77D')
    
    # Línea de referencia al 100%
    plt.axhline(y=100, color='#A23B72', linestyle='--', 
                linewidth=2, alpha=0.7, label='Eficiencia Ideal (100%)')
    
    plt.xlabel('Número de Hilos', fontsize=12, fontweight='bold')
    plt.ylabel('Eficiencia (%)', fontsize=12, fontweight='bold')
    plt.title('Eficiencia de Paralelización', 
              fontsize=14, fontweight='bold', pad=20)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3, linestyle='--')
    plt.ylim(0, 110)
    plt.tight_layout()
    
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"✓ Gráfica de eficiencia guardada en: {output_file}")
    plt.close()

def plot_combined(df, output_file='benchmarks/combined_analysis.png'):
    """Genera una figura combinada con todas las métricas"""
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle('Análisis Completo de Rendimiento - Mandelbrot HPC', 
                 fontsize=16, fontweight='bold', y=0.995)
    
    # Speedup
    ax1 = axes[0, 0]
    ax1.plot(df['threads'], df['speedup'], 'o-', linewidth=2, 
             markersize=8, label='Speedup Real', color='#2E86AB')
    ax1.plot(df['threads'], df['threads'], '--', linewidth=2, 
             label='Speedup Ideal', color='#A23B72', alpha=0.7)
    ax1.set_xlabel('Número de Hilos', fontweight='bold')
    ax1.set_ylabel('Speedup', fontweight='bold')
    ax1.set_title('Speedup', fontweight='bold')
    ax1.legend()
    ax1.grid(True, alpha=0.3, linestyle='--')
    
    # Tiempo de ejecución
    ax2 = axes[0, 1]
    ax2.plot(df['threads'], df['time'], 'o-', linewidth=2, 
             markersize=8, color='#F18F01')
    ax2.set_xlabel('Número de Hilos', fontweight='bold')
    ax2.set_ylabel('Tiempo (s)', fontweight='bold')
    ax2.set_title('Tiempo de Ejecución', fontweight='bold')
    ax2.grid(True, alpha=0.3, linestyle='--')
    
    # Eficiencia
    ax3 = axes[1, 0]
    ax3.plot(df['threads'], df['efficiency'] * 100, 'o-', linewidth=2, 
             markersize=8, color='#06A77D')
    ax3.axhline(y=100, color='#A23B72', linestyle='--', 
                linewidth=2, alpha=0.7, label='Ideal')
    ax3.set_xlabel('Número de Hilos', fontweight='bold')
    ax3.set_ylabel('Eficiencia (%)', fontweight='bold')
    ax3.set_title('Eficiencia', fontweight='bold')
    ax3.legend()
    ax3.grid(True, alpha=0.3, linestyle='--')
    ax3.set_ylim(0, 110)
    
    # Tabla de resultados
    ax4 = axes[1, 1]
    ax4.axis('tight')
    ax4.axis('off')
    
    table_data = []
    for _, row in df.iterrows():
        table_data.append([
            f"{int(row['threads'])}",
            f"{row['time']:.3f}s",
            f"{row['speedup']:.2f}x",
            f"{row['efficiency']*100:.1f}%"
        ])
    
    table = ax4.table(cellText=table_data,
                      colLabels=['Hilos', 'Tiempo', 'Speedup', 'Eficiencia'],
                      cellLoc='center',
                      loc='center',
                      colWidths=[0.2, 0.25, 0.25, 0.3])
    table.auto_set_font_size(False)
    table.set_fontsize(9)
    table.scale(1, 2)
    
    # Estilo de la tabla
    for i in range(len(table_data) + 1):
        for j in range(4):
            cell = table[(i, j)]
            if i == 0:
                cell.set_facecolor('#2E86AB')
                cell.set_text_props(weight='bold', color='white')
            else:
                cell.set_facecolor('#f0f0f0' if i % 2 == 0 else 'white')
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"✓ Gráfica combinada guardada en: {output_file}")
    plt.close()

def print_summary(df):
    """Imprime un resumen estadístico de los resultados"""
    print("\n" + "="*60)
    print("RESUMEN DE RESULTADOS")
    print("="*60)
    
    max_threads = df['threads'].max()
    max_speedup = df['speedup'].max()
    max_speedup_threads = df.loc[df['speedup'].idxmax(), 'threads']
    
    serial_time = df.loc[df['threads'] == 1, 'time'].values[0]
    best_parallel_time = df['time'].min()
    
    print(f"\nTiempo serial (1 hilo):          {serial_time:.3f} segundos")
    print(f"Mejor tiempo paralelo:           {best_parallel_time:.3f} segundos")
    print(f"Máximo speedup alcanzado:        {max_speedup:.2f}x (con {int(max_speedup_threads)} hilos)")
    print(f"Número máximo de hilos probados: {int(max_threads)}")
    
    # Calcular eficiencia promedio
    avg_efficiency = df['efficiency'].mean() * 100
    print(f"Eficiencia promedio:             {avg_efficiency:.1f}%")
    
    print("\n" + "="*60 + "\n")

def main():
    """Función principal"""
    print("\n" + "="*60)
    print("GENERANDO GRÁFICAS DE ANÁLISIS DE RENDIMIENTO")
    print("="*60 + "\n")
    
    # Cargar resultados
    df = load_results()
    if df is None:
        return
    
    # Generar gráficas individuales
    plot_speedup(df)
    plot_execution_time(df)
    plot_efficiency(df)
    
    # Generar gráfica combinada
    plot_combined(df)
    
    # Imprimir resumen
    print_summary(df)
    
    print("✓ Todas las gráficas generadas exitosamente\n")

if __name__ == '__main__':
    main()
