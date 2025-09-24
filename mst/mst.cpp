#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v;
    int w;
};

// =================== DSU (Disjoint Set Union) ===================
struct DSU {
    vector<int> parent, rankv;
    DSU(int n) {
        parent.resize(n);
        rankv.assign(n,0);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rankv[a] < rankv[b]) swap(a,b);
        parent[b] = a;
        if (rankv[a] == rankv[b]) rankv[a]++;
        return true;
    }
};

// =================== Kruskal ===================
long long mst_kruskal(int V, vector<Edge> edges) {
    sort(edges.begin(), edges.end(), [](auto &a, auto &b){ return a.w < b.w; });
    DSU dsu(V);
    long long cost = 0;
    int edges_used = 0;
    for (auto &e : edges) {
        if (dsu.unite(e.u, e.v)) {
            cost += e.w;
            edges_used++;
            if (edges_used == V-1) break;
        }
    }
    return cost;
}

// =================== Prim ===================
long long mst_prim(int V, vector<vector<pair<int,int>>> &adj) {
    vector<bool> vis(V, false);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    pq.push({0,0});
    long long cost = 0;

    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        int w = top.first;
        int u = top.second;

        if (vis[u]) continue;
        vis[u] = true;
        cost += w;

        for (auto &nbr : adj[u]) {
            int v = nbr.first;
            int ww = nbr.second;
            if (!vis[v]) pq.push({ww,v});
        }
    }
    return cost;
}

// =================== Generador de grafos ===================
vector<Edge> generar_grafo(int V, int E) {
    random_device rd; mt19937 gen(rd());
    uniform_int_distribution<> distW(1,1000);
    vector<Edge> edges;
    edges.reserve(E);

    unordered_set<long long> usado; // evitar aristas repetidas
    uniform_int_distribution<> distV(0,V-1);

    while ((int)edges.size() < E) {
        int u = distV(gen), v = distV(gen);
        if (u == v) continue;
        int w = distW(gen);
        long long key = 1LL*min(u,v)*V + max(u,v);
        if (usado.count(key)) continue;
        usado.insert(key);
        edges.push_back({u,v,w});
    }
    return edges;
}

// Construir lista de adyacencia desde edges
vector<vector<pair<int,int>>> edges_to_adj(int V, const vector<Edge> &edges) {
    vector<vector<pair<int,int>>> adj(V);
    for (auto &e : edges) {
        adj[e.u].push_back({e.v, e.w});
        adj[e.v].push_back({e.u, e.w});
    }
    return adj;
}

// =================== MAIN ===================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> vertices = {100, 500, 1000};

    ofstream fout("resultados_mst.csv");
    fout << "V,E,costoKruskal,costoPrim,tKruskal(ms),tPrim(ms),tipo\n";

    for (int V : vertices) {
        // 1) Grafo disperso
        int E_disp = 3*V;
        auto edges_disp = generar_grafo(V, E_disp);
        auto adj_disp = edges_to_adj(V, edges_disp);

        auto t1 = chrono::high_resolution_clock::now();
        long long ck = mst_kruskal(V, edges_disp);
        auto t2 = chrono::high_resolution_clock::now();
        long long cp = mst_prim(V, adj_disp);
        auto t3 = chrono::high_resolution_clock::now();
        double tk = chrono::duration<double,milli>(t2-t1).count();
        double tp = chrono::duration<double,milli>(t3-t2).count();

        fout << V << "," << E_disp << "," << ck << "," << cp << "," << tk << "," << tp << ",disperso\n";
        cout << "V=" << V << " disperso: Kruskal=" << ck << " (" << tk << " ms), Prim=" << cp << " (" << tp << " ms)\n";

        // 2) Grafo denso
        int E_dens = V*(V-1)/4; // densidad ~25%
        auto edges_dens = generar_grafo(V, E_dens);
        auto adj_dens = edges_to_adj(V, edges_dens);

        t1 = chrono::high_resolution_clock::now();
        ck = mst_kruskal(V, edges_dens);
        t2 = chrono::high_resolution_clock::now();
        cp = mst_prim(V, adj_dens);
        t3 = chrono::high_resolution_clock::now();
        tk = chrono::duration<double,milli>(t2-t1).count();
        tp = chrono::duration<double,milli>(t3-t2).count();

        fout << V << "," << E_dens << "," << ck << "," << cp << "," << tk << "," << tp << ",denso\n";
        cout << "V=" << V << " denso: Kruskal=" << ck << " (" << tk << " ms), Prim=" << cp << " (" << tp << " ms)\n";
    }

    fout.close();
    cout << "Resultados guardados en resultados_mst.csv\n";
    return 0;
}
