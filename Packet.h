#ifndef PACKET_H
#define PACKET_H
#include <string>

typedef struct Packet {
    int packetID;
    int dest; //destination
    int source;
    int ttl; //time-to-live (i think we could have user input decide this time)
    std::string payload; //message wanting to be passed

    Packet(int pID, int src, int dst, int ttl, std::string msg) : packetID(pID), source(src), dest(dst), ttl(ttl), payload(msg) {} //constructor for calling the packet
} Packet;

typedef struct Switch {
    int ports; // number of ports
    int MAC; //mac address will need to define a mac address table
} Switch;

#endif