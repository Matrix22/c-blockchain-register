#include "./include/blockdag.h"

/**
 * @brief An util function to check if a graph has cycles
 * 
 * @param graph an allocated graph
 * @param visited node visited vector
 * @param start start node
 * @param current current node of traversion
 * @return uint8_t a boolean value 1 for finding cycles
 * and 0 otherwise
 */
static uint8_t graph_has_cycle_util(graph_t *graph, uint8_t *visited, int32_t start, int32_t current) {
    if ((visited[start] == 1) && (start == current)) {
        return 1;
    }

    if (visited[current] == 1) {
        return 0;
    }

    visited[current] = 1;

    graph_node_t *vex = graph->adj_list[current];
    
    uint8_t has_cycle = 0;

    while ((vex != NULL) && (has_cycle == 0)) {
        int32_t vex_idx = get_node_index_id(graph, vex->name);

        has_cycle = graph_has_cycle_util(graph, visited, start, vex_idx);

        vex = vex->next;
    }

    return has_cycle;
}

/**
 * @brief Function to check if a graph has cycles
 * 
 * @param graph an allocated graph
 * @return uint8_t a boolean value 1 for cycles
 * and 0 otherway
 */
static uint8_t graph_has_cycle(graph_t *graph) {
    if ((graph == NULL) || (graph->adj_list == NULL) || (graph->index_map == NULL)) {
        return 1;
    }

    uint8_t *visited = malloc(sizeof (*visited) * graph->vex_nr);

    if (visited != NULL) { 
        uint8_t has_cycle = 0;

        for (int32_t iter = 0; (iter < graph->vex_nr) && (has_cycle == 0); ++iter) {
            for (int32_t vis = 0; vis < graph->vex_nr; ++vis) {
                visited[vis] = 0;
            }
            
            has_cycle = graph_has_cycle_util(graph, visited, iter, iter);
        }

        free(visited);

        return has_cycle;
    }

    return 1;
}

/**
 * @brief Main function to check if a graph is a valid dag
 * 
 */
void check_valid_dag(void) {
    graph_t *graph = create_graph();

    if (graph == NULL) {
        perror("Couldn't allocate graph");
        exit(EXIT_FAILURE);
    }
    
    uint8_t check_cycle = graph_has_cycle(graph);
    
    FILE *fout = NULL;

    if ((fout = fopen("blockdag.out", "w")) == NULL) {
        free_graph(graph);

        perror("Couldn't open file for writing");
        exit(EXIT_FAILURE);
    }

    if (check_cycle >= 1) {
        fprintf(fout, "impossible\n");    
    } else if (check_cycle == 0) {
        fprintf(fout, "correct\n");
    } else {
        perror("Couldn't check for cycles");
    }

    fclose(fout);
    free_graph(graph);
}