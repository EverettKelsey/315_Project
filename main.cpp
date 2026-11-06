// This is CS315 project which implements Dijkstras akgorithm to find the shortest path and simulates packets routing from router to router.
// the Dijkstras algorithm was found online to help implementation. Using this link:https://www.geeksforgeeks.org/dsa/dijkstras-shortest-path-algorithm-greedy-algo-7/
#include <iostream>
#include <fstream>
#include <vector>
#include "Packet.h"
#include "NetworkGraph.h"
using namespace std;

void drawGraph(Network network, int source, int destination) {
    // create a text file to store the ascii art
    ofstream graphFile("Graph.txt", ios::app);
    if (!graphFile.is_open()) {
        cerr << "Error in openning file!" << endl;
        return;
    }
    
    // create each node with each having a height of the number of nodes
    graphFile << " _______ " << endl;
    graphFile << "|       |" << endl;

    int spaces = 3 - (source >= 10 ? 1 : 0); 
    graphFile << "|   " <<  source << string(spaces, ' ') << "|" << endl;
    
    graphFile << "|_______|" << endl;
    
    // getting the weight between the 2 nodes
    int weight = network.getCount(source, destination);
    
    if (weight != -100) {
        graphFile << "    |" << endl;
        graphFile << "    | " <<  weight << endl;
        graphFile << "    |" << endl;
        graphFile << "    v" << endl;
    }
    
    graphFile.close();

    // read text file and display node
}

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
    
    // resetting text file
    ofstream graphFile("Graph.txt");
    if (!graphFile.is_open()) {
        cerr << "Error in openning file!" << endl;
        return false;
    }
    graphFile << "Fastest route to destination:" << endl;
    graphFile.close();
    
    // printing best route
    for (int i = 0; i < steps.size(); i++) {
        drawGraph(network, steps[i], steps[i + 1]);
    }
    cout << endl;
    
    // also give out the cost of this route
    cout << "Total cost of delivery: " << cost << endl;
    
    // outputing time in system
    cout << "Time in system: " << package.ttl - ttlCount << endl;
    cout << "Time left in package: " << ttlCount << endl;

    return true; // succesful delivery
}

int main() {
    //main right now is a bunch of tests to make sure my algorithm works properly
    int src = 0;

    Network network(0); // size gets set by loadNetwork
    Packet package = network.loadNetwork("network.txt"); // asks user for network + packet
    bool a = simulatePackets(network, package);

    vector<int> router0Table = network.getNextHop(0);
    cout << "Next-Hop Routing Table for Router 0:\n";
    return 0;
}