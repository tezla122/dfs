/*@author Rotar Dan Alexandru
 * @group 30226
 *
 * DFS:
 * It relies on a stack-based approach, typically implemented via recursion, to
manage the exploration of nodes.
 * During execution, it assigns discovery and finish timestamps to vertices,
which are essential for classifying edges and identifying cycles.
 * The time complexity of DFS is O(V+E), where V represents the number of
vertices and E the number of edges, as every node is visited once and every edge
is explored exactly once in a directed graph. The space complexity is O(V),
 * dictated by the maximum depth of the recursion stack.
 *Tarjan:
Tarjan's algorithm is a highly efficient method for identifying strongly
connected components within a directed graph using a single pass of DFS. The
algorithm maintains an index for the order of discovery and a low-link value for
each vertex, representing the smallest index reachable from that vertex in the
DFS tree. By utilizing a auxiliary stack to track vertices currently in the
recursion branch, it identifies a component's root when a node's index matches
its low-link value. The time complexity is O(V+E), making it asymptotically
optimal for SCC identification, while the space complexity remains O(V) due to
the storage requirements for indices, low-links, and the stack.

Khan:
Kahn’s algorithm provides an iterative approach to performing a topological sort
on a directed acyclic graph. Unlike DFS-based methods, it functions by
repeatedly identifying and removing vertices with an in-degree of zero—those
with no incoming dependencies. These vertices are placed into a queue, and as
they are removed, the in-degree of their neighbors is decremented. If a
neighbor’s in-degree reaches zero, it is added to the queue. The process
continues until the queue is empty; if the number of processed vertices is less
than the total count, a cycle exists. The algorithm operates with a time
complexity of O(V+E) and a space complexity of O(V) to store the in-degree array
and the queue.
 *
 *
 */
#include "profiler.h"
#include <bits/stdc++.h>
using namespace std;
int op = 0;
enum Color { WHITE, GRAY, BLACK };

struct Graph {
  int v;
  vector<vector<int>> adj;
  vector<Color> color;
  vector<int> pi;     // parent arr
  vector<int> d;      // discovery
  vector<int> f;      // finish time
  int time;           // global for time
  vector<int> in_deg; // in degree for all the vertices inside the graph
  vector<int> index;
  vector<int> lowlink;
  vector<bool> on_stack;
  stack<int> s;
  int curr_index;
};
void prep_vectors(Graph &G) {
  G.color.assign(G.v, WHITE);
  G.pi.assign(G.v, -1);
  G.d.assign(G.v, 0);
  G.f.assign(G.v, 0);
  G.time = 0;
}
void dfs_visit(Graph &G, int u) {
  G.time = G.time + 1;
  G.d[u] = G.time;
  op++;
  G.color[u] = GRAY;
  op++;
  for (int v : G.adj[u]) {
    op++;
    if (G.color[v] == WHITE) {
      G.pi[v] = u;
      op++;
      dfs_visit(G, v);
    }
  }
  G.color[u] = BLACK;
  G.time = G.time + 1;
  G.f[u] = G.time;
  op += 2;
}

void dfs(Graph &G) {
  prep_vectors(G);
  for (int i = 0; i < G.v; i++) {
    G.color[i] = WHITE;
    G.pi[i] = -1;
  }
  G.time = 0;
  for (int i = 0; i < G.v; i++) {
    if (G.color[i] == WHITE) {
      dfs_visit(G, i);
    }
  }
}

void khan_topological(Graph &G) {
  vector<int> top_order;
  queue<int> q;

  for (int i = 0; i < G.v; i++) {
    if (G.in_deg[i] == 0) {
      q.push(i);
    }
  }

  int cnt = 0;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    top_order.push_back(u);

    for (int v : G.adj[u]) {
      G.in_deg[v]--;
      if (G.in_deg[v] == 0) {
        q.push(v);
      }
    }

    cnt++;
  }

  if (cnt != G.v) {
    cout << "Has cycle" << endl;
  } else {
    cout << "Khan algorithm for topological sort" << endl;
    for (int node : top_order) {
      cout << node << " ";
    }
    cout << endl;
  }
}

void strong_connect(int v, Graph &G) {
  G.index[v] = G.curr_index;
  G.lowlink[v] = G.curr_index;
  G.curr_index++;

  G.s.push(v);
  G.on_stack[v] = true;
  for (int w : G.adj[v]) {
    if (G.index[w] == -1) {
      strong_connect(w, G);
      G.lowlink[v] = min(G.lowlink[v], G.lowlink[w]);
    } else if (G.on_stack[w]) {
      G.lowlink[v] = min(G.lowlink[v], G.index[w]);
    }
  }

  if (G.lowlink[v] == G.index[v]) {
    cout << "SCC : ";
    int w;
    do {
      w = G.s.top();
      G.s.pop();
      G.on_stack[w] = false;
      cout << w << " ";
    } while (w != v);
    cout << endl;
  }
}

void tarjan(Graph &G) {
  G.curr_index = 0;
  for (int i = 0; i < G.v; i++) {
    G.index[i] = -1;
    G.on_stack[i] = false;
  }

  for (int i = 0; i < G.v; i++) {
    if (G.index[i] == -1) {
      strong_connect(i, G);
    }
  }
}
set<pair<int, int>> generate_unique_edge(int v, int e) {
  set<pair<int, int>> edges;
  int maxEdge = v * (v - 1);
  if (e > maxEdge)
    e = maxEdge;
  while (edges.size() < e) {
    int u = rand() % v;
    int v = rand() % v;

    if (u != v) {
      edges.insert({u, v});
    }
  }
  return edges;
}
/*
Graph generate_random_graph(Graph &G, int v, int e) {
  G.v = v;
  G.adj.assign(v, vector<int>());
  srand(time(0));
  set<pair<int, int>> unique_edge = generate_unique_edge(v, e);
  for (auto const &edge : unique_edge) {
    G.adj[edge.first].push_back(edge.second);
  }
  return G;
}
*/
Graph generate_random_graph(int V, int e) {

  Graph g;
  g.v = V;
  g.adj.resize(V);
  int maxEdge = V * (V - 1);
  if (e > maxEdge) {
    e = maxEdge;
  }
  set<pair<int, int>> unique_edge;
  while (unique_edge.size() < (size_t)e) {
    int u = rand() % V;
    int v = rand() % V;
    if (u != v && unique_edge.insert({u, v}).second) {
      g.adj[u].push_back(v);
    }
  }
  return g;
}

void performance() {

  Profiler p("dfs performance ");
  srand(time(0));
  const int v_fixed = 100;
  Graph g;
  for (int e = 1000; e <= 9900; e += 100) {
    g = generate_random_graph(v_fixed, e);
    op = 0;
    dfs(g);
    p.countOperation("Fixed_vertices_-_Variable_Edges", e, op);
  }
  const int e = 4500;
  for (int v = 100; v <= 200; v += 10) {
    g = generate_random_graph(v, e);
    op = 0;
    dfs(g);
    p.countOperation("Variable_vertices_-_Fixed_Edges", v, op);
  }
  p.showReport();
}

int main() {
  char c;
  cin >> c;

  if (c == 'D') {
    Graph G;
    int E;

    cin >> G.v;
    cin >> E;

    // trb sa fac redimensionare in functite de nr de noduri citit

    G.adj.resize(G.v);
    G.color.resize(G.v);
    G.pi.resize(G.v);
    G.d.resize(G.v);
    G.f.resize(G.v);

    for (int i = 0; i < E; i++) {
      int u, v;
      cin >> u >> v;
      G.adj[u].push_back(v);
    }

    dfs(G);

    cout << "Rezultate" << endl;
    cout << "Node\tDescoperire\tFinalizare\tParinte\n";
    for (int i = 0; i < G.v; i++) {
      cout << i << "\t" << G.d[i] << "\t\t" << G.f[i] << "\t\t";
      if (G.pi[i] == -1)
        cout << "NIL";
      else
        cout << G.pi[i];
      cout << endl;
    }
  } else if (c == 'K') {
    Graph G;
    int E;
    cout << "Numar noduri ";
    cin >> G.v;
    cout << "Numar muchii ";
    cin >> E;

    G.adj.resize(G.v);
    G.in_deg.assign(G.v, 0);
    cout << "Introdu muchiile sursa - destinatie " << endl;
    for (int i = 0; i < E; i++) {
      int u, v;
      cin >> u >> v;
      G.adj[u].push_back(v);
      G.in_deg[v]++;
    }
    khan_topological(G);
  } else if (c == 'T') {
    Graph G;
    int E;
    cout << "Numar noduri ";
    cin >> G.v;
    cout << "Numar muchii ";
    cin >> E;

    G.adj.resize(G.v);
    G.index.resize(G.v);
    G.lowlink.resize(G.v);
    G.on_stack.resize(G.v);

    cout << "Introdu muchiile" << endl;
    for (int i = 0; i < E; i++) {
      int u, v;
      cin >> u >> v;
      G.adj[u].push_back(v);
    }
    cout << endl << "Strongly connected components " << endl;
    tarjan(G);
  } else if (c == 'R') {
    performance();
  }
  return 0;
}
