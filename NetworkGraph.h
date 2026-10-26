#ifndef NETWORK_GRAPH_H
#define NETWORK_GRAPH_H
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
using namespace std;

class Network {
    private:
        int V; //number of routers
        vector<vector<pair<int,int>>> adj; //adjacency list
    public:
        //Constuctor
        Network(int vertices) {
            V = vertices;
            adj.resize(V);
        }

        void addEdge(int u, int v, int weight); //adds an edge to adj
        int countVertices() const; //returns V so we know router amount;
        vector<int> getNextHop(int src); //getNextHop fcn using Dijkstras
        void loadNetwork(string filename); //James' part where the user input decides the topology of the network
};

void Network::addEdge(int u, int v, int weight) {
    adj[u].push_back({v,weight});
    adj[v].push_back({u,weight}); //we are assuming undirected network lines here
}

int Network::countVertices() const {
    return V;
}

vector<int> Network::getNextHop(int src) {
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq; //establishes priority queue as a min heap
    vector<int> dist(V, INT_MAX);
    vector<int> parent(V, -1);

    dist[src] = 0; //distance from itself is 0
    pq.emplace(0,src); //push the source into the pq

    while(!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        int d = top.first;
        int u = top.second;

        if(d > dist[u]) continue;
        for (auto &p : adj[u]) { //loop through adjacent edges connected to the current vertex u
            int v = p.first; //neighboring vertex
            int w = p.second; //the weight (COST)

            // If we found a shorter path to v through u, update it
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w; 
                parent[v] = u; //record preceding node  
                pq.emplace(dist[v], v); //update
            }
        }
    }

    vector<int> nextHop(V,-1);
    for(int i = 0; i < V; ++i) { //looping through every destination
        if(i == src) continue; //skips source

        int curr = i;
        while(src != parent[curr] && parent[curr] != -1) { //look at the parents until the node we are looking at was reached from our source
            curr = parent[curr];
        }
        if(parent[curr] == src) {
            nextHop[i] = curr; //store the next hop destination (curr is looking at what node needs to be hit next to reach dest i)
        }
    }
    // Return the next hop to get to location
    return nextHop;

}


void Network::loadNetwork(string filename) {
    //INSERT CODE HERE
    //James can do this one
    //have user input write to the network.txt file and then put the values from that file into the adj list
}

#endif