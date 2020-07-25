#include <iostream>
#include <bits/stdc++.h>
#include "Graph.hpp"
using namespace std;

/**
 * Particiona un vector que contiene palabras y las añade en otro vector de strings
 * @param s: String a particionar
 * @param delim: Delimitador que indice cómo se particionará
 * @param elems: Vector donde se almacerán las palabras partidas
 */
void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

/**
 * Extiende la función anterior para particionar basado en un espacio, así una frase como "Hola Mundo" es particionada
 * basada en el espacio presente y añade al vector de retorno las palabras "Hola" y "Mundo" a los espacios elems[0] y
 * elems[1] respectivamente.
 * @param s: String a particionar
 * @param delim: Caracter delimitador. Utilizada especialmente para separar por espacio  ' '.
 * @return
 */
vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

int main() {
    UndirectedGraph ug;
    string currentString;
    vector <string> words;

    // Lee cada línea de entrada
    while (getline(cin, currentString)) {
        // Coloca las palabras separadas en un vector de strings
        words = split(currentString, ' ');
        // Dependiendo de la instrucción entregada, ejecuta el código correspondiente
        if (words[0] == "Add") {
            cout << "Adding users " << words[1] << "and " << words[2];
            ug.addUser(words[1], words[2]);
        } else if (words[0] == "Find") {
            cout << "Finding user " << words[1] << endl;
            ug.find(words[1]);
        } else if (words[0] == "Clique") {
            cout << "Finding cliques ... " << endl;
            vector<string> R = {};
            vector<string> X = {};
            ug.clique(R, ug.getNodes(), X);
        } else if (words[0] == "Follow") {
            cout << "Most " << words[1] << "Followed users" << endl;
            ug.follow(stoi(words[1]));
        } else if (words[0] == "Compact") {
            cout << "The compact graph is shown below" << endl;
            ug.compact();
        }

    }


    return 0;
}
