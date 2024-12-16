#include "mandarSerial.h"
#include <iostream>
#include <stdexcept>
#include <windows.h>

int mandarSerial::isOpen() {
    return hCom != INVALID_HANDLE_VALUE;
}

using std::cout;
using std::cerr;
using std::endl;

mandarSerial::mandarSerial(char* portName, int baudRate) {
    COMMTIMEOUTS comto;

    hCom = CreateFileA(
        portName,                // Nome do arquivo
        GENERIC_READ | GENERIC_WRITE, // Abre para leitura e escrita
        0,                       // Sem compartilhamento
        NULL,                    // Par�metros de seguran�a padr�o
        OPEN_EXISTING,           // Abre apenas se o arquivo existir
        0,                       // Atributos padr�o
        NULL                     // Sem gabarito
    );

    if (hCom == INVALID_HANDLE_VALUE) { // Verifica erro ao abrir porta
        throw std::runtime_error("Falha ao abrir porta serial");
    }

    std::cout << "Porta aberta com sucesso!" << std::endl;

    // Configura��o de par�metros seriais
    GetCommState(hCom, &dcb);
    dcb.BaudRate = baudRate; // Configura taxa de transmiss�o
    dcb.ByteSize = 8;        // Tamanho do byte de dados
    dcb.StopBits = ONESTOPBIT; // Um bit de parada
    dcb.Parity = NOPARITY;   // Sem paridade

    if (!SetCommState(hCom, &dcb)) {
        std::cerr << "Erro ao configurar porta serial: " << GetLastError() << std::endl;
        throw std::runtime_error("Falha ao configurar porta serial");
    }

    // Configura��o de timeouts
    GetCommTimeouts(hCom, &comto);
    comto.ReadIntervalTimeout = 50;  // Intervalo entre os bytes (em ms)
    comto.ReadTotalTimeoutConstant = 500;  // Timeout total de leitura (em ms)
    comto.ReadTotalTimeoutMultiplier = 10; // Multiplicador de tempo de leitura
    comto.WriteTotalTimeoutConstant = 500; // Timeout de escrita (em ms)
    comto.WriteTotalTimeoutMultiplier = 10; // Multiplicador de escrita

    if (!SetCommTimeouts(hCom, &comto)) {
        std::cerr << "Erro ao configurar timeouts: " << GetLastError() << std::endl;
        throw std::runtime_error("Falha ao configurar timeouts");
    }

    std::cout << "Configuracao da porta serial concluida." << std::endl;
}

mandarSerial::~mandarSerial() {
    if (hCom != INVALID_HANDLE_VALUE) {
        CloseHandle(hCom);
        std::cout << "Porta serial fechada." << std::endl;
    }

}
void mandarSerial::sendByte(uint8_t byte) {

    DWORD bytesWritten;

    if (!WriteFile(hCom, &byte, 1, &bytesWritten, NULL)) {
        throw std::runtime_error("Erro ao enviar byte para a porta serial");
    }
    if (bytesWritten == 0) {
        throw std::runtime_error("Nenhum byte foi enviado para a porta serial");
    }
    std::cout << "Byte enviado: 0x" << std::hex << static_cast<int>(byte)<< std::endl;
}

uint8_t mandarSerial::receiveByte() {
    uint8_t byte;
    DWORD bytesRead = 0;

    std::cout << "tentando ler" << std::endl;

    while (bytesRead == 0) {

        if (ReadFile(hCom, &byte, 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                std::cout << "Byte recebido: 0x" << std::hex << static_cast<int>(byte) << std::endl;
            }
        } else {
            throw std::runtime_error("Falha ao ler da porta serial.");
        }
        Sleep(1);
    }


    return byte; // Retorna o byte lido
}

void mandarSerial::sendFrame(cubo& c, int i) {
    cout << "O CID e : " << i << endl;
    char cid = (char)0x80 | (char)i; // Define o CID (identificador do cubo)

    // Envia comandos iniciais

Repete_Manda:
    sendByte(SNC); // Envia comando de envio de imagem
    sendByte(cid); // Envia o CID


    // Aguarda a resposta MND
    uint8_t resposta = receiveByte();
    if (resposta != MND)
    {
        if(resposta == TOUT)
        {
            cerr << "Erro de timeout" << endl;
            goto Repete_Manda;
        }
        throw std::runtime_error("Falha ao receber MND");
    } else {
        std::cout << "Comando MND recebido com sucesso!" << std::endl;
    }

    // Envia os dados do frame

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            for (int z = 0; z < 8; z++) {
                sendByte(c.img[i][x][y][z]);
            }
        }
    }

    uint8_t resposta2 = receiveByte();
    if (resposta2 != ACK) {
        throw std::runtime_error("Falha ao enviar frame: ACK n�o recebido");
    }

std::cout << "Frame enviado com sucesso!" << std::endl;
}

