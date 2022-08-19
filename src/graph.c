#include "./include/blockdag.h"

/**
 * @brief Function to create a index map (names of the nodes) for a specific
 * graph.
 * 
 * @param vex_nr number of vertices to map 
 * @param buffer a line containing all nodes name
 * @return char** a index map of all nodes
 */
char** create_index_map(int32_t vex_nr, char *buffer) {
    char **index_map = malloc(sizeof(*index_map) * vex_nr);

    if (index_map != NULL) {
        char *word = NULL, *save_ptr = NULL;

        word = __strtok_r(buffer, DELIM_OPER, &save_ptr);

        int32_t iter = 0;

        while ((word != NULL) && (iter < vex_nr)) {
            index_map[iter] = malloc(MAX_NODE_LEN);

            snprintf(index_map[iter++], MAX_NODE_LEN, "%s", word);

            word = __strtok_r(NULL, DELIM_OPER, &save_ptr);
        }
    }

    return index_map;
}

/**
 * @brief Function to create metadata for a graph object
 * 
 * @param vex_nr number of vertices from the graph
 * @param buffer a line containing all nodes name
 * @return graph_t* pointer to an allocated graph object or NULL
 */
graph_t* create_graph_adj_list(int32_t vex_nr, char *buffer) {
	graph_t *new_graph = malloc(sizeof(*new_graph));

	if (new_graph != NULL) {
		new_graph->vex_nr = vex_nr;
        new_graph->index_map = create_index_map(vex_nr, buffer);

        if (new_graph->index_map == NULL) {
            free(new_graph);

            return NULL;
        }

		new_graph->adj_list = malloc(sizeof (*new_graph->adj_list) * new_graph->vex_nr);

		if (new_graph->adj_list == NULL) {
            for (int32_t iter = 0; iter < new_graph->vex_nr; ++iter) {
                if (new_graph->index_map[iter] != NULL) {
                    free(new_graph->index_map[iter]);
                    new_graph->index_map[iter] = NULL;
                }

                free(new_graph->index_map);
                new_graph->index_map = NULL;

                free(new_graph);
            }

            return NULL;
        }
			
		for (int32_t iter = 0; iter < new_graph->vex_nr; ++iter) {
			new_graph->adj_list[iter] = NULL;
		}
	}

	return new_graph;
}

/**
 * @brief Get the node index from index map
 * 
 * @param graph pointer to graph object
 * @param node_name name of the wanted node
 * @return int32_t the index in the mapping array
 */
int32_t get_node_index_id(graph_t *graph, const char *node_name) {
    if ((graph == NULL) || (graph->index_map == NULL) || (node_name == NULL)) {
        return -1;
    }
    
    for (int32_t iter = 0; iter < graph->vex_nr; ++iter) {
        if (strncmp(graph->index_map[iter], node_name, MAX_NODE_LEN) == 0) {
            return iter;
        }
    }

    return -1;
}

/**
 * @brief Get the node name from his index
 * 
 * @param graph pointer to graph object
 * @param vex_idx index of the node
 * @return char* the name of the node
 */
char* get_node_value(graph_t *graph, int32_t vex_idx) {
    if ((graph == NULL) || (graph->index_map == NULL) || (vex_idx >= graph->vex_nr)) {
        return NULL;
    }

    return graph->index_map[vex_idx];
}

/**
 * @brief Function to add an edge between VERTEX1 --> VERTEX2
 * 
 * @param graph pointer to graph object
 * @param vex1 name of the first node
 * @param vex2 name of the second node
 */
void add_edge_list(graph_t *graph, const char *vex1, const char *vex2) {
	if ((graph == NULL) || (graph->adj_list == NULL) || (graph->index_map == NULL) || (vex1 == NULL) || (vex2 == NULL)) {
        return;
    }

	graph_node_t *node_vex2 = malloc(sizeof(*node_vex2));

	if (node_vex2 != NULL) {
		int32_t vex1_idx = get_node_index_id(graph, vex1);

        if (vex1_idx == -1) {
            free(node_vex2);

            return;
        }

        node_vex2->name = malloc(MAX_NODE_LEN);

        if (node_vex2->name == NULL) {
            free(node_vex2);

            return;
        }

        snprintf(node_vex2->name, MAX_NODE_LEN, "%s", vex2);
		node_vex2->next = NULL;

		if (graph->adj_list[vex1_idx] != NULL) {
			node_vex2->next = graph->adj_list[vex1_idx];
			graph->adj_list[vex1_idx] = node_vex2;
		} else {
			graph->adj_list[vex1_idx] = node_vex2;
		}
	}
}

/**
 * @brief Function to create a graph object froma file
 * 
 * @return graph_t* pointer to an allocated graph object or NULL
 */
graph_t* create_graph(void) {
    FILE *fin = NULL;

    if ((fin = fopen("blockdag.in", "r")) == NULL) {
        perror("Couldn't open file for reading");
        exit(EXIT_FAILURE);
    }

    char *line_buff = malloc(MAX_LINE_LEN);

    if (fgets(line_buff, MAX_LINE_LEN, fin) == NULL) {
        perror("Line was not read correctly");
    }

    int32_t vex_nr = atoi(line_buff);

    if (fgets(line_buff, MAX_LINE_LEN, fin) == NULL) {
        perror("Line was not read correctly");
    }

    graph_t *graph = create_graph_adj_list(vex_nr, line_buff);

    if (graph == NULL) {
        free(line_buff);

        return NULL;
    }
    
    if (fgets(line_buff, MAX_LINE_LEN, fin) == NULL) {
        perror("Line was not read correctly");
    }

    while (fgets(line_buff, MAX_LINE_LEN, fin) != NULL) {
        char *save_ptr = NULL;

        char *vex1 = __strtok_r(line_buff, DELIM_OPER, &save_ptr);
        char *vex2 = __strtok_r(NULL, DELIM_OPER, &save_ptr);
        
        while (vex2 != NULL) {
            add_edge_list(graph, vex1, vex2);

            vex2 = __strtok_r(NULL, DELIM_OPER, &save_ptr);
        }
    }

    free(line_buff);
    fclose(fin);

    return graph;
}

/**
 * @brief Function to create the transposed graph object
 * 
 * @param graph pointer to a graph object location
 * @return graph_t* pointer to the transposed graph or NULL
 */
graph_t* create_transpose_graph(graph_t *graph) {
    if ((graph == NULL) || (graph->index_map == NULL) || (graph->adj_list == NULL)) {
        return NULL;
    }

    FILE *fin = NULL;

    if ((fin = fopen("blockdag.in", "r")) == NULL) {
        perror("Couldn't open file for reading");
        exit(EXIT_FAILURE);
    }

    char *line_buff = malloc(MAX_LINE_LEN);

    if (fgets(line_buff, MAX_LINE_LEN, fin) == NULL) {
        perror("Line was not read correctly");
    }

    if (fgets(line_buff, MAX_LINE_LEN, fin) == NULL) {
        perror("Line was not read correctly");
    }

    fclose(fin);

    graph_t *trans_graph = create_graph_adj_list(graph->vex_nr, line_buff);

    free(line_buff);

    if (trans_graph == NULL) {
        return NULL;
    }

    for (int32_t iter = 0; iter < graph->vex_nr; ++iter) {
        graph_node_t *vex = graph->adj_list[iter];

        while (vex != NULL) {
            add_edge_list(trans_graph, vex->name, get_node_value(graph, iter));

            vex = vex->next;
        }
    }

    return trans_graph;
}

/**
 * @brief Subroutine function to delete a linked list of vertices
 * 
 * @param start_vex head of the vertices list
 */
static void free_vertices(graph_node_t *start_vex) {
    while (start_vex != NULL) {
        graph_node_t *del_vex = start_vex;
        
        start_vex = start_vex->next;

        free(del_vex->name);
        free(del_vex);
    }
}

/**
 * @brief Function to free loaded memory for a graph
 * 
 * @param graph pointer to an allocated graph object
 */
void free_graph(graph_t *graph) {
    if (graph == NULL) {
        return;
    }

    if (graph->adj_list != NULL) {
        for (int32_t iter = 0; iter < graph->vex_nr; ++iter) {
            if (graph->adj_list[iter] != NULL) {
                free_vertices(graph->adj_list[iter]);
                graph->adj_list[iter] = NULL;
            }
        }

        free(graph->adj_list);
        graph->adj_list = NULL;
    }

    if (graph->index_map != NULL) {
        for (int32_t iter = 0; iter < graph->vex_nr; ++iter) {
            if (graph->index_map[iter] != NULL) {
                free(graph->index_map[iter]);
                graph->index_map[iter] = NULL;
            }
        }

        free(graph->index_map);
        graph->index_map = NULL;
    }

    free(graph);
}

/**
 * @brief Function to print on stdout the graph object
 * 
 * @param graph pointer to an valid graph object
 */
void print_graph(graph_t *graph) {
    if ((graph == NULL) || (graph->adj_list == NULL) || (graph->index_map == NULL)) {
        return;
    }

    for (int32_t iter = 0; iter < graph->vex_nr; ++iter) {
        printf("Node [%s] : ", get_node_value(graph, iter));

        graph_node_t *vex = graph->adj_list[iter];

        while (vex != NULL) {
            printf("(%s) -> ", vex->name);
            vex = vex->next;
        }

        printf("(null)\n");
    }
}