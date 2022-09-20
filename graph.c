#include "graph.h"
#include "character.h"

/*
 * GRAPH
 */

struct Graph makeGraph() {
    struct Graph graph = { .length = 0 };
    // todo: FIXME
    return graph;
}

int addNode(struct Graph * graph, struct Node node) {
    // todo: FIXME
    return 0;
}

void addEdge(struct Graph * graph, int node1, int node2) {
    // todo: FIXME
}

struct Node * findNext(struct Graph * graph, const char * trigger) {
    // todo: FIXME
    return NULL;
}

struct Node * findEntry(struct Graph * graph) {
    // todo: FIXME
    return NULL;
}


/*
 * NODE SPECIFICS
 */

struct Node makeNode(enum nodeType t, const char * trigger, const char * say) {
    struct Node node = {
        .type = t,
        .trigger = trigger,
        .say = say,
    };
    // todo: FIXME
    return node;
}

const char * nextSay(struct Graph * graph, struct Character * player, struct Character * npc, const char * input) {
    // todo: FIXME
    return NULL;
}

const char ** getTriggers(struct Graph * graph) {
    // todo: FIXME
    return NULL;
}