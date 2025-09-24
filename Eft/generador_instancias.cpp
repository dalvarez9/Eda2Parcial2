#include <bits/stdc++.h>
using namespace std;

struct Actividad { int inicio, fin; };

int main() {
    int numInstancias = 10;    // número de instancias grandes
    int minN = 5000, maxN = 15000; // rango de tamaño permitido
    int maxTiempo = 30000;     // rango de tiempos [0, maxTiempo)

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distN(minN, maxN);
    uniform_int_distribution<> distT(0, maxTiempo-1);

    cout << "[\n";
    for (int inst = 0; inst < numInstancias; inst++) {
        int n = distN(gen);  // tamaño variable entre 5000 y 15000
        cout << "[";
        for (int i = 0; i < n; i++) {
            int s = distT(gen);
            int f = distT(gen);
            if (f <= s) f = s + 1; // asegurar fi > si
            cout << "[" << s << "," << f << "]";
            if (i != n-1) cout << ",";
        }
        cout << "]";
        if (inst != numInstancias-1) cout << ",";
        cout << "\n";
    }
    cout << "]\n";

    return 0;
}
