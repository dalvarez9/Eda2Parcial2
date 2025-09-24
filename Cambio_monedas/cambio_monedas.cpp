#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <climits>
#include <sstream>

using namespace std;

// Función para implementar el algoritmo greedy
vector<int> cambio_greedy(int M, const vector<int>& D) {
    vector<int> resultado(D.size(), 0);
    int monto_restante = M;
    
    // Procesar las denominaciones de mayor a menor
    for (int i = 0; i < D.size() && monto_restante > 0; i++) {
        if (D[i] <= monto_restante) {
            resultado[i] = monto_restante / D[i];
            monto_restante %= D[i];
        }
    }
    
    // Si no se pudo hacer el cambio completo, devolver vector vacío
    if (monto_restante > 0) {
        return vector<int>();
    }
    
    return resultado;
}

// Función auxiliar para búsqueda exhaustiva recursiva
void buscar_combinacion_optima(int M, const vector<int>& D, vector<int>& combinacion_actual, 
                              vector<int>& mejor_combinacion, int& min_monedas, 
                              int indice, int monedas_usadas) {
    
    // Poda: si ya usamos más monedas que la mejor solución encontrada, parar
    if (monedas_usadas >= min_monedas) {
        return;
    }
    
    // Caso base: si el monto es exactamente 0, encontramos una solución válida
    if (M == 0) {
        if (monedas_usadas < min_monedas) {
            min_monedas = monedas_usadas;
            mejor_combinacion = combinacion_actual;
        }
        return;
    }
    
    // Caso base: si el monto es negativo o no hay más denominaciones, parar
    if (M < 0 || indice >= D.size()) {
        return;
    }
    
    // Probar diferentes cantidades de la denominación actual
    int max_cantidad = M / D[indice];
    for (int cantidad = max_cantidad; cantidad >= 0; cantidad--) {
        combinacion_actual[indice] = cantidad;
        buscar_combinacion_optima(M - cantidad * D[indice], D, combinacion_actual, 
                                 mejor_combinacion, min_monedas, indice + 1, 
                                 monedas_usadas + cantidad);
    }
    combinacion_actual[indice] = 0;
}

// Función para implementar búsqueda exhaustiva con poda
vector<int> cambio_bruteforce(int M, const vector<int>& D) {
    vector<int> combinacion_actual(D.size(), 0);
    vector<int> mejor_combinacion;
    int min_monedas = INT_MAX;
    
    buscar_combinacion_optima(M, D, combinacion_actual, mejor_combinacion, min_monedas, 0, 0);
    
    return mejor_combinacion;
}

// Función para calcular el número total de monedas en una combinación
int contar_monedas(const vector<int>& combinacion) {
    int total = 0;
    for (int cantidad : combinacion) {
        total += cantidad;
    }
    return total;
}

// Función para convertir combinación a string para mostrar
string combinacion_to_string(const vector<int>& combinacion, const vector<int>& D) {
    if (combinacion.empty()) {
        return "No hay solucion";
    }
    
    stringstream ss;
    bool primera = true;
    for (int i = 0; i < combinacion.size(); i++) {
        if (combinacion[i] > 0) {
            if (!primera) ss << "+";
            ss << combinacion[i] << "*" << D[i];
            primera = false;
        }
    }
    return ss.str();
}

// Función para verificar si dos combinaciones son iguales
bool combinaciones_iguales(const vector<int>& c1, const vector<int>& c2) {
    if (c1.size() != c2.size()) return false;
    for (int i = 0; i < c1.size(); i++) {
        if (c1[i] != c2[i]) return false;
    }
    return true;
}

int main() {
    // Sistema de denominaciones (ordenado de mayor a menor)
    vector<int> D = {6,4,1};
    
    cout << "=== EJERCICIO 1: CAMBIO DE MONEDAS ===" << endl;
    cout << "Sistema de denominaciones: ";
    for (int i = 0; i < D.size(); i++) {
        cout << D[i];
        if (i < D.size() - 1) cout << ", ";
    }
    cout << endl << endl;
    
    // Abrir archivo CSV para escribir resultados
    ofstream archivo("resultados_ej1.csv");
    archivo << "M,Combinacion_Greedy,Combinacion_Optima,Num_Monedas_Greedy,Num_Monedas_Optima,Coinciden" << endl;
    
    int primer_fallo = -1;
    
    cout << setw(3) << "M" << " | " << setw(20) << "Greedy" << " | " << setw(20) << "Optimo" 
         << " | " << setw(8) << "Monedas" << " | " << setw(9) << "Coinciden" << endl;
    cout << string(70, '-') << endl;
    
    // Analizar para M = 1 hasta 30
    for (int M = 1; M <= 30; M++) {
        vector<int> greedy = cambio_greedy(M, D);
        vector<int> optimo = cambio_bruteforce(M, D);
        
        int monedas_greedy = greedy.empty() ? -1 : contar_monedas(greedy);
        int monedas_optimo = optimo.empty() ? -1 : contar_monedas(optimo);
        
        bool coinciden = combinaciones_iguales(greedy, optimo);
        
        // Detectar primer fallo del greedy
        if (!coinciden && primer_fallo == -1) {
            primer_fallo = M;
        }
        
        // Mostrar en consola
        cout << setw(3) << M << " | " << setw(20) << combinacion_to_string(greedy, D) 
             << " | " << setw(20) << combinacion_to_string(optimo, D)
             << " | " << setw(3) << monedas_greedy << "/" << setw(3) << monedas_optimo
             << " | " << setw(9) << (coinciden ? "SI" : "NO") << endl;
        
        // Escribir al CSV
        archivo << M << ",\"" << combinacion_to_string(greedy, D) << "\",\""
                << combinacion_to_string(optimo, D) << "\"," << monedas_greedy 
                << "," << monedas_optimo << "," << (coinciden ? "SI" : "NO") << endl;
    }
    
    archivo.close();
    
    cout << endl << "=== RESULTADOS ===" << endl;
    cout << "Archivo CSV generado: resultados_ej1.csv" << endl;
    
    if (primer_fallo != -1) {
        cout << "El algoritmo greedy falla por primera vez en M = " << primer_fallo << endl;
    } else {
        cout << "El algoritmo greedy no falla para M = 1..30" << endl;
    }
    
    return 0;
}