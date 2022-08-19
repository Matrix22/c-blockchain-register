#ifndef BLACKDAG_H_
#define BLOCKDAG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <error.h>

#define MAX_NODE_LEN 10
#define MAX_COMM_LEN 3
#define MAX_LINE_LEN 256
#define DELIM_OPER " :\n"

typedef struct graph_node_s {
    char *name;
    struct graph_node_s *next;
} graph_node_t;

typedef struct graph_s {
    int32_t vex_nr;
    char **index_map;
    graph_node_t **adj_list;
} graph_t;

typedef struct ord_list_s {
    char *name;
    struct ord_list_s *next;
} ord_list_t;

typedef struct list_node_s {
	int32_t key;
	struct list_node_s *next;
	struct list_node_s *prev;
} list_node_t;

typedef struct list_s {
	list_node_t *head;
} list_t;

typedef list_t stack_t;
typedef list_t queue_t;

char**              create_index_map                (int32_t vex_nr, char *buffer);
graph_t*            create_graph_adj_list           (int32_t vex_nr, char *buffer);
int32_t             get_node_index_id               (graph_t *graph, const char *node_name);
char*               get_node_value                  (graph_t *graph, int32_t vex_idx);
void                add_edge_list                   (graph_t *graph, const char *vex1, const char *vex2);
graph_t*            create_graph                    (void);
graph_t*            create_transpose_graph          (graph_t *graph);
void                free_graph                      (graph_t *graph);
void                print_graph                     (graph_t *graph);

ord_list_t*         create_ord_list_node            (const char *vex_name);
int32_t             compare_string                  (const char *str1, const char *str2);
ord_list_t*         insert_ord_list                 (ord_list_t *list, const char *vex_name);
void                print_ord_list                  (ord_list_t *list, FILE *fout);
uint8_t             ord_list_contains               (ord_list_t *list, const char *vex_name);
void                free_ordered_list               (ord_list_t *list);

list_t*             create_list                     (void);
void                free_list                       (list_t *list);
uint8_t             is_list_empty                   (list_t *list);

stack_t*            create_stack                    (void);
void                free_stack                      (stack_t *stack);
void                push                            (stack_t *stack, int32_t key);
void                pop                             (stack_t *stack);
uint8_t             is_stack_empty                  (stack_t *stack);
int32_t             top                             (stack_t *stack);

queue_t*            create_queue                    (void);
void                free_queue                      (queue_t *queue);
void                enqueue                         (queue_t *queue, int32_t key);
void                dequeue                         (queue_t *queue);
uint8_t             is_queue_empty                  (queue_t *queue);
int32_t             front                           (queue_t *queue);

ord_list_t*         past                            (graph_t *graph, int32_t vex);
ord_list_t*         future                          (graph_t *graph, int32_t vex);
ord_list_t*         anticone                        (graph_t *graph, ord_list_t *past_list, ord_list_t *future_list, int32_t vex);
ord_list_t*         tips                            (graph_t *graph);

void                check_valid_dag                 (void);
void                graph_sets                      (const char *vex_name);

#endif /* BLOCKDAG_H_ */