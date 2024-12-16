#include "cubo.h"

cubo::cubo()
{
    //ctor
}

cubo::~cubo()
{
    //dtor
}

// Função para limpar o cubo
int cubo::limparCubo() {
    for(int i =0; i<32; i++){
    for (int x = 0; x < 8; x++){
        for (int y = 0; y < 8; y++){
            for (int z = 0; z < 8; z++){
                img[i][x][y][z] = 0;
            }
        }
    }
}
return 0;
}

void cubo::passarFaceParaCamadas(uint8_t face[8][8], int tam) {

    if (tam >= 32) {
        std::cerr << "Índice de camada inválido!" << std::endl;
        return;
    }

    for (int x = 0; x <8; x++) {
        for (int y = 0; y < 8; y++) {
            for (int z = 0; z < 8; z++) {
                img[tam][x][y][z] = face[x][y];  // Atribui os valores da face à camada
            }
        }
    }
}
