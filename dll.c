#include <Windows.h> #include #include

int main() { WIN32_FIND_DATA findData; HANDLE hFind;

// Abre o diret�rio atual para leitura
std::string dirPath = ".";
dirPath += "\\*.dll";
hFind = FindFirstFile(dirPath.c_str(), &findData);

if (hFind == INVALID_HANDLE_VALUE) {
    std::cerr << "Erro ao abrir diret�rio." << std::endl;
    return 1;
}

do {
    // Ignora os diret�rios '.' e '..'
    if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
        continue;
    }

    // Cria o nome do arquivo de sa�da com extens�o .txt
    std::string inputFile = findData.cFileName;
    std::string outputFile = inputFile.substr(0, inputFile.length() - 4) + ".txt";

    // Abre o arquivo de entrada
    std::string inputFilePath = ".\\" + inputFile;
    std::ifstream inFile(inputFilePath, std::ios::binary | std::ios::ate);

    if (inFile.fail()) {
        std::cerr << "Erro ao abrir arquivo " << inputFilePath << std::endl;
        continue;
    }

    // L� o tamanho do arquivo
    std::streamsize size = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    // Aloca mem�ria para ler o conte�do do arquivo
    char* buffer = new char[size];

    if (!buffer) {
        std::cerr << "Erro ao alocar mem�ria." << std::endl;
        inFile.close();
        continue;
    }

    // L� o conte�do do arquivo
    if (!inFile.read(buffer, size)) {
        std::cerr << "Erro ao ler arquivo " << inputFilePath << std::endl;
        inFile.close();
        delete[] buffer;
        continue;
    }

    // Abre o arquivo de sa�da
    std::string outputFilePath = ".\\" + outputFile;
    std::ofstream outFile(outputFilePath);

    if (outFile.fail()) {
        std::cerr << "Erro ao criar arquivo " << outputFilePath << std::endl;
        inFile.close();
        delete[] buffer;
        continue;
    }

    // Escreve o conte�do do arquivo no arquivo de sa�da
    outFile.write(buffer, size);

    // Fecha os arquivos de entrada e sa�da
    inFile.close();
    outFile.close();

    // Deleta o arquivo de entrada
    if (remove(inputFilePath.c_str()) != 0) {
        std::cerr << "Erro ao deletar arquivo " << inputFilePath << std::endl;
    } else {
        std::cout << "Arquivo " << inputFilePath << " deletado." << std::endl;
    }

    // Libera a mem�ria alocada
    delete[] buffer;

} while (FindNextFile(hFind, &findData) != 0);

// Fecha a busca
FindClose(hFind);

return 0;
}
