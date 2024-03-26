#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <bitset>
#include <string>

using namespace std;

// Структура узла дерева Хаффмана
struct HuffmanNode {
    char data; // Символ
    int frequency; // Частота символа
    HuffmanNode *left, *right; // Левый и правый потомки

    HuffmanNode(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Сравнение для приоритетной очереди
struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->frequency > r->frequency;
    }
};

// Построение кодов Хаффмана
void buildCodes(HuffmanNode* root, string code, unordered_map<char, string>& codes) {
    if (!root)
        return;

    // Если узел - лист, добавляем его код
    if (!root->left && !root->right) {
        codes[root->data] = code;
    }

    // Рекурсивно строим коды для левого и правого поддеревьев
    buildCodes(root->left, code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

// Кодирование файла с использованием таблицы кодов Хаффмана
void encodeFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    if (!inFile.is_open()) {
        cerr << "Unable to open input file\n";
        return;
    }

    // Подсчитываем частоту каждого символа
    unordered_map<char, int> freq;
    char ch;
    while (inFile.get(ch)) {
        freq[ch]++;
    }
    inFile.close();

    // Построение дерева Хаффмана
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for (const auto& pair : freq) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }
    while (pq.size() != 1) {
        HuffmanNode *left = pq.top(); pq.pop();
        HuffmanNode *right = pq.top(); pq.pop();
        HuffmanNode *parent = new HuffmanNode('$', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    HuffmanNode *root = pq.top();

    // Построение таблицы кодов Хаффмана
    unordered_map<char, string> codes;
    buildCodes(root, "", codes);

    // Запись таблицы кодов в выходной файл
    ofstream outFile(outputFile, ios::binary);
    for (const auto& pair : codes) {
        outFile << pair.first << pair.second << '\n';
    }
    outFile << "___\n"; // Разделитель между таблицей кодов и закодированными данными

    // Кодирование данных и запись в выходной файл
    inFile.open(inputFile, ios::binary);
    string encodedString = "";
    while (inFile.get(ch)) {
        encodedString += codes[ch];
    }
    inFile.close();
    int padding = 8 - (encodedString.length() % 8);
    for (int i = 0; i < padding; ++i) {
        encodedString += '0'; // Дополнение нулями до полного байта
    }
    for (size_t i = 0; i < encodedString.length(); i += 8) {
        bitset<8> bits(encodedString.substr(i, 8));
        char c = static_cast<char>(bits.to_ulong());
        outFile << c;
    }
    outFile.close();

    // Освобождаем память
    delete root;
}

// Декодирование файла, используя таблицу кодов Хаффмана
void decodeFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    if (!inFile) {
        cerr << "Unable to open input file\n";
        return;
    }

    // Чтение таблицы кодов Хаффмана
    unordered_map<string, char> reverseCodes;
    string line;
    char ch;
    while (inFile.get(ch) && ch != '_') {
        inFile.get(ch); // Пропускаем '\n'
        getline(inFile, line); 
        reverseCodes[line] = ch;
    }

    // Чтение закодированных данных и декодирование
    ofstream outFile(outputFile, ios::binary);
    string code = "";
    while (inFile.get(ch)) {
        bitset<8> bits(ch);
        code += bits.to_string();
        for (size_t i = 0; i < code.length(); ) {
            string substr = code.substr(0, i + 1);
            if (reverseCodes.find(substr) != reverseCodes.end()) {
                outFile << reverseCodes[substr];
                code = code.substr(i + 1);
                i = 0;
            } else {
                ++i;
            }
        }
    }
    inFile.close();
    outFile.close();
}

int main(int argc, char *argv[]) {
    if (argc == 2)
    {
    const string inputFile = argv[0];
    const string encodedFile = "encoded.bin";
    const string decodedFile = "decoded.txt";

    // Кодирование файла
    encodeFile(inputFile, encodedFile);

    // Декодирование файла
    decodeFile(encodedFile, decodedFile);

    cout << "Encoding and decoding done successfully.\n";
    }
    return 0;
}
