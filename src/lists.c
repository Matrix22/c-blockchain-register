#include "./include/blockdag.h"

/**
 * @brief Function to create a list object
 * 
 * @return list_t* pointer to an allocated list object or NULL
 */
list_t* create_list(void) {
	list_t *new_list = malloc(sizeof(*new_list));

	if (new_list != NULL) {
		new_list->head = malloc(sizeof(*new_list->head));

		if (new_list->head != NULL) {
			new_list->head->next = new_list->head->prev = new_list->head;
		}
	}

	return new_list;
}

/**
 * @brief Function to free a list object
 * 
 * @param list pointer to a list object
 */
void free_list(list_t *list) {
	list_node_t *iter = list->head->next;

	while(iter != list->head) {
		list_node_t *temp = iter;

		iter = iter->next;
		
		free(temp);
	}

	free(list->head);
	free(list);
}

/**
 * @brief Function to check if the list object is empty
 * 
 * @param list pointer to an allocated list object
 * @return uint8_t 1 if list is emoty, 0 otherwise
 */
uint8_t is_list_empty(list_t *list) {
	if ((list == NULL) || (list->head == NULL)) {
		return 1;
	}

	return (list->head->next == list->head);
}

/**
 * @brief Create a stack object
 * 
 * @return stack_t* pointer to an allocated stack object
 */
stack_t* create_stack(void) {
	return create_list();
}

/**
 * @brief Function to free a stack object
 * 
 * @param stack pointer to an allocated stack object
 */
void free_stack(stack_t *stack) {
	free_list(stack);
}

/**
 * @brief Function to push one element onto the stack
 * 
 * @param stack pointer to an allocated stack object
 * @param key element to push
 */
void push(stack_t *stack, int32_t key) {
	list_node_t *new_node = malloc(sizeof(*new_node));
	
	new_node->key = key;

	new_node->next = stack->head->next;
	new_node->prev = stack->head;

	stack->head->next->prev = new_node;
	stack->head->next = new_node;
}

/**
 * @brief Function to pop one element from stack
 * 
 * @param stack pointer to an allocated stack object
 */
void pop(stack_t *stack) {
	list_node_t *temp = stack->head->next;

	stack->head->next = stack->head->next->next;
	stack->head->next->prev = stack->head;
	
	free(temp);
}

/**
 * @brief Function to check if the stack object is empty
 * 
 * @param stack pointer to an allocated stack object
 * @return uint8_t 1 if stack is empty, 0 otherwise
 */
uint8_t is_stack_empty(stack_t *stack) {
	return is_list_empty(stack);
}

/**
 * @brief Function to get the top stack element
 * 
 * @param stack pointer to an allocated stack object
 * @return int32_t top element
 */
int32_t top(stack_t *stack) {
	return stack->head->next->key;
}

/**
 * @brief Create a queue object
 * 
 * @return queue_t* pointer to an allocated queue object
 */
queue_t* create_queue(void) {
	return create_list();
}

/**
 * @brief Function to free a queue object
 * 
 * @param queue pointer to an allocated queue object
 */
void free_queue(queue_t *queue) {
	free_list(queue);
}

/**
 * @brief Function to push one element into the queue
 * 
 * @param queue pointer to an allocated queue object
 * @param key element to push
 */
void enqueue(queue_t *queue, int32_t key) {
	push(queue, key);
}

/**
 * @brief Function to pop one element from queue
 * 
 * @param queue pointer to an allocated queue object
 */
void dequeue(queue_t *queue) {
	list_node_t *temp = queue->head->prev;

	queue->head->prev = queue->head->prev->prev;
	queue->head->prev->next = queue->head;

	free(temp);
}

/**
 * @brief Function to check if the queue object is empty
 * 
 * @param queue pointer to an allocated queue object
 * @return uint8_t 1 if queue is empty, 0 otherwise
 */
uint8_t is_queue_empty(queue_t *queue) {
	return is_list_empty(queue);
}

/**
 * @brief Function to get the front queue element
 * 
 * @param queue pointer to an allocated queue object
 * @return int32_t front element
 */
int32_t front(queue_t *queue) {
	return queue->head->prev->key;
}