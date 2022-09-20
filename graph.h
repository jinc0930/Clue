#ifndef GRAPH_H
#define GRAPH_H
#define GRAPH_SIZE 16
#include <stdio.h>
#include <stdlib.h>
#include "character.h"

// Trigger flags
enum {
  PIN            = 1 << 0,
  INACTIVE       = 1 << 1,
  END            = 1 << 2,
  UNPIN_LAST     = 1 << 3,
  INACTIVE_LAST  = 1 << 4,
};

// Types
enum nodeType {
  Trade,     // trade stuff (not necessarily a buy or sell, but gift to send or receive)
  Say,       // just say somenthing
  Success,   // if previous node succeeds
  NotEnough, // not enough money or item
};

// Node
struct Node {
  enum nodeType type;       // a enum to identify the type: `buy`, `receive...
  const char * trigger;     // a command to trigger this node. if null fallback to the nodeType.
  const char * say;         // text/speech of this node
  int amount_send;          // when this node is a `Trade` type, this will be the cost
  int amount_receive;       // when the character gives something to the player
  const char * send;        // when this node is a `Trade` type this will be the name of the item
  const char * receive;     // what item the player receives
  int  prob;                // probability of this node failing
  bool is_inactive;         // node is not accessible
  int trigger_flags;        // example: INACTIVE | END (this node will end and not be more accessible)
};

// adjacency list
struct List {
  int dest; // index of the node in the graph
  struct List* next; // connected to
};

// graph
struct Graph {
  // array of indexes and its linked lists (connections)
  struct List* list[GRAPH_SIZE];
  // actual nodes
  struct Node nodes[GRAPH_SIZE];
  int current; // current index in the graph
  int length; // how many nodes
  int pinned; // idx of a pinned node
};

/*
 * GRAPH IMPLEMENTATION
 */

// make a directed graph
struct Graph makeGraph();
// find the node with the trigger
struct Node * findNext(struct Graph * graph, const char * trigger);
// find the node pinned or the first
struct Node * findEntry(struct Graph * graph);
// connect node1 to nodeb in the graph
void addEdge(struct Graph * graph, int node1, int node2);
// add a node to the graph and returns a index which is where a node is connected
int addNode(struct Graph * graph, struct Node node);


/*
 * NODE SPECIFCS
 */

// return array of strings of the current triggers
const char ** getTriggers(struct Graph * graph);
// make a node with a simple initialization
struct Node makeNode(enum nodeType t, const char * trigger, const char * say);
// player buys somenthing
struct Node makeNodeBuy(const char * trigger, const char * say, int cost, const char * item);
// player trades somenthing
struct Node makeNodeTrade(const char * trigger, const char * say, const char * send, const char * item);
// get next npc speech
const char * nextSay(struct Graph * graph, struct Character * player, struct Character * npc, const char * input);
#endif