#ifndef MANDARSERIAL_H
#define MANDARSERIAL_H

#include "cubo.h"   // Inclui o cabeçalho da classe cubo
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <bitset>

#define SNC ((uint8_t)0xD3)
#define MND ((uint8_t)0x4D)
#define ACK ((uint8_t)0x41)
#define TOUT ('T')
#define ATX ((uint8_t)0xD8)


// Tamanho das faces (8x8)

class mandarSerial {
private:
    HANDLE hCom;              // Handler para a comunicação serial
    DCB dcb;                  // Estrutura de configuração de comunicação
    COMMTIMEOUTS timeouts;    // Estrutura de timeouts para comunicação serial

public:
    // Construtor: recebe o nome da porta serial e a taxa de transmissão (baud rate)
    mandarSerial(char* portName = "", int baudRate = 0);

    // Destruidor: fecha o handle da porta serial
    virtual ~mandarSerial();

    // Função para verificar se a porta serial está aberta
    int isOpen();

    // Função para enviar um byte pela porta serial
    void sendByte(uint8_t byte);

    // Função para receber um byte pela porta serial
    uint8_t receiveByte();

    // Função para enviar o frame para o cubo
    void sendFrame(cubo& c, int i);

    //void enviar_cubo(uin8_t *dados, int cid);
    //void ativa_cubo(int ind);
    //void apagar_cubo(int ind);
    //void apagar_cubo();
};

#endif