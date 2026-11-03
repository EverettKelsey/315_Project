// This is CS315 project which implements Dijkstras akgorithm to find the shortest path and simulates packets routing from router to router.
// the Dijkstras algorithm was found online to help implementation. Using this link:https://www.geeksforgeeks.org/dsa/dijkstras-shortest-path-algorithm-greedy-algo-7/
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include "Packet.h"
#include "NetworkGraph.h"
using namespace std;

// returns true if succesful and false if failed
bool simulatePackets(Network network, Packet package) {
    int cost = 0;
    vector<int> steps = {package.source};
    int ttlCount = package.ttl;
    
    // check if destination is source, if so success
    while (package.dest != steps.back()) {
        // check to make sure ttl isnt gone
        if (ttlCount-- <= 0) {
            cerr << "Package time to live has expired." << endl;
            return false; // failed delivery
        }
        
        // give out the best route to destination using the next hop fcn
        vector<int> hopOptions = network.getNextHop(steps.back());
        steps.push_back(hopOptions[package.dest]);
        
        // give out the cost of this route
        cost += network.getCount(steps[steps.size() - 2], steps.back());

        // if next hop is -1 then the packet was dropped
        if (steps.back() == -1) {
            cout << "Package was dropped." << endl;
            break;
        }
    }
    
    
    // printing best route
    for (int i = 0; i < steps.size(); i++) {
        cout << steps[i];
        
        if (i != steps.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
    
    // also give out the cost of this route
    cout << "Total cost of delivery: " << cost << endl;
    
    // outputing time in system
    cout << "Time in system: " << package.ttl - ttlCount << endl;
    cout << "Time left in package: " << ttlCount << endl;

    return true; // succesful delivery

    // if wanted we could use some imagery and draw the graph out either in terminal or some gui thing
}

void drawGraph(Network network) {
    // create a text file to store the ascii art
    ofstream graphFile("Graph.txt");
    if (!graphFile.is_open()) {
        cout << "Error in openning file!" << endl;
        return;
    }
    
    // create each node with each having a height of the number of nodes
    for (int i = 0; i < network.countVertices(); i++) {
        graphFile << " _______ " << endl;
        
        for (int j = 0; j < network.countVertices() - 2; j++) {
            if (j == network.countVertices() / 2) {
                int spaces = 5 - (i > 10 ? 1 : 0); 
                graphFile << "| " <<  i << string(spaces, ' ') << "|" << endl;
                continue;
            }
            
            graphFile << "|       |" << endl;
        }
        
        graphFile << "|_______|" << endl << endl;
    }
    
    graphFile.close();

    // ofstream graphFile("Graph.txt");
    // if (!graphFile.is_open()) {
    //     cout << "Error in openning file!" << endl;
    //     return;
    // }

    // // read file into string
    // string graph = readTextFile("Graph.txt");
    
    // for (int i = 0; i < network.countVertices(); i++) {
        
    // }
    
    // graphFile.close();

    // read text file and display node
}

string readTextFile(string fileName) {
    ifstream readTextFile(fileName);
    if (!readTextFile.is_open()) {
        cout << "Error in openning file!" << endl;
        return;
    }

    string fileRead = "";
    while (getline(readTextFile, graph));

    readTextFile.close();

    return fileRead;
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


    Packet package(1818, 0, 3, 1, "name");
    bool a = simulatePackets(network, package);
    
    vector<int> router0Table = network.getNextHop(0);
    cout << "Next-Hop Routing Table for Router 0:\n";
    for (int i = 0; i < network.countVertices(); i++) {
        if (i == 0) continue;
        cout << "Destination " << i << " -> Next Hop: " << router0Table[i] <<endl;
    }
    return 0;
}
