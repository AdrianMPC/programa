#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <cstdlib>

std::mutex mutex_controlador;

double elemRam() {
    std::mt19937 generador(std::random_device{}());
    std::uniform_real_distribution<double> distribucion(1.0, 100.0);

    return distribucion(generador);
}

void productoPuntoParcial(const std::vector<double>& vec1, const std::vector<double>& vec2, int inicio, int fin, double& proDot) {
    double parcial = 0.0;
    for (int i = inicio; i < fin; ++i) {
        parcial += vec1[i] * vec2[i];
    }
    std::lock_guard<std::mutex> lock(mutex_controlador);
    proDot += parcial;
}

double productoPuntoVectores(const std::vector<double>& vec1, const std::vector<double>& vec2, int num_threads) {
    double proDot = 0.0;
    std::vector<std::thread> threads;
    int tam = vec1.size();
    int paso = tam / num_threads;
    
    // dividiendo los datos entre los threads
    //  v1 = **** **** **** **** || v2= **** **** **** ****
    // entonces pasamos referencias constantes multiplicar v1.* x v2.* para luego sumar ese resultado en mutex
    for (int i = 0; i < num_threads; ++i) {
        int inicio = i * paso;
        int fin = (i == num_threads - 1) ? tam : inicio + paso;  // manejar cualquier posible redondeo
        threads.emplace_back(productoPuntoParcial, std::cref(vec1), std::cref(vec2), inicio, fin, std::ref(proDot));
    }

    for (std::thread& thread : threads) {
        thread.join();
    }

    
    return proDot;
}

int main(int argc, char *argv[]){

    if (argc != 3) {
        std::cerr << "USO: " << argv[0] << " <arg1> <arg2>" << std::endl;
        return 1;
    }

    int num_threads = atoi(argv[1]);;
    int tam = atoi(argv[argc-1]);

    std::vector<double> vector1(tam);
    std::vector<double> vector2(tam);

    for (int i = 0; i < tam; i++){
        double elemento = elemRam();
        vector1[i] = elemento;
    }
    std::cout<<"Primer vector insertado" << std::endl;
    for (int i = 0; i < tam; i++){
        double elemento = elemRam();
        vector2[i] = elemento;
    }
    std::cout<<"Segundo vector insertado" << std::endl;
    auto inicio = std::chrono::high_resolution_clock::now();
    std::cout << "Producto punto de los 2 vectores: " << productoPuntoVectores(vector1, vector2, num_threads) << std::endl;
    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion = fin - inicio;
    std::cout << "Tiempo de ejecucion: " << duracion.count() << " segundos" << std::endl;
    return 0;
}