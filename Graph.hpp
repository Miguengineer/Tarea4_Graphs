//
// Created by krampus on 7/20/20.
//

#ifndef TAREA4FEDA_GRAPH_HPP
#define TAREA4FEDA_GRAPH_HPP

#endif //TAREA4FEDA_GRAPH_HPP

#include <bits/stdc++.h>

using namespace std;

/**
 * Clase que implementa un gráfico no dirigido (bidireccional). Contiene métodos para buscar usuarios con más seguidores
 * compactación, búsqueda de la presencia de algún usuario y búsuqeda de cliques
 */
class UndirectedGraph {
    // Vector de vectores para almacenar cliques
    vector<vector<string>> cliques;
    // Grafo con estructura de lista de adjacencias, implementada como vector para mejorar la eficiencia de acceso/búsqueda
    map<string, vector<string>> users;
    // Lleva track de nodos totales agregados, para evitar llamar métodos si no hay los suficientes nodos
    int totalNodes = 0;


public:
    UndirectedGraph();
    void addUser(const string& userA, const string& userB);
    void find(const string& u);
    void clique(vector<string> R, vector<string> P, vector<string> X);
    void compact();
    void follow(int n);
    void showGraph();
    vector<string> getNodes();
    void showVertices();
};

/**
 * Constructor. No requiere parámetros. Dummy constructor.
 */
UndirectedGraph::UndirectedGraph() = default;

/**
 * Agrega un usuario nuevo al grafo. Hace una conexión bidireccional (al descomentar la línea comentada)
 * @param userA: String de usuario A conectado a usuario B
 * @param userB: String de usuario B conectado a usuario A
 */
void UndirectedGraph::addUser(const string& userA, const string& userB) {
    // Agrega los usuarios de forma bidireccional
    users[userA].push_back(userB);
    users[userB].push_back(userA);
    // Incrementa contador de nodos
    totalNodes++;
}

/**
 * Muestra el grafo en pantalla. La representación es en forma de lista de adyacencia. Cada nodo está seguido de una
 * flecha con todas sus conexiones
 */
void UndirectedGraph::showGraph() {
    for (const auto& it: users){
        cout << it.first << "->";
        for (const auto& it2: it.second){
            cout << it2 << " ";
        }
        cout << endl;
    }
}

/**
 * Intenta encontrar si el usuario u existe en el grafo. Imprime en pantalla el resultado de la búsqueda. Comentado
 * está un código menos eficiente pero más sencillo, el cual aprovecha la forma en que se almacenó el grafo (map)
 * @param u: String de usuario a buscar en el grafo
 */
void UndirectedGraph::find(const string& u) {
    /*
    for (const auto& it: users){
        if (it.first == u){
            cout << "Yes" << endl;
            return;
        }
    }
    cout << "No" << endl;
    */
    // Cola para guardar temporalmente los nodos
    queue<string> q;
    // Mapa para llevar nodos y un valor de verdad de si fueron visitados
    map<string, bool> visited;
    // Comienza, aleatoriamente, con el "primer" nodo del grafo
    q.push(users.begin()->first);
    visited[users.begin()->first] = true;
    // Itera hasta que la cola esté vacía
    while(!q.empty()) {
        // Remueve la cabeza de la cola
        string currentUser = q.front();
        // Revisa si el primer elemento de la cola es el buscado
        if (currentUser == u) {
            cout << "Yes" << endl;
            return;
        }
        // Retira el elemento
        q.pop();
        // Itera en el vector de currentUser
        for (auto &i : users[currentUser]) {
            // Si los nodos adyacentes a currentUser no han sido visitados, se agregan para ser visitados
            if (!visited[i]) {
                // Se revisa match
                if (i == u) {
                    cout << "Yes" << endl;
                    return;
                }
                // Se marca visitado
                visited[i] = true;
                q.push(i);
            }
        }
    }
    // No se retornó al visitar todos los nodos, implica que el elemento no se encontró
    cout << "No" << endl;
}

/**
 * Imprime en pantalla los usuarios con más cantidad de seguidores. El orden de impresión es arbitrario (dependiente
 * de cómo se agregaron) y no hay un criterio definido para conflictos (usuarios con igual cantidad de seguidores). Dada
 * la construcción del algoritmo, siempre se imprimirán los últimos en ser ingresados (los que estén más profundamente
 * en el grafo).
 * @param n: Cantidad de usuarios con más seguidores a buscar
 */
void UndirectedGraph::follow(int n) {
    // Si se piden más usuarios de los ingresados, retorna informando error
    if (totalNodes < n){
        cout << "Error: Cantidad de usuarios requeridos excede tamaño total de usuarios registrados" << endl;
        return;
    }
    // Vector de los usuarios más seguidos
    vector <string> mostFollowed;
    // String temporal para almacenar persona más seguida actual
    string currentMostFollowed;
    // Temporal para buscar el largo máximo
    int maxLength;
    // Crea una copia de usuarios, para así eliminar cuando se ha agregado uno
    map<string, vector<string>> copyOfUsers(users);
    // Itera n veces para los usuarios requeridos
     for (int i = 0; i < n; i++){
         // Reinicio para buscar nuevo largo máximo
         maxLength = 0;
         // Busca por todos los nodos del grafo el que tenga el mayor largo
         for (const auto& it: copyOfUsers){
             // Si se encontró uno con mayor largo, se actualiza el largo y se almacena parcialmente el nodo
             if (maxLength <= it.second.size()){
                 maxLength = it.second.size();
                 currentMostFollowed = it.first;
             }
         }
         // El máximo ha sido encontrado, se agrega al vector de más seguidos
         mostFollowed.push_back(currentMostFollowed);
         // Se elimina de la lista, para evitar repeticiones
         copyOfUsers.erase(currentMostFollowed);
     }
     // Impresión en pantalla
     for (const auto& it: mostFollowed){
         cout << it << endl;
     }
}

/**
 * Realiza una búsqueda de cliques basado en el algoritmo de Bron-Kerbosch sin pivote.
 * Ref: https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
 * @param R: Conjunto de posible clique. Inicialmente vacío
 * @param P: Conjunto de todos los vértices. Inicialmente lleno de todos los vértices.
 * @param X: Conjunto de vértices ya visitados. Inicialmente vacío
 */
void UndirectedGraph::clique(vector<string> R, vector<string> P, vector<string> X) {
    // Revisa si están vacíos, lo que implicaría que hay un clique
    if (P.empty() && X.empty()){
        if (R.size() > 2){
            cout << "Clique found!" << endl;
            for (const auto& it: R)
                cout << it << " " << endl;
            cliques.push_back(R);
        }

        return;
    }
    // Crea una copia de P para evitar problemas en el iterador
    vector<string> pCopy(P);
    for (const auto& v: pCopy){
        // Vectores para almacenar las distintas operaciones de conjuntos
        vector<string> unionSet;
        vector<string> intersectionSetPNv;
        vector<string> intersectionSetXNv;
        vector<string> neighborsP;
        vector<string> vVector = {v};
        // Obtiene los vecinos de P
        for (const auto& neighbors: users[v]) neighborsP.push_back(neighbors);
        // R u {v}
        set_union(R.begin(), R.end(), vVector.begin(), vVector.end(), back_inserter(unionSet));
        // Obtiene intersección P y N(v)
        set_intersection(P.begin(), P.end(), neighborsP.begin(), neighborsP.end(), back_inserter(intersectionSetPNv));
        set_intersection(X.begin(), X.end(), neighborsP.begin(), neighborsP.end(), back_inserter(intersectionSetXNv));
        // Llamada iterativa segun algoritmo
        clique(unionSet, intersectionSetPNv, intersectionSetXNv);
        // Vectores para almacenar operaciones parciales
        vector<string> PminusV;
        vector<string> XunionV;
        // Obtiene P - {v} y X u {v}
        set_difference(P.begin(), P.end(), vVector.begin(), vVector.end(), inserter(PminusV, PminusV.begin()));
        set_union(X.begin(), X.end(), vVector.begin(), vVector.end(), back_inserter(XunionV));
        // Asigna  operaciones recien realizadas
        P = vector<string> (PminusV);
        X = vector<string> (XunionV);
    }
}

/**
 * Algoritmo naive para compactar un grafo, basado en los cliques maximales. Cada clique maximal es reemplazado
 * por un solo nodo. Para ser llamdo requiere que los cliques maximales hayan sido previamente encontrados
 */
void UndirectedGraph::compact() {
    // Nuevo grafo donde almacenar el nuevo compacto
    UndirectedGraph compactGraph;
    // Vectores auxiliares para almacenar todos los nodos y cliques
    vector<string> allNodes;
    vector<string> allCliques;
    // Vector donde se almacenaran los nuevos nodos (componentes)
    vector<vector<string>> components;
    // Obtiene todos los nodos presentes
    for (const auto& it: users) allNodes.push_back(it.first);
    for (const auto& it: cliques){
        for (const auto& it2: it) {
            allCliques.push_back(it2);
        }
    }
    // Indice para llevar control de la iteracion y asignar el nombre correcto a cada componente. Empieza de 0
    int index = 0;
    // Arma el nuevo grafo compacto. Reemplaza un clique por un solo nodo (newNode) y conecta todos los nodos exteriores
    // (aquellos que no estan en cliques) a este nuevo nodo. Lo hace para cada clique.
    for (const auto& clique: cliques){
        // Crea el nuevo nodo concatenando basado en index
        string newNode = "Componente " + to_string(index);
        // Obtiene los elementos fuera del clique que se deben conectar a éste
        vector<string> outerNodes;
        set_difference(allNodes.begin(), allNodes.end(), allCliques.begin(), allCliques.end(), inserter(outerNodes,
                outerNodes.begin()));
        // Itera en cada uno de los cliques y busca si hay alguna conexion entre los nodos exteriores y el clique actual
        for (const auto& inClique: clique){
            for (const auto& outers: outerNodes){
                for (const auto& it: users[outers]){
                    // Conexion encontrada. Agrega la conexion al grafo compacto
                    if (it == inClique){
                        compactGraph.addUser(newNode, outers);
                    }
                }
            }
        }
        index++;
        // NOTA: Una vez terminada la ejecucion de este trozo de codigo, conectamos todos los nodos exteriores a las
        // componentes, pero no dice nada sobre si las componentes estan conectadas entre si
    }
    // Revisa si las componentes están conectadas entre sí
    // Con i y j compara entre pares de cliques maximales
    for (int i = 0; i < cliques.size() - 1; i++){
        for (int j = i + 1; j < cliques.size(); j++){
            // Con it e it2 itera entre los elementos de los pares de cliques maximales
            for (const auto& it: cliques[i]){
                for (const auto& it2: cliques[j]){
                    // Finalmente itera sobre los vecinos de it y comprueba si está conectado. Si lo está, agrega la
                    // conexión
                    for (const auto& it3: users[it]){
                        // Los componentes estan efectivamente conectados
                        if (it3 == it2){
                            compactGraph.addUser("Componente " + to_string(i), "Componente " + to_string(j));
                        }

                    }
                }
            }


        }
    }
    // Imprime el grafo en pantalla
    compactGraph.showVertices();
}
/**
 * Obtiene todos los nodos presentes en el grafo. Utilizado principalmente para la primera iteracion de clique, pero
 * no se hace privado por si se requiriera externamente obtener todos los nodos
 * @return: Retorna un vector de nodos (strings)
 */
vector<string> UndirectedGraph::getNodes(){
    vector<string> nodes;
    for (const auto& it: users) nodes.push_back(it.first);
    return nodes;
}

/**
 * Muestra todas las conexiones presentes en el grafo en forma de (u,v) donde esta notacion indica que u sigue a v.
 * Dado que el grafico es bidireccional, si (u,v) existe implica que (v,u) existe tambien.
 */
void UndirectedGraph:: showVertices(){
    for (const auto& it: users){
        for (const auto& it2: it.second){
            cout << "(" + it.first + "," + it2 +  ")" << endl;
        }
    }
}







