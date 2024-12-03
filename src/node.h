#pragma once

#include <iostream>

struct Node {
    int id;
    double x, y;
    
    Node(int id, double x, double y) : id(id), x(x), y(y) {}

    void print() const {
        std::cout << "Node " << id << ": (" << x << ", " << y << ")" << std::endl;
    }
};

double calculate_distance(Node A, Node B);