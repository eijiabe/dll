#include <Windows.h>
#include <iostream>
#include <fstream>

int main() {
    WIN32_FIND_DATA findData;
    HANDLE hFind;

    // Abre o diretório atual para leitura
    std::string dirPath = ".\\*.dll";
    hFind = FindFirstFile(dirPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Erro ao abrir diretório." << std::endl;
        return 1;
    }

    do {
        // Ignora os diretórios '.' e '..'
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
            continue;
        }

        // Cria o nome do arquivo de saída com extensão .txt
        std::string inputFile = findData.cFileName;
        std::string outputFile = inputFile.substr(0, inputFile.length() - 4) + ".txt";

        // Abre o arquivo de entrada
        std::string inputFilePath = ".\\" + inputFile;
        std::ifstream inFile(inputFilePath, std::ios::binary | std::ios::ate);

        if (!inFile) {
            std::cerr << "Erro ao abrir arquivo " << inputFilePath << std::endl;
            continue;
        }

        // Lê o tamanho do arquivo
        std::streamsize size = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        // Aloca memória para ler o conteúdo do arquivo
        char* buffer = new char[size];

        if (!buffer) {
            std::cerr << "Erro ao alocar memória." << std::endl;
            inFile.close();
            continue;
        }

        // Lê o conteúdo do arquivo
        if (!inFile.read(buffer, size)) {
            std::cerr << "Erro ao ler arquivo " << inputFilePath << std::endl;
            inFile.close();
            delete[] buffer;
            continue;
        }

        // Abre o arquivo de saída
        std::string outputFilePath = ".\\" + outputFile;
        std::ofstream outFile(outputFilePath);

        if (!outFile) {
            std::cerr << "Erro ao criar arquivo " << outputFilePath << std::endl;
            inFile.close();
            delete[] buffer;
            continue;
        }

        // Escreve o conteúdo do arquivo no arquivo de saída
        outFile.write(buffer, size);

        // Fecha os arquivos de entrada e saída
        inFile.close();
        outFile.close();

        // Deleta o arquivo de entrada
        if (DeleteFile(inputFilePath.c_str()) == 0) {
            std::cerr << "Erro ao deletar arquivo " << inputFilePath << std::endl;
        } else {
            std::cout << "Arquivo " << inputFilePath << " deletado." << std::endl;
        }

        // Libera a memória alocada
        delete[] buffer;

    } while (FindNextFile(hFind, &findData) != 0);

    // Fecha a busca
    FindClose(hFind);

    return 0;
}
