#include "dotproduct.h"

double dot_product::elemRam() {
    std::mt19937 generador(std::random_device{}());
    std::uniform_real_distribution<double> distribucion(1.0, 10.0);

    return distribucion(generador);
}

double dot_product::retornarTiempo(std::chrono::duration<double>& duracion){
    return duracion.count();
}

void dot_product::productoPuntoParcial(int inicio, int fin, double& proDot) {
    double parcial = 0.0;
    for (int i = inicio; i < fin; ++i) {
        parcial += vector1[i] * vector2[i];
    }
    std::lock_guard<std::mutex> lock(mutex_controlador);
    proDot += parcial;
}

double dot_product::productoPuntoVectores(int num_threads) {
    double proDot = 0.0;
    std::vector<std::thread> threads;
    int tam = vector1.size();
    int paso = tam / num_threads;
    
    // dividiendo los datos entre los threads
    //  v1 = **** **** **** **** || v2= **** **** **** ****
    // entonces pasamos referencias constantes multiplicar v1.* x v2.* para luego sumar ese resultado en mutex
    auto inicio = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_threads; ++i) {
        int inicio = i * paso;
        int fin = (i == num_threads - 1) ? tam : inicio + paso;  // manejar cualquier posible redondeo
        threads.emplace_back([this, inicio, fin, &proDot]() {
            productoPuntoParcial(inicio, fin, proDot);
        });
    }

    for (std::thread& thread : threads) {
        thread.join();
    }
    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion = fin - inicio;
    std::cout << "[dot_product] Tiempo de ejecucion PARALELO con " << num_threads << " cores: " << retornarTiempo(duracion) << " segundos" << std::endl;
    threads.clear();
    return proDot;
}

double dot_product::productoPuntoSecuencial() {
    double resultado = 0.0;
    auto inicio = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < vector1.size(); ++i) {
        resultado += vector1[i] * vector2[i];
    }
    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion = fin - inicio;
    std::cout << "[dot_product] Tiempo de ejecucion SECUENCIAL: " << retornarTiempo(duracion) << " segundos" << std::endl;
    return resultado;
}

// se que no cumple con responsabilidad unica pero que mas da...
void dot_product::inicializacionProducto(){
    std::cout<<"[dot_product] Iniciando llenado de vectores..."<< std::endl;
    std::cout<<"[dot_product] Llenando vector 1 "; std::cout<<"con " << tam_vector << " datos" << std::endl;

    for (int i = 0; i < tam_vector; i++){
        double elem = elemRam();
        vector1[i] = elem;
    }

    std::cout<<"[dot_product] Primer vector llenado!" << std::endl;
    std::cout<<"[dot_product] Llenando vector 2 "; std::cout<<"con " << tam_vector << " datos" << std::endl;

    for (int i = 0; i < tam_vector; i++){
        double elem = elemRam();
        vector2[i] = elem;
    }
    std::cout<<"[dot_product] Segundo vector llenado!" << std::endl;   
}

void dot_product::productoPuntoSecPar(){
    std::cout<<"[dot_product] Calculando producto punto secuencialmente" << std::endl;
    std::cout<<"[dot_product] Producto punto secuencial"<< productoPuntoSecuencial() << std::endl;
    
    for(int c_count : core_count){
        std::cout<<"[dot_product] Producto punto paralelo con "<< c_count << " hilos" << std::endl;
        productoPuntoVectores(c_count);
    }
}

dot_product::dot_product(int tam_vector){
    this->tam_vector = tam_vector;
    vector1.resize(tam_vector);
    vector2.resize(tam_vector);
    inicializacionProducto();
}