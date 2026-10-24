//This is CS315 project which implements Dijkstras akgorithm to find the shortest path and simulates packets routing from router to router.
// the Dijkstras algorithm was found online to help implementation.
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;


vector<int> algorithm(vector<vector<pair<int,int>>>& adj, int src) {

    int size = adj.size(); //size of matrix

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; //priority queue min-heap

    vector<int> dist(size, INT_MAX); //distance from each node

    // Distance from source to itself is 0
    dist[src] = 0;
    pq.emplace(0, src); //push this onto the pq

    while (!pq.empty()) {
        auto top = pq.top(); //smallest distance
        pq.pop(); //remove it so it is not processed again

        int d = top.first; //recordered distance
        int u = top.second; //the index

        if (d > dist[u]) //this is lazy deletion (just skip it so its "deleted")
            continue;

        for (auto &p : adj[u]) { //loop through adjacent edges connected to the current vertex u
            int v = p.first; //neighboring vertex
            int w = p.second; //the weight (COST)

            // If we found a shorter path to v through u, update it
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;   
                pq.emplace(dist[v], v);
            }
        }
    }

    // Return the final shortest distances from the source
    return dist;
}

int main() {
    int src = 0;

    vector<vector<pair<int,int>>> adj(5);
    adj[0] = {{1,4}, {2,8}};
    adj[1] = {{0,4}, {4,6}, {2,3}};
    adj[2] = {{0,8}, {3,2}, {1,3}};
    adj[3] = {{2,2}, {4,10}};
    adj[4] = {{1,6}, {3,10}};

    vector<int> result = algorithm(adj, src);
    for (int i = 0; i < result.size(); i++) {
    cout << "Distance from source to vertex " << i << " is " << result[i] << endl;
}
    return 0;
}
