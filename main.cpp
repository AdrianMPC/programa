#include "./classes/dotproduct.h"

int main(int argc, char *argv[]){

    if(argc < 2 && argc > 3){
        std::cout << "Uso <arg>";
    }
    int tam = std::stoi(argv[1]);
    dot_product dp(tam);
    dp.productoPuntoSecPar();
    return 0;
}