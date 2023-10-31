#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bitset>
#include <locale.h>
#include <limits>
#include <tuple>

using namespace std;

int ObterValorLinha(int bits) {
    setlocale(LC_ALL, "");
    string filename = "";
    if (bits == 16)
        filename = "addresses_16b.txt";
    else if (bits == 32)
        filename = "addresses_32b.txt";

    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cout << "Erro ao abrir o arquivo de entrada." << endl;
        return 0;
    }

    // Ler o arquivo e armazenar os números em um vetor
    vector<int> numbers;
    int number;
    while (inputFile >> number) {
        numbers.push_back(number);
    }
    inputFile.close();

    if (numbers.empty()) {
        cout << "O arquivo este vazio ou nao contem numeros validos." << endl;
        return 0;
    }

    // Solicitar ao usuário o índice da linha a ser lida
    int lineIndex;
    cout << "Digite o indice da linha a ser lida: ";
    cin >> lineIndex;

    if (lineIndex < 0 || lineIndex >= numbers.size()) {
        cout << "Índice invalido. Certifique-se de escolher um numero entre 0 e " << numbers.size() - 1 << endl;
        return 0;
    }

    // Converter o número escolhido em binário
    return numbers[lineIndex];
}

int CalcularPagina(int bits, string binario) {
    int pageNumber = 0;      // Converter binário para decimal

    if (bits == 16) {
        // Calcular o número da página
        string pageBinary = binario.substr(0, 8);  // 4 primeiros caracteres para o número da página
        pageNumber = bitset<8>(pageBinary).to_ulong();      // Converter binário para decimal
    }
    else if (bits == 32) {
        // Calcular o número da página
        string pageBinary = binario.substr(0, 20);  // 20 primeiros caracteres para o número da página
        pageNumber = bitset<20>(pageBinary).to_ulong();      // Converter binário para decimal
    }
    return pageNumber;
}

int CalcularOffset(int bits, string binario) {
    int offset = 0;

    if (bits == 16) {
        // Calcular o deslocamento
        string offsetBinary = binario.substr(8);    // Restante para o deslocamento
        offset = bitset<8>(offsetBinary).to_ulong();      // Converter binário para decimal
    }
    else if (bits == 32) {
        // Calcular o deslocamento
        string offsetBinary = binario.substr(20);    // Restante para o deslocamento
        offset = bitset<12>(offsetBinary).to_ulong();      // Converter binário para decimal
    }
    return offset;
}

int main() {

    // Solicitar ao usuário se deseja efetuar a busca em 16 ou 32 bits
    int bits;
    cout << "Deseja efetuar a busca em '16' ou '32' bits? ";
    cin >> bits;

    int chosenNumber = ObterValorLinha(bits);

    string binaryRepresentation;
    if (bits == 16)
        binaryRepresentation = bitset<16>(chosenNumber).to_string();
    else if (bits == 32)
        binaryRepresentation = bitset<32>(chosenNumber).to_string();

    int pageNumber = CalcularPagina(bits, binaryRepresentation);
    int offset = CalcularOffset(bits, binaryRepresentation);

    // Imprimir o resultado
    cout << "O endereco " << chosenNumber << " contem:" << endl;
    cout << "Numero da pagina = " << pageNumber << endl;
    cout << "Deslocamento = " << offset << endl;
    cout << "Representacao binaria: " << binaryRepresentation << endl;

    // Agora, vamos ler o arquivo "data_memory.txt" para encontrar o valor correspondente
    ifstream dataMemoryFile("data_memory.txt");
    if (!dataMemoryFile.is_open()) {
        cout << "Erro ao abrir o arquivo data_memory.txt." << endl;
        return 1;
    }

    int value;
    int pageSize; // Tamanho da página

    if (bits == 16)
        pageSize = 256;
    else if (bits == 32)
        pageSize = 4096;

    int pageStartLine = pageNumber * pageSize;
    int targetLine = pageStartLine + offset; // Calcule a linha alvo

    vector<string> lines;
    string line;

    while (getline(dataMemoryFile, line)) {
        lines.push_back(line);
    }
    cout << "Valor lido: " << lines[targetLine - 1] << endl;

    dataMemoryFile.close();

    return 0;
}
