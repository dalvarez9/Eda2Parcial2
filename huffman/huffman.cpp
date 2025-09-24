#include <bits/stdc++.h>
using namespace std;

// Nodo del árbol de Huffman
struct Node {
    char simbolo;       // caracter (solo para hojas)
    int freq;           // frecuencia
    Node *izq, *der;
    Node(char s, int f) : simbolo(s), freq(f), izq(nullptr), der(nullptr) {}
    Node(Node* l, Node* r) : simbolo('\0'), freq(l->freq + r->freq), izq(l), der(r) {}
};

// Comparador para cola de prioridad mínima
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Construcción del árbol de Huffman
Node* construirHuffman(const unordered_map<char,int> &freqs) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto &kv : freqs) {
        pq.push(new Node(kv.first, kv.second));
    }
    while (pq.size() > 1) {
        Node* l = pq.top(); pq.pop();
        Node* r = pq.top(); pq.pop();
        pq.push(new Node(l, r));
    }
    return pq.top();
}

// Recorrer árbol y generar códigos
void generarCodigos(Node* root, string prefijo,
                    unordered_map<char,string> &codigos,
                    unordered_map<char,int> &longitudes) {
    if (!root) return;
    if (!root->izq && !root->der) {
        codigos[root->simbolo] = prefijo;
        longitudes[root->simbolo] = prefijo.size();
        return;
    }
    generarCodigos(root->izq, prefijo + "0", codigos, longitudes);
    generarCodigos(root->der, prefijo + "1", codigos, longitudes);
}

// Verificar propiedad prefijo
bool verificarPrefijo(const unordered_map<char,string> &codigos) {
    vector<string> lista;
    for (auto &kv : codigos) lista.push_back(kv.second);
    for (size_t i=0; i<lista.size(); i++) {
        for (size_t j=i+1; j<lista.size(); j++) {
            if (lista[i].size() <= lista[j].size()) {
                if (lista[j].substr(0, lista[i].size()) == lista[i]) return false;
            } else {
                if (lista[i].substr(0, lista[j].size()) == lista[j]) return false;
            }
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: ./huffman corpus.txt\n";
        return 1;
    }
    string filename = argv[1];
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "No se pudo abrir archivo " << filename << "\n";
        return 1;
    }

    // Leer archivo y contar frecuencias
    unordered_map<char,int> freqs;
    char c;
    long total = 0;
    while (fin.get(c)) {
        freqs[c]++;
        total++;
    }
    fin.close();

    cout << "Archivo: " << filename << " (" << total << " caracteres leidos)\n";
    cout << "Alfabeto: " << freqs.size() << " simbolos\n";

    // Construir Huffman
    Node* root = construirHuffman(freqs);

    unordered_map<char,string> codigos;
    unordered_map<char,int> longitudes;
    generarCodigos(root, "", codigos, longitudes);

    // Verificar propiedad prefijo
    bool prefijo_ok = verificarPrefijo(codigos);
    cout << "Propiedad prefijo: " << (prefijo_ok ? "OK" : "VIOLADA") << "\n";

    // Longitud media
    double longitud_media = 0.0;
    for (auto &kv : freqs) {
        longitud_media += (double)kv.second / total * longitudes[kv.first];
    }
    cout << "Longitud media Huffman: " << longitud_media << " bits/simbolo\n";

    // Longitud fija
    int Lfija = ceil(log2((double)freqs.size()));
    cout << "Longitud fija: " << Lfija << " bits/simbolo\n";

    double antes = total * Lfija;
    double despues = total * longitud_media;
    cout << "Tamano original (bits): " << antes << "\n";
    cout << "Tamano Huffman (bits): " << despues << "\n";
    cout << "Compresion: " << (100.0 * (1 - despues/antes)) << "%\n";

    // Guardar tabla en CSV
    ofstream fout("tabla_codigos.csv");
    fout << "simbolo,frecuencia,codigo,longitud\n";
    for (auto &kv : codigos) {
        string simbolo;
        if (kv.first == '\n') simbolo = "\\n";
        else if (kv.first == '\t') simbolo = "\\t";
        else if (kv.first == ',') simbolo = "\\,";
        else simbolo = string(1, kv.first);
        fout << "\"" << simbolo << "\"," << freqs[kv.first] << ","
             << kv.second << "," << longitudes[kv.first] << "\n";
    }
    fout.close();
    cout << "Tabla guardada en tabla_codigos.csv\n";

    return 0;
}
