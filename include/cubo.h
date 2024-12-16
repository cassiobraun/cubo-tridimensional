#ifndef CUBO_H
#define CUBO_H
#include <cstdint>  // Necessário para uint8_t
#include <iostream>

class cubo
{
    public:
        cubo();
        virtual ~cubo();

        int limparCubo();

        void passarFaceParaCamadas(uint8_t face[8][8], int tam=0) ;

        uint8_t img [32][8][8][8];

        uint8_t ind=32;

    protected:

    private:

};

#endif // CUBO_H