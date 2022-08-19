#include "./include/blockdag.h"

/**
 * @brief Function to create a ordere list node object
 * 
 * @param vex_name name of the vertex
 * @return ord_list_t* pointer to a new allocated list node object or NULL
 */
ord_list_t* create_ord_list_node(const char *vex_name) {
    ord_list_t *new_node = malloc(sizeof(*new_node));

    if (new_node != NULL) {
        new_node->name = malloc(MAX_NODE_LEN);

        if (new_node->name == NULL) {
            free(new_node);

            return NULL;
        }

        snprintf(new_node->name, MAX_NODE_LEN, "%s", vex_name);
        new_node->next = NULL;
    }

    return new_node;
}

/**
 * @brief Compare function to compare to strings
 * 
 * @param str1 first string to be compared
 * @param str2 second string to be compared
 * @return int32_t result of comparation
 */
int32_t compare_string(const char *str1, const char *str2) {
    if (strncmp(str1, "Genesis", MAX_NODE_LEN) == 0) {
        return -1;
    }

    if (strncmp(str2, "Genesis", MAX_NODE_LEN) == 0) {
        return 1;
    }

    return strncmp(str1, str2, MAX_NODE_LEN);
}

/**
 * @brief Function to insert one node into the ordered list
 * 
 * @param list pointer to the head of the list
 * @param vex_name vertex name to insert
 * @return ord_list_t* pointer to the new head of the list
 */
ord_list_t* insert_ord_list(ord_list_t *list, const char *vex_name) {
    if (list == NULL) {
        list = create_ord_list_node(vex_name);
        
        return list;
    }

    ord_list_t *iter = list;
    ord_list_t *prev_iter = NULL;

    while ((iter != NULL) && (compare_string(iter->name, vex_name) < 0)) {
        prev_iter = iter;
        iter = iter->next;
    }

    ord_list_t *new_node = create_ord_list_node(vex_name);

    if (new_node != NULL) {
        if (iter == list) {
            new_node->next = list;
            list = new_node;
        } else if (iter == NULL) {
            prev_iter->next = new_node;
        } else {
            new_node->next = iter;
            prev_iter->next = new_node;
        }
    }

    return list;
}

/**
 * @brief Function to print the list names in the file stream
 * 
 * @param list pointer to the head of the list 
 * @param fout pointer to a stream
 */
void print_ord_list(ord_list_t *list, FILE *fout) {
    while (list != NULL) {
        fprintf(fout, "%s ", list->name);
        list = list->next;
    }

    fprintf(fout, "\n");
}

/**
 * @brief Function to check if a vertex name is in the current selected ordered list
 * 
 * @param list pointer to the head of the list
 * @param vex_name name of the vertex
 * @return uint8_t 1 if name exists in the list, 0 otherwise
 */
uint8_t ord_list_contains(ord_list_t *list, const char *vex_name) {
    if (list == NULL) {
        return 0;
    }

    while(list != NULL) {
        if (strncmp(list->name, vex_name, MAX_NODE_LEN) == 0) {
            return 1;
        }

        list = list->next;
    }

    return 0;
}

/**
 * @brief Function to free loaded memory for an orderd list
 * 
 * @param list pointer to an allocated ordered list object
 */
void free_ordered_list(ord_list_t *list) {
    while (list != NULL) {
        ord_list_t *iter = list;

        list = list->next;

        free(iter->name);
        iter->name = NULL;

        free(iter);
    }
}