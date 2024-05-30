#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <list>
using namespace std;

// Graph representation by adj list, thanks to Hanin Ayyash
class Graph {
private:
    int size = 0;
    vector<list<int>> G;
public:
    Graph(){}
    int get_size() {
        return size;
    }
    void add_vertex(int v) {
        size++;
        list<int> l;
        G[v] = l;
    }
    void delete_vertex(int v) {
        size--;
        G.erase(G.begin()+v);
    }
    void add_edge(int u, int v) {
        G[u].push_back(v);
    }
    void delete_edge(int u, int v) {
        G[u].remove(v);
    }
    bool check_vertex(int v) {
        return v < G.size();
    }

    // Get vertex and all edges from it
    auto& get_vertex(int i) {
        return G[i];
    }

    // Set edges of vertex
    void set_vrtex(list<int> l, int i) {
        G[i] = l;
    }
};

// Check if an element within a stack, written by Shahd Ali
bool check(int i, stack<int>& S) {
    bool exist = false;
    vector<int> array;
    while(!S.empty()) {
        int x = S.top();
        S.pop();
        array.push_back(x);
        if(S.top() == i) {
            exist = true;
            break;
        }
    }
    for(int i = 0; i < array.size(); i++) {
        int x = array[array.size()-1];
        S.push(x);
        array.pop_back();
    }
    return exist;
}

// Variables for algorithms, by Aman Harb
int dfs_counter, *dfs_num, *comp, *order;
char* state, chain;
stack<int> Ostack, Rstack;

// Initialization function, by Haneen Shriba
void init(int size) {
    dfs_counter = 1;
    dfs_num = new int[size];
    comp = new int[size];

    // We will assume that new state is n, active state is a, and finished state is v
    state = new char[size];

    for(int i = 0; i < size; i++) {
        state[i] = 'n';
        dfs_num[i] = -1;
        comp[i] = -1;
    }
}

// Gabow base function, by Iskandar Mesrob
bool Cheriyan_Mehlhorn_Gabow(Graph G) {
    init(G.get_size());

    // Number of strongly connnected components in the graph
    int components = 0;

    for(int i = 0; i < G.get_size(); i++) {
        if(state[i] == 'n') DFS(i, G, "CMG");
    }
    for(int i = 0; i < G.get_size(); i++) {
        if(comp[i] > 0) components++;
        if(components > 1) return false;
    }
    return true;
}

// DFS but mixed with all statements needed for each step of each algorithm, By Representative .. Haidara Mohammad
void DFS(int vertex, Graph& G, string type, int source = -1) {

    // Type for what DFS will work for, CMG for Gabow and JS for Jens
    state[vertex] = 'a';
    dfs_num[vertex] = dfs_counter;
    dfs_counter++;
    if(type == "CMG") Ostack.push(vertex), Rstack.push(vertex);
    for(auto i = G.get_vertex(vertex).begin(); i != G.get_vertex(vertex).end(); i++) {
        if(state[*i] == 'n') type == "CMG" ? DFS(*i, G, "CMG") : G.delete_edge(vertex, *i), DFS(*i, G, "JS");
        else {
            if(type == "JS") {

                // When we discover that the chain is a cycle: chain = 'c'
                if(source > -1 && vertex == source) chain = 'c';

                else if(state[vertex] == 'a') G.delete_edge(vertex, *i);
            }
            if(type == "CMG" && check(*i, Ostack)) while(dfs_num[*i] > dfs_num[Rstack.top()])
                Rstack.pop();
        }
    }
    state[vertex] = 'f';
    if(type == "CMG" && vertex == Rstack.top()) {
        Rstack.pop();
        int w;
        do {
            w = Ostack.top();
            Ostack.pop();
            comp[w] = vertex;
        } while(w != vertex);
    }
}

bool Jens_Schmidt(Graph& G) {

    // Make a directed graph from the original, by Arwa Hammoud
    init(G.get_size());
    DFS(0, G, "JS");
    if(dfs_counter < G.get_size()) return false;

    // The order of dfs to use it for discovering the Chaza Abood
    order = new int[G.get_size()];
    for(int i = 0; i < G.get_size(); i++)
        for(int j = 0; j < G.get_size(); j++)
            if(dfs_num[j] == i) order[i] = j;

    // Discover chains, by Asmaa Tamer
    init(G.get_size());
    for(int i = 0; i < G.get_size(); i++) {

        // We will assume that the chain is just path not a cycle and with DFS we will find that
        chain = 'p';
        DFS(i, G, "JS", i);
        if(i == 0 && chain != 'c' || chain != 'p') return false;
    }
    return true;
}

int main() {
    Graph G;
    bool check = true;

    // Read datasets from txt files, by Aya Dayoub
    int v, u;
    string txt, line;
    cin>>txt;
    ifstream file(txt);
    for(int i = 0; i < 4; i++)
        getline(file, line);
    while(getline(file, line)) {
        stringstream stream(line);
        if(stream>>v>>u) {
            if(!G.check_vertex(v)) G.add_vertex(v);
            if(!G.check_vertex(u)) G.add_vertex(u);
            G.add_edge(v, u);
        }
    }

    // Check if the graph is 2-vertex strongly Bi-connected, by Batoul Nouh
    for(int i = 0; i < G.get_size(); i++) {
        auto l = G.get_vertex(i);
        G.delete_vertex(i);
        if(!Cheriyan_Mehlhorn_Gabow(G) || !Jens_Schmidt(G)) {
            check = false;
            break;
        }
        G.add_vertex(i);
        G.set_vertex(l, i);
    }
    cout<<(check ? "YES" : "NO");
}