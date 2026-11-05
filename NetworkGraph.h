#ifndef NETWORK_GRAPH_H
#define NETWORK_GRAPH_H
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include "Packet.h"
using namespace std;

// keeps asking until the user types a whole number between minVal and maxVal
int readInt(string prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal && value <= maxVal) {
            return value; // good input
        }
        if (cin.eof()) { // input ran out (Ctrl+D)
            cout << "\nNo more input, exiting." << endl;
            exit(1);
        }
        cout << "Invalid input. Enter a whole number from " << minVal << " to " << maxVal << "." << endl;
        cin.clear(); // reset the fail state
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // throw away the bad line
    }
}

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
        Packet loadNetwork(string filename); //James' part where the user input decides the topology of the network
        int getCount(int source, int destination); // returns the weight between 2 networks
};

void Network::addEdge(int u, int v, int weight) {
    adj[u].push_back({v, weight});
    adj[v].push_back({u, weight}); //we are assuming undirected network lines here
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

Packet Network::loadNetwork(string filename) {
    // ---- Part 1: ask the user for the network ----
    int routers = readInt("Enter number of routers: ", 1, 100);
    int edges = readInt("Enter number of connections: ", 0, routers * (routers - 1) / 2);

    // ---- Part 2: write it to the file ----
    {
        ofstream out(filename);
        if (!out) {
            cout << "Error: could not write " << filename << endl;
            exit(1);
        }
        out << "# Number of Routers" << endl;
        out << routers << endl;
        out << "# Edges: Source, Destination, Weight (Latency in ms)" << endl;
        for (int i = 0; i < edges; i++) {
            cout << "Connection " << i + 1 << ":" << endl;
            int u = readInt("  Router A: ", 0, routers - 1);
            int v = readInt("  Router B: ", 0, routers - 1);
            int w = readInt("  Latency (ms): ", 1, 10000);
            out << u << " " << v << " " << w << endl;
        }
    } // out closes here so the file is fully saved before we read it

    // ---- Part 3: read the file back into the graph ----
    ifstream in(filename); //open the file to read in the network topology
    if (!in) { //error checking to see if the file opened correctly
        cout << "Error: could not open " << filename << endl;
        exit(1);
    }
    string line;
    int lineNum = 0;
    bool gotCount = false;

    while (getline(in, line)) {
        lineNum++;
        if (line.empty() || line[0] == '#') continue; //skip empty lines and comments

        stringstream ss(line); // to >> numbers out of the line

        if (!gotCount) {
            ss >> V; //first line is the number of routers
            adj.assign(V, {});
            gotCount = true;
            cout << "Number of Routers: " << V << endl;
        } else {
            int u, v, w; // variables to hold the edge and weight
            ss >> u >> v >> w; //read in the edge and weight
            addEdge(u, v, w); //add the edge to the adjacency list
            cout << "Added edge: " << u << " <--(" << w << ")--> " << v << endl; //print out the edge that was added
        }
    }

    // ---- Part 4: ask for the packet and return it ----
    cout << "\nPacket setup:" << endl;
    int src = readInt("  Source router: ", 0, V - 1);
    int dst = readInt("  Destination router: ", 0, V - 1);
    int ttl = readInt("  TTL (max hops): ", 1, 255);

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the leftover newline before getline
    string msg;
    cout << "  Message: ";
    getline(cin, msg);

    return Packet(1, src, dst, ttl, msg);
}

int Network::getCount(int source, int destination) {
    for (const auto& p : adj[source]) {
        if (p.first == destination) {
            return p.second;
        }
    }

    return 0;
}

#endif
