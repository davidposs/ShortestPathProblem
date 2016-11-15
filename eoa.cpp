//*****************************************************************************
// David Poss
// CPSC 335
// Project 3
// 10/26/2016
//*****************************************************************************

/* Program Description:
EOA - Exhaustic Optimization Algorithm - finds the shortest path by generating
all permutations of the vertices and choosing the shortest Hamiltonian cycle.
*/

#include <iostream>
#include <chrono>
#include <math.h>

struct point {
    float x, y; //x and y coordinates of graph
};

float farthest(int n, point* graph);
void print_cycle(int n, point* G, int* seq);
void print_perm(int n, int *A, int sizeA, point* G, 
        int* bestSet, float& bestDist);

int main() {
    int n; //number of vertices in the graph
    std::cout << "Enter the number of vertices (>2): ";
    std::cin >> n;
    if (n < 3) {
        std::cout << "n must be higher than 2\n";
        return 0;
    }
    point* graph = new point[n]; //graph is a collection of points

    std::cout << "Enter the points, making sure they are distinct: \n";
    for (int i = 0; i < n; i++) {
        std::cout << "  Point " << i << ": (x,y) = ";
        std::cin >> graph[i].x >> graph[i].y;
    }

    int* bestSet = new int[n + 1]; //n + 1 since we need to go back to start
    auto start = std::chrono::high_resolution_clock::now();
    float dist = farthest(n, graph);
    float bestDist = n*dist;

    int* A = new int[n + 1];
    for (int i = 0; i < n + 1; i++) {
        A[i] = i;
        bestSet[i] = 0;
    }

    print_perm(n, A, n + 1, graph, bestSet, bestDist);

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "The Hamiltonian Cycle of minimum length is " << std::endl;
    print_cycle(n, graph, bestSet);
    std::cout << "Minimum length is: " << bestDist << std::endl;
    int microseconds = int(std::chrono::duration_cast<
            std::chrono::microseconds>(end - start).count());
    double seconds = microseconds / 1E6;
    std::cout << "Elapsed time: " << seconds << " seconds" << std::endl;
    delete[] graph;
    delete[] A;
    delete[] bestSet;

    return 0;
}

void print_perm(int n, int* A, int sizeA, point* G, int* bestSet, float& bestDist) {
    int i;
    float dist;
    if (n == 1) {
        dist = 0;
        for (int j = 0; j < sizeA - 2; j++) {
            dist += sqrt((pow(G[A[j]].x - G[A[j + 1]].x, 2)) 
                    + pow(G[A[j]].y - G[A[j + 1]].y, 2));
        }
        dist += sqrt((pow(G[A[sizeA - 2]].x - G[A[0]].x, 2)) 
                + pow(G[A[sizeA - 2]].y - G[A[0]].y, 2));
        if (dist < bestDist) {
            bestDist = dist;
            for (int k = 0; k < sizeA; k++) {
                bestSet[k] = A[k];
            }
        }
    }//end if
    else {
        for (i = 0; i < n - 1; i++) {
            print_perm(n - 1, A, sizeA, G, bestSet, bestDist);
            if (n % 2 == 0) {
                int temp = A[i];
                A[i] = A[n - 1];
                A[n - 1] = temp;
            }
            else {
                int temp = A[0];
                A[0] = A[n - 1];
                A[n - 1] = temp;
            }
        }
        print_perm(n - 1, A, sizeA, G, bestSet, bestDist);
    }//end else
}//end of print_perm

float farthest(int n, point* P) {
    float max_dist = 0;
    float dist;
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n; j++) {
            dist = (P[i].x - P[j].x)*(P[i].x - P[j].x) 
                + (P[i].y - P[j].y)*(P[i].y - P[j].y);
            if (max_dist < dist)
                max_dist = dist;
        }
    return sqrt(max_dist);
}//end of farthest

void print_cycle(int n, point* G, int* seq) {
    for (int i = 0; i < n; i++) {
        std::cout << "(" << G[seq[i]].x << "," << G[seq[i]].y << ")";
        if (i < n - 1) {
            std::cout << ", ";
        }
        else {  //Reprint first node
            std::cout << ", (" << G[seq[0]].x << ", " << G[seq[0]].y << ")";
        }
    }
    std::cout << std::endl;
}//end of print_cycle
