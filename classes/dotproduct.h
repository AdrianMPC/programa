#ifndef DOT_PRODUCT_H
#define DOT_PRODUCT_H

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <cstdlib>

class dot_product{
    private:
        int tam_vector;
        std::vector<double> vector1;
        std::vector<double> vector2;
        std::vector<int> core_count = {2,4,8,16,32,64,128,256};
        std::mutex mutex_controlador;

        double elemRam();
        double retornarTiempo(std::chrono::duration<double>& duracion);
        void productoPuntoParcial(int inicio, int fin, double& proDot);
        double productoPuntoVectores(int num_threads);
        double productoPuntoSecuencial();
        void inicializacionProducto();
    public:
        dot_product(int tam_vector); 
        void productoPuntoSecPar();
};

#endif 