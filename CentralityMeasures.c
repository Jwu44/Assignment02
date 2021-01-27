// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"

static int reachableV(Graph g, Vertex src);
static double sumOfPaths(Graph g, Vertex src);
static int findV(PredNode *head, PredNode **arr, int s, int v, int t);
static double numPaths(PredNode *head, PredNode **arr, int s);

NodeValues closenessCentrality(Graph g) {
    NodeValues nvs = {0};
    nvs.numNodes = GraphNumVertices(g);
    int n = 0;
    double pathTotal = 0, N = nvs.numNodes;
    // Initialising closeness values for each vertex as zero.
    nvs.values = calloc(nvs.numNodes, sizeof(double));

    // Looping through each node to find its closeness value.
    for (int i = 0; i < nvs.numNodes; i++) {
        n = reachableV(g, i);
        pathTotal = sumOfPaths(g, i);
        // Continue if no reachable paths could be found.
        if (n == 0 || pathTotal == 0) {
            continue;
        }
        // Applying formula.
        nvs.values[i] = ((n - 1)/(N - 1)) * ((n - 1)/(pathTotal));
    }
    return nvs;
}

NodeValues betweennessCentrality(Graph g) {
    NodeValues nvs = {0};
    int appearances = 0;
    double pathCount = 0;
    nvs.numNodes = GraphNumVertices(g);
    // Initialising betwenness values for each vertex as zero.
    nvs.values = calloc(nvs.numNodes, sizeof(double));

    // Finding the betweenness for each node 'v'.
    for (int v = 0; v < nvs.numNodes; v++) {
        // Search for v for every path between s-t.
        for (int s = 0; s < nvs.numNodes; s++) {
            // Find every path from s-t
            ShortestPaths paths = dijkstra(g, s);
            // Go over each shortest path s-t.
            for (int t = 0; t < nvs.numNodes; t++) {
                appearances = findV(paths.pred[t], paths.pred, s, v , t);
                // Only calculate betweenness if the node actually appears in pred array.
                if (appearances > 0) {
                    pathCount = numPaths(paths.pred[t], paths.pred, s);
                    nvs.values[v] += appearances/pathCount;
                }
            }
        }
    }
    return nvs;
}

NodeValues betweennessCentralityNormalised(Graph g) {
    NodeValues nvs = {0};
    nvs.numNodes = GraphNumVertices(g);
    // Initialising betwenness values for each vertex as zero.
    nvs.values = calloc(nvs.numNodes, sizeof(double));
	int n = nvs.numNodes;
    // Setting denom beforehand to get the right normalised value.
    double denom = (n-1)*(n-2);
    
    // Looping through each node to find its normalised betweenness value.
    for (int i = 0; i < nvs.numNodes; i++) {
        // Applying formula.
        NodeValues gV = betweennessCentrality(g);
        nvs.values[i] = (1/denom)*gV.values[i];
    }
    
    return nvs;
}

// Printing node values.
void showNodeValues(NodeValues nvs) {
    for (int i = 0; i < nvs.numNodes; i++) {
        printf("%d: %lf", i, nvs.values[i]);
        printf("\n");
    }

}

void freeNodeValues(NodeValues nvs) {
    free(nvs.values);
}

// Returns the number of vertices src node can reach.
static int reachableV(Graph g, Vertex src) {
    ShortestPaths paths = dijkstra(g, src);
    int reachable = 0;
    int totalV = paths.numNodes;
    for (int i = 0; i < totalV; i++) {
        // Go to next array element if current distance is zero.
        if (paths.dist[i] == 0)
            continue;
        else
            reachable++;
    }
    // Because source node is also reachable, increment it once again.
    reachable++;
    return reachable;
}

// Sums the lengths of the shortest paths from src node to every other node.
static double sumOfPaths(Graph g, Vertex src) {
    ShortestPaths paths = dijkstra(g, src);
    int totalV = paths.numNodes;
    double sum = 0;
    for (int i = 0; i < totalV; i++) {
        sum += paths.dist[i];
    }
    return sum;
}

// Recursive function that detects whether 'v' appears in pred array.
// Analyses the pred of a pred and so on...
static int findV(PredNode *head, PredNode **arr, int s, int v, int t) {
    if (head == NULL)
        return 0;
    // Check if 'v' value in the linked list is equal to s or t.
    if (head->v == s || head->v == t) 
        return findV(head->next, arr, s, v, t) + findV(arr[head->v], arr, s, v, t);
    // Check if v is in pred array and also check deeper in the linked list.
    if (head->v == v) 
        return 1 + findV(head->next, arr, s, v, t) + findV(arr[head->v], arr, s, v, t);
    // In previous cells of arr.
    else {
        if (head->next != NULL)
            return findV(arr[head->v], arr, s, v, t) + findV(head->next, arr, s, v, t);
        // Head->next is NULL.
        else 
            return findV(arr[head->v], arr, s, v, t);
    }
}

// Recursive function that detects the total number of paths that 'v' appears in for each pair s-t.
static double numPaths(PredNode *head, PredNode **arr, int s) {
    if (head == NULL)
        return 0;
    if (head->next != NULL)
        return 1 + numPaths(head->next, arr, s);
    if (head->v == s)
        return 1 + numPaths(arr[head->v], arr, s);
    // Head->next is NULL.
    return numPaths(arr[head->v], arr, s);
}