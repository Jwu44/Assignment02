// Dijkstra ADT implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "PQ.h"

#define MAX 100

static PredNode *newList(Vertex v);
static void ListAppend(PredNode *l, Vertex newV);
static void freeList(PredNode *l);

ShortestPaths dijkstra(Graph g, Vertex src) {
	ShortestPaths sps = {0};
	sps.numNodes = GraphNumVertices(g);
	// Dynamically allocating memory for arrays.
	sps.dist = (int *)malloc(sizeof(int)*MAX*sps.numNodes);
	sps.pred = (PredNode **)malloc(sizeof(int)*MAX*sps.numNodes);
	sps.src = src;

	// Initialising all distances to infinity and creating a new Node for each predecessor.
	for (int i = 0; i < sps.numNodes; i++) {
		sps.dist[i] = __INT_MAX__;
		sps.pred[i] = newList(src);
	}	

	// Distance to source is zero.
	sps.dist[src] = 0;
	
	// Create an empty priority queue.
	PQ pq = PQNew();
	// Priority = weight. xd
	// Item = vertex node.
	PQInsert(pq, src, 0);
	while (!PQIsEmpty(pq)) {
		// Min represents vertex with smallest priority value or the shortest distance to source.
		Vertex min = PQDequeue(pq);
		// Setting pred to NULL & dist to zero if scanned vertex is the source.
		if (min == sps.src ) {
			sps.pred[min] = NULL;
			sps.dist[min] = 0;
		}
		// Get a list of edges coming out from min.
		AdjList out = GraphOutIncident(g, min);
		// Loop through all edges coming out from min.
		for (; out != NULL; out = out->next) {
			// Relax the edge.
			if (sps.dist[min] != __INT_MAX__ && sps.dist[min] + out->weight < sps.dist[out->v]) {
				sps.dist[out->v] = sps.dist[min] + out->weight;
				// Need to clear pred[] before inserting new min as we may have found a new path.
				sps.pred[out->v] = newList(min);
				PQInsert(pq, out->v, sps.dist[out->v]);
				continue;
			}
			// Identical shortest paths.
			if (sps.dist[min] != __INT_MAX__ && sps.dist[min] + out->weight == sps.dist[out->v]) {
				PredNode *head = sps.pred[out->v];
				// Append vertex to the end of the linked list & insert vertex into PQ.
				ListAppend(head, min);
				PQInsert(pq, out->v, sps.dist[out->v]); 
			}
		}
	}
	// For unreachable vertices, set dist to 0 and pred to NULL.
	for (int i = 0; i < sps.numNodes; i++) {
		if (sps.dist[i] == __INT_MAX__) {
			sps.dist[i] = 0;
			sps.pred[i] = NULL;
		}
	}
	return sps;
}

void showShortestPaths(ShortestPaths sps) {

}

void freeShortestPaths(ShortestPaths sps) {
	free(sps.dist);
	for (int i = 0; i < sps.numNodes; i++) {
		freeList(sps.pred[i]);
	}
	free(sps.pred);
}

// Creates a new empty list.
static PredNode *newList(Vertex v) {
	PredNode *l = malloc(sizeof(PredNode));
	l->v = v;
	l->next = NULL;
	return l;
}

// Adds vertex to the end of pred array list.
static void ListAppend(PredNode *l, Vertex newV) {
	PredNode *curr = l;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = newList(newV);
}

// Frees nodes in pred array.
static void freeList(PredNode *l) {
	PredNode *temp;
	while (l != NULL) {
		temp = l;
		l = l->next;
		free(temp);
	}
}
