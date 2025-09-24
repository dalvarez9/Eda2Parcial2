#include <bits/stdc++.h>
using namespace std;

struct Actividad {
    int inicio, fin;
};

// ---------- Greedy EFT ----------
vector<Actividad> seleccion_eft(vector<Actividad> acts) {
    sort(acts.begin(), acts.end(), [](auto &a, auto &b){
        return a.fin < b.fin;
    });
    vector<Actividad> seleccion;
    int fin_ultima = -1e9;
    for (auto &a : acts) {
        if (a.inicio >= fin_ultima) {
            seleccion.push_back(a);
            fin_ultima = a.fin;
        }
    }
    return seleccion;
}

// ---------- Fuerza bruta (n <= 20) ----------
vector<Actividad> mejor_subset;
int mejor_cardinalidad;

void brute_rec(vector<Actividad> &acts, int idx, vector<Actividad> &current) {
    if (idx == (int)acts.size()) {
        bool ok = true;
        for (int i = 0; i < (int)current.size(); i++) {
            for (int j = i+1; j < (int)current.size(); j++) {
                if (!(current[i].fin <= current[j].inicio || current[j].fin <= current[i].inicio)) {
                    ok = false;
                    break;
                }
            }
            if (!ok) break;
        }
        if (ok && (int)current.size() > mejor_cardinalidad) {
            mejor_cardinalidad = current.size();
            mejor_subset = current;
        }
        return;
    }
    brute_rec(acts, idx+1, current);
    current.push_back(acts[idx]);
    brute_rec(acts, idx+1, current);
    current.pop_back();
}

vector<Actividad> seleccion_bruteforce(vector<Actividad> acts) {
    mejor_subset.clear();
    mejor_cardinalidad = -1;
    vector<Actividad> current;
    brute_rec(acts, 0, current);
    return mejor_subset;
}

// ---------- Generar aleatoriamente ----------
vector<Actividad> generar_instancia(int n, int maxTiempo) {
    vector<Actividad> acts;
    random_device rd; mt19937 gen(rd());
    uniform_int_distribution<> dist(0, maxTiempo-1);

    for (int i = 0; i < n; i++) {
        int s = dist(gen);
        int f = dist(gen);
        if (f <= s) f = s + 1;
        acts.push_back({s,f});
    }
    return acts;
}

// ---------- Leer desde JSON ----------
vector<vector<Actividad>> leer_json_multinst(const string &filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Error: no se pudo abrir " << filename << "\n";
        exit(1);
    }

    string content((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();

    vector<vector<Actividad>> instancias;
    vector<Actividad> actual;
    int s,f;
    bool dentro_instancia = false;

    for (size_t i = 0; i < content.size(); i++) {
        if (content[i] == '[') {
            if (i+1 < content.size() && content[i+1] == '[') {
                // empieza una nueva instancia
                actual.clear();
                dentro_instancia = true;
            }
            // detectar par [s,f]
            size_t j = i+1;
            while (j < content.size() && (isdigit(content[j]) || content[j]=='-')) j++;
            if (j < content.size() && content[j] == ',') {
                s = stoi(content.substr(i+1, j-(i+1)));
                size_t k = j+1;
                while (k < content.size() && (isdigit(content[k]) || content[k]=='-')) k++;
                if (k < content.size() && content[k] == ']') {
                    f = stoi(content.substr(j+1, k-(j+1)));
                    actual.push_back({s,f});
                }
            }
        } else if (content[i] == ']') {
            // fin de una instancia (cuando viene "]]")
            if (i+1 < content.size() && content[i+1] == ']') {
                if (!actual.empty()) instancias.push_back(actual);
                dentro_instancia = false;
            }
        }
    }

    return instancias;
}

// ---------- MAIN ----------
int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Actividad> acts;

    if (argc == 2) {
    string filename = argv[1];
    auto instancias = leer_json_multinst(filename);
    cout << "Se cargaron " << instancias.size() << " instancias desde " << filename << "\n";

    for (size_t t = 0; t < instancias.size(); t++) {
        auto acts = instancias[t];
        cout << "\nInstancia " << t+1 << " (n=" << acts.size() << "):\n";
        if (acts.size() <= 20) {
            auto eft_sol = seleccion_eft(acts);
            auto bf_sol  = seleccion_bruteforce(acts);
            cout << "EFT = " << eft_sol.size() 
                 << " | Fuerza Bruta = " << bf_sol.size()
                 << " -> " << (eft_sol.size() == bf_sol.size() ? "OK" : "DIFERENCIAS") 
                 << "\n";
        } else {
            auto start = chrono::high_resolution_clock::now();
            auto eft_sol = seleccion_eft(acts);
            auto end = chrono::high_resolution_clock::now();
            double ms = chrono::duration<double,milli>(end-start).count();
            cout << "EFT selecciona " << eft_sol.size() 
                 << " actividades en " << ms << " ms\n";
        }
    }
}
}
