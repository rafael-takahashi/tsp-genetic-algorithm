#pragma once

#include <iostream>
#include <iomanip>

struct Node {
    int id;
    double x, y;
    
    Node(int id, double x, double y) : id(id), x(x), y(y) {}

    void print() const {
        std::cout << "Node " << id << ": ("
              << std::fixed << std::setprecision(15) << x << ", "
              << std::fixed << std::setprecision(15) << y << ")" << std::endl;
    }
};

double calculate_distance(Node A, Node B);