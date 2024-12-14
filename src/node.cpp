#include "node.h"

#include <math.h>

double calculate_distance(Node A, Node B) {
    return sqrt((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));
}