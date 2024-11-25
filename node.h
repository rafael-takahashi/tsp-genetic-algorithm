#pragma once

#include <iostream>

struct Node {
    int id;
    double x, y;
    
    Node(int id, double x, double y) : id(id), x(x), y(y) {}

    void print() const {
        std::cout << "Node ID: " << id << " | Coordinates: (" << x << ", " << y << ")" << std::endl;
    }
};