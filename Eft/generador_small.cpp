#include <bits/stdc++.h>
using namespace std;

struct Actividad { int inicio, fin; };

int main() {
    int numInstancias = 50;   // número de instancias pequeñas
    int minN = 8, maxN = 18;  // cada instancia tendrá entre 8 y 18 actividades
    int maxTiempo = 30;       // límite superior de tiempos

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distN(minN, maxN);
    uniform_int_distribution<> distT(0, maxTiempo-1);

    cout << "[\n";
    for (int inst = 0; inst < numInstancias; inst++) {
        int n = distN(gen);
        vector<Actividad> acts;
        for (int i = 0; i < n; i++) {
            int s = distT(gen);
            int f = distT(gen);
            if (f <= s) f = s + 1; // asegurar fi > si
            acts.push_back({s,f});
        }

        // escribir instancia
        cout << "[";
        for (int i = 0; i < n; i++) {
            cout << "[" << acts[i].inicio << "," << acts[i].fin << "]";
            if (i != n-1) cout << ",";
        }
        cout << "]";
        if (inst != numInstancias-1) cout << ",";
        cout << "\n";
    }
    cout << "]\n";

    return 0;
}
