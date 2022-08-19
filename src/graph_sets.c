#include "./include/blockdag.h"

/**
 * @brief Function to find all past nodes
 * bassically it does and bfs for a selected node
 * and returns it's path
 * 
 * @param graph base graph
 * @param vex index node to find past nodes
 * @return ord_list_t* a list with all past nodes
 * sorted lexicografically
 */
ord_list_t* past(graph_t *graph, int32_t vex) {
    if ((graph == NULL) || (graph->adj_list == NULL) || (graph->index_map == NULL)) {
        return NULL;
    }

    ord_list_t *path = NULL;

	queue_t *bfs_queue = create_queue();

	if (bfs_queue != NULL) {
        int32_t start_vex = vex;

		uint8_t *visited = malloc(sizeof(*visited) * graph->vex_nr);

		if (visited != NULL) {
			for (int32_t iter = 0; iter < graph->vex_nr; ++iter) {
				visited[iter] = 0;
            }

			visited[vex] = 1;

			enqueue(bfs_queue, vex);

			while (!is_queue_empty(bfs_queue)) {
				vex = front(bfs_queue);

                if (vex != start_vex) {
				    path = insert_ord_list(path, get_node_value(graph, vex));
                }

				dequeue(bfs_queue);

                graph_node_t *iter_vex = graph->adj_list[vex];

				while (iter_vex != NULL) {
                    int32_t vex_idx = get_node_index_id(graph, iter_vex->name);

					if (visited[vex_idx] == 0) {
						visited[vex_idx] = 1;

						enqueue(bfs_queue, vex_idx);
					}

					iter_vex = iter_vex->next;
				}
			}

			free(visited);
		}

		free_queue(bfs_queue);
	}

	return path;
}

/**
 * @brief Function that computes all future nodes
 * for a desired node, bassicaly future is past revered
 * so we will transpose our initial graph and will
 * compute its past nodes
 * 
 * @param graph initial graph
 * @param vex node to find future list
 * @return ord_list_t* future list of the node
 */
ord_list_t* future(graph_t *graph, int32_t vex) {
    if ((graph == NULL) || (graph->adj_list == NULL) || (graph->index_map == NULL)) {
        return NULL;
    }

    graph_t *trans_graph = create_transpose_graph(graph);

    ord_list_t *path = past(trans_graph, vex);

    free_graph(trans_graph);

    return path;
}

/**
 * @brief Function that computes anticone list
 * of a desired node
 * 
 * @param graph initial graph
 * @param past_list past list of the exact same node
 * @param future_list future list of the exact same node
 * @param vex index of node that we want to compute
 * anticone list
 * @return ord_list_t* anticone list object
 */
ord_list_t* anticone(graph_t *graph, ord_list_t *past_list, ord_list_t *future_list, int32_t vex) {
    if ((graph == NULL) || (graph->adj_list == NULL) || (graph->index_map == NULL)) {
        return NULL;
    }

    ord_list_t *path = NULL;

    for (int32_t iter = 0; iter < graph->vex_nr; ++iter) {
        char *vex_name = get_node_value(graph, iter);

        if ((!ord_list_contains(past_list, vex_name)) && (!ord_list_contains(future_list, vex_name)) && (vex != iter)) {
            path = insert_ord_list(path, vex_name);
        }
    }

    return path;
}

/**
 * @brief Function that computes graph tips list
 * 
 * @param graph initial graph
 * @return ord_list_t* tips list
 */
ord_list_t* tips(graph_t *graph) {
    if ((graph == NULL) || (graph->adj_list == NULL) || (graph->index_map == NULL)) {
        return NULL;
    }

    ord_list_t *path = NULL;

    int32_t *node_deg = malloc(sizeof(*node_deg) * graph->vex_nr);

    if (node_deg != NULL) {
        for (int32_t iter = 0; iter < graph->vex_nr; ++iter) {
            node_deg[iter] = 0;
        }

        for (int32_t iter = 0; iter < graph->vex_nr; ++iter) {
            graph_node_t *vex = graph->adj_list[iter];

            while (vex != NULL) {
                int32_t vex_idx = get_node_index_id(graph, vex->name);
                ++node_deg[vex_idx];
                vex = vex->next;
            }
        }

        for (int32_t iter = 0; iter < graph->vex_nr; ++iter) {
            if (node_deg[iter] == 0) {
                path = insert_ord_list(path, get_node_value(graph, iter));
            }
        }

        free(node_deg);
    }

    return path;
}

/**
 * @brief Function to find graph sets
 * 
 * @param vex_name Desired node name to find all answers
 */
void graph_sets(const char *vex_name) {
    graph_t *graph = create_graph();

    if (graph == NULL) {
        perror("Couldn't allocate graph");
        exit(EXIT_FAILURE);
    }

    int32_t vex_idx = get_node_index_id(graph, vex_name);

    if (vex_idx <= -1) {
        free_graph(graph);

        return;
    }

    ord_list_t *past_list = past(graph, vex_idx);
    ord_list_t *future_list = future(graph, vex_idx);
    ord_list_t *anticone_list = anticone(graph, past_list, future_list, vex_idx);
    ord_list_t *tips_list = tips(graph);

    FILE *fout = NULL;
    if ((fout = fopen("blockdag.out", "w")) == NULL) {
        perror("Couldn't open file for writing");
        exit(EXIT_FAILURE);
    }

    fprintf(fout, "past(%s) : ", vex_name);
    print_ord_list(past_list, fout);
    free_ordered_list(past_list);

    fprintf(fout, "future(%s) : ", vex_name);
    print_ord_list(future_list, fout);
    free_ordered_list(future_list);

    fprintf(fout, "anticone(%s) : ", vex_name);
    print_ord_list(anticone_list, fout);
    free_ordered_list(anticone_list);

    fprintf(fout, "tips(G) : ");
    print_ord_list(tips_list, fout);
    free_ordered_list(tips_list);

    fclose(fout);
    free_graph(graph);
}