//*****************************************************************************
// David Poss
// CPSC 335
// Project 3
// 10/26/2016
//*****************************************************************************

/* Program Description:
INNA - Improve Nearest Neighbor Algorithm. Does not guarantee an optimal
solution, but finds a "best guess" for the shortest path between all 
vertices. 
*/

#include <iostream>
#include <chrono>
#include <math.h>

struct point {
    float x, y; //x and y coordinates of graph
};

void print_cycle(int, point*, int*);
int farthest_point(int, point*);//returns the index the furthest point
int nearest(int, point*, int, bool*);//finds the nearest unvisited neighbor

int main() {

    point *G; //pointer to points, a Graph
    int *M;   //pointer to vertices in G that form estimated shrotest path
    bool *Visited; //whether or not a point in G has been visited
    int i, n;
    float dist;
    int A, B;
    std::cout << "Enter the number of vertices (>2): "; 
    std::cin >> n;
    if (n < 3) {
        return 0;
    }
    G = new point[n];

    std::cout << "Enter the points; make sure they are distinct" << std::endl;
    for(i = 0; i < n; i++) {
        std::cout << "  Point " << i << ": (x,y) = ";
        std::cin >> G[i].x >> G[i].y;
    }
    M = new int[n]; //M holds the order of indicesi of G for the best cycle
    for(i = 0; i < n; i++) {
        M[i]=i;
    }
    auto start = std::chrono::high_resolution_clock::now();
    
    //To track if a point has been visited, so we don't add it twice
    Visited = new bool[n];
    for(i = 0; i < n; i++) {
        Visited[i] = false;
    }

    //Calculator the two furthest points, and choose 1 to start from
    A = farthest_point(n,G);
    M[0]= A;
    Visited[A] = true;

    for(i = 1; i < n; i++) {
        B = nearest(n, G, A, Visited); //B = nearest point to A
        //A = B; //set 
        M[i] = B; //add B to the list
        A = B;
        Visited[A] = true;
    }


    dist = 0; //Calculate the distance for our Hamiltonian cycle
    for (i = 0; i < n - 1; i++) {
        dist += sqrt((G[M[i]].x - G[M[i+1]].x)*(G[M[i]].x - G[M[i+1]].x) +
                (G[M[i]].y - G[M[i+1]].y)*(G[M[i]].y - G[M[i+1]].y));
    }
    dist += sqrt((G[M[0]].x - G[M[n-1]].x)*(G[M[0]].x - G[M[n-1]].x) +
            (G[M[0]].y - G[M[n-1]].y)*(G[M[0]].y - G[M[n-1]].y));

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "The Hamiltonian cycle of a relative minimum length " << std::endl;
    print_cycle(n, G, M);
    std::cout << "The relative minimum length is " << dist << std::endl;

    int microseconds = 
        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double seconds = microseconds / 1E6;
    std::cout << "elapsed time: " << seconds << " seconds" << std::endl;

    delete [] G;
    delete [] M;
    return 0;
}

//calculates two farthest points, returns 1 of them
int farthest_point(int n, point *P) {
    float max_dist = 0;
    float dist;
    int bestPoint = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dist = pow(P[i].x - P[j].x, 2) + pow(P[i].y - P[j].y, 2);
            if (max_dist < dist) {
                max_dist = dist;
                bestPoint = i;
            }
        }
    }
    return bestPoint;
}
    
int nearest(int n, point *P, int A, bool *Visited) {
//finds nearest point to P[A];
    int curr_dist;
    int nearest_dist = 0;
    int closest_point = A;
    bool Nearest_DistAssigned = false;
    for (int i = 0; i < n; i++) {
        if (i == A)
            continue;
        if (!Visited[i]) {
            curr_dist = sqrt(pow(P[A].x - P[i].x, 2) + pow(P[A].y - P[i].y, 2));
            if (!Nearest_DistAssigned) {
                nearest_dist = curr_dist;
                closest_point = i;
                Nearest_DistAssigned = true;
            }
            if (nearest_dist > curr_dist) {
                nearest_dist = curr_dist;
                closest_point = i;
            }
        }
    }
    return closest_point;
}
void print_cycle(int n, point* G, int* seq) {
    for (int i = 0; i < n; i++) {
        std::cout << "(" << G[seq[i]].x << "," << G[seq[i]].y << "), ";
    }
    std::cout << "(" << G[seq[0]].x << "," << G[seq[0]].y << ")";
    std::cout << std::endl;
}
