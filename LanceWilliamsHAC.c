// Lance-Williams Algorithm for Hierarchical Agglomerative Clustering
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "LanceWilliamsHAC.h"

static Dendrogram newDend(int v);

/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    int numV = GraphNumVertices(g);
    int row, col;
    double min = __INT_MAX__;
    double max = 0;
    // Allocating memory dynamically for structures to be utilised:
    // dist is an array of size numV x numV that represents the max dist between vertex i & j.
    // Reference: https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/ 
    double **dist = (double **)malloc(sizeof(double *)*numV);
    for (int i = 0; i < numV; i++) {
        dist[i] = (double *)malloc(sizeof(double)*numV);
        // out represents a list of vertices coing out from vertex i.
        AdjList out = GraphOutIncident(g, i);
        for (; out != NULL; out = out->next) {
            // Populating dist array with edge weights
            dist[i][out->v] = 1/out->weight;
        }
        // If edge weight could not be found, set dist to infinity.
        for (int j = 0; j < numV; j++) {
            if (!GraphIsAdjacent(g, i, j))
                dist[i][j] = __INT_MAX__;
        }
    }
    // dendA is an array of size numV.
    Dendrogram *dendA = (Dendrogram *)malloc(sizeof(DNode)*numV);
    // Each cell of the array stores a pointer to 1 dendogram with a vertex in it.
    for (int x = 0; x < numV; x++) {
        dendA[x] = newDend(x);
    }

    // Single linkage
    if (method == 1) {
        // For k = 1 to N - 1
        for (int k = 1; k < numV - 1; k++) {
            // Search for the closest distance between clusters in the dist array
            for (int l = 0; l < numV; l++) {
                for (int m = 0; m < numV; m++) {
                    // Updating min.
                    if (dist[l][m] < min) {
                        min = dist[l][m];
                        // Saving the coordinates of where the min occurs.
                        row = l;
                        col = m;
                    }
                }
            }
            // Remove the clusters ci & cj from dendA.
            for (int n = 0; n < numV; n++) {
                if (dendA[n]->vertex == row || dendA[n]->vertex == col)
                    dendA[n] = NULL;
            }
            // Merging ci & cj & add it to the dendA array.
            for (int empty = 0; empty < numV; empty++) {
                // Inserting cluster into an empty dendA slot.
                if (dendA[empty] == NULL) {
                    dendA[empty] = newDend(empty);
                    dendA[empty]->left = newDend(col);
                    dendA[empty]->right = newDend(row);
                }
            }
            // Updating the dist array but setting all the entries in 'row' & 'col' to inf.
            for (int p = 0; p < numV; p++) { 
                dist[row][p] = __INT_MAX__;
                dist[p][col] = __INT_MAX__;
            }
        }
    }

    // Complete linkage
    if (method == 2) {
         // For k = 1 to N - 1
        for (int k = 1; k < numV - 1; k++) {
            // Search for the closest distance between clusters in the dist array
            for (int l = 0; l < numV; l++) {
                for (int m = 0; m < numV; m++) {
                    // Updating max.
                    if (dist[l][m] > max) {
                        max = dist[l][m];
                        // Saving the coordinates of where the max occurs.
                        row = l;
                        col = m;
                    }
                }
            }
            // Remove the clusters ci & cj from dendA.
            for (int n = 0; n < numV; n++) {
                if (dendA[n]->vertex == row || dendA[n]->vertex == col)
                    dendA[n] = NULL;
            }
            // Merging ci & cj & add it to the dendA array.
            for (int empty = 0; empty < numV; empty++) {
                // Inserting cluster into an empty dendA slot.
                if (dendA[empty] == NULL) {
                    dendA[empty] = newDend(empty);
                    dendA[empty]->left = newDend(col);
                    dendA[empty]->right = newDend(row);
                }
            }
            // Updating the dist array but setting all the entries in 'row' & 'col' to inf.
            for (int p = 0; p < numV; p++) { 
                dist[row][p] = __INT_MAX__;
                dist[p][col] = __INT_MAX__;
            }
        }
    }
    return dendA[1];
}

/**
 * Frees all memory associated with the given Dendrogram structure.
 */
void freeDendrogram(Dendrogram d) {
    if (d != NULL) {
        freeDendrogram(d->left);
        freeDendrogram(d->right);
        free(d);
    }
}

// Creates a new dendogram.
static Dendrogram newDend(int v) {
    Dendrogram d = malloc(sizeof(struct DNode));
    d->vertex = v;
    d->left = NULL;
    d->right = NULL;
    return d;
}
