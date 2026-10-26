//This is CS315 project which implements Dijkstras akgorithm to find the shortest path and simulates packets routing from router to router.
// the Dijkstras algorithm was found online to help implementation. Using this link:https://www.geeksforgeeks.org/dsa/dijkstras-shortest-path-algorithm-greedy-algo-7/
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include "Packet.h"
#include "NetworkGraph.h"
using namespace std;

void simulatePackets() {
    
}

int main() {
    //main right now is a bunch of tests to make sure my algorithm works properly
    int src = 0;

    Network network(5);
    network.addEdge(0, 1, 4);
    network.addEdge(0, 2, 8);
    network.addEdge(1, 4, 6);
    network.addEdge(1, 2, 3);
    network.addEdge(2, 3, 2);
    network.addEdge(3, 4, 10);
    
    vector<int> router0Table = network.getNextHop(0);
    cout << "Next-Hop Routing Table for Router 0:\n";
    for (int i = 0; i < network.countVertices(); i++) {
        if (i == 0) continue;
        cout << "Destination " << i << " -> Next Hop: " << router0Table[i] <<endl;
    }
    return 0;
}
