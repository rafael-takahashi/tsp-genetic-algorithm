#include "tsp.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <limits>
#include <sstream>
#include "node.h"

using namespace std;

vector<Node> tsp_to_vector(string& file_path) {
    ifstream file(file_path);

    if (!file.is_open()) {
        cerr << "Failed to open file.\n";
        exit(1);
    }

    string line;
    vector<Node> node_list;
    
    while (getline(file, line) && line != "NODE_COORD_SECTION") continue;
    
    int id;
    double x, y;
    
    while (getline(file, line) && line != "EOF") {
        istringstream stream(line);
        string string_id, string_x, string_y;

        stream >> string_id >> string_x >> string_y;
        id = stoi(string_id);
        x = stod(string_x);
        y = stod(string_y); 

        node_list.emplace_back(id, x, y);
    }

    file.close();

    return node_list;
}

Path nearest_neighbor(const vector<Node>& node_list) {
    vector<Node> node_sequence;
    double res_distance = 0;
    vector<bool> visited(node_list.size(), false);

    int current = 0; // Change this to a random number to get a different starting point

    node_sequence.push_back(node_list[current]);

    visited[current] = true;

    while (node_sequence.size() < node_list.size()) {
        double min_distance = numeric_limits<double>::max();
        int next;

        for (size_t i = 0; i < node_list.size(); i++) {
            if (visited[i] == false) {
                double distance = calculate_distance(node_list[current], node_list[i]);
                if (distance < min_distance) {
                    min_distance = distance;
                    next = i;
                }
            }
        }

        node_sequence.push_back(node_list[next]);
        res_distance += min_distance;
        visited[next] = true;
        current = next;
    }

    return Path(res_distance, node_sequence);
}

void two_opt_swap(vector<Node>& sequence, unsigned int first, unsigned int second) {
    first++;
    while (first < second) {
        swap(sequence[first], sequence[second]);
        first++;
        second--;
    }
}

Path two_opt(const Path& path) {
    vector<Node> current_sequence = path.node_sequence;
    double current_distance = path.distance;
    bool has_improved;

    // Open file and start timer, can be deleted
    ofstream outfile;
    outfile.open("plot/data.txt");
    if (!outfile.is_open())
        cerr << "Unable to open file for writing.\n"; 
    auto start_time = chrono::high_resolution_clock::now();

    do {
        has_improved = false;
        for (size_t i = 0; i < path.size - 1; i++) {
            for (size_t j = i + 2; j < path.size; j++) {
                double delta = - calculate_distance(current_sequence[i], current_sequence[i + 1])
                               - calculate_distance(current_sequence[j], current_sequence[(j + 1) % path.size])
                               + calculate_distance(current_sequence[i], current_sequence[j])
                               + calculate_distance(current_sequence[i + 1], current_sequence[(j + 1) % path.size]);

                if (delta < 0 && delta < -1e-10) {
                    two_opt_swap(current_sequence, i, j);
                    current_distance += delta;
                    has_improved = true;
                }
            }
        }
        // Write data to file, can be deleted
        auto current_time = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = current_time - start_time;
        outfile << elapsed.count() << " " << current_distance << endl;
    } while (has_improved);

    return Path(current_distance, current_sequence);
}

void calcularDistancias(const vector<Node>& nodes, vector<vector<double>>& dist) {
    int n = nodes.size();
    dist.resize(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double d = calculate_distance(nodes[i], nodes[j]);
            dist[i][j] = dist[j][i] = d;
        }
    }
}

void insercaoMaisDistante(vector<Node> nodes, vector<Node>& caminho, double& distancia_total) {
    int n = nodes.size();
    vector<bool> visitados(n, false);

    caminho.push_back(nodes[0]);
    caminho.push_back(nodes[1]);
    caminho.push_back(nodes[2]);
    visitados[0] = visitados[1] = visitados[2] = true;
    
    distancia_total += calculate_distance(nodes[0], nodes[1]) + 
                        calculate_distance(nodes[1], nodes[2]) + 
                        calculate_distance(nodes[2], nodes[0]);

    while (caminho.size() < nodes.size()) {

        int proximo_no = -1;
        double maior_distancia_ao_caminho = -1;

        for (int i = 0; i < n; ++i) {
            if (visitados[i]) continue;
            double maior_distancia = -1;

            for (const auto& node_caminho : caminho) {
                maior_distancia = max(maior_distancia, calculate_distance(nodes[i], node_caminho));
            }

            if (maior_distancia > maior_distancia_ao_caminho) {
                maior_distancia_ao_caminho = maior_distancia;
                proximo_no = i;
            }
        }

        double menor_custo_insercao = numeric_limits<double>::max();
        int melhor_posicao = -1;

        for (size_t j = 0; j < caminho.size(); ++j) {
            int atual = j;
            int seguinte = (j + 1) % caminho.size();
            double custo_insercao = calculate_distance(caminho[atual], nodes[proximo_no]) +
                                    calculate_distance(nodes[proximo_no], caminho[seguinte]) -
                                    calculate_distance(caminho[atual], caminho[seguinte]);

            if (custo_insercao < menor_custo_insercao) {
                menor_custo_insercao = custo_insercao;
                melhor_posicao = seguinte;
            }
        }

        caminho.insert(caminho.begin() + melhor_posicao, nodes[proximo_no]);
        visitados[proximo_no] = true;
        distancia_total += menor_custo_insercao;
    }
}

double calcula_custo(const vector<Node>& caminho) {
    double custo_novo = 0.0;

    for (size_t i = 0; i < caminho.size(); ++i) {
        int prox_node = (i + 1) % caminho.size();
        custo_novo += calculate_distance(caminho[i], caminho[prox_node]);
    }

    return custo_novo;
}

vector<Node> troca_pares(vector<Node> caminho, double& custo) {
    int iteracoes_sem_melhoria = 0;
    int iteracoes_maximas = 0;

    while (iteracoes_sem_melhoria < 5 && iteracoes_maximas < 25) {
        bool houve_melhoria = false;

        size_t i = 0;
        while (i < caminho.size() && !houve_melhoria) {
            size_t j = i + 1;
            while (j < caminho.size() && !houve_melhoria) {
                swap(caminho[i], caminho[j]);

                double custo_apos_troca = calcula_custo(caminho);

                if (custo_apos_troca < custo) {
                    custo = custo_apos_troca;
                    houve_melhoria = true;
                    iteracoes_sem_melhoria = 0;

                    cout << "Novo custo: " << custo << endl;
                } else {
                    swap(caminho[i], caminho[j]);
                }
                ++j;
            }
            ++i;
        }

        if (!houve_melhoria) {
            iteracoes_sem_melhoria++;
        }
        iteracoes_maximas++;
    }

    return caminho;
}