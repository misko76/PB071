#include "scheduler.h"
#include <assert.h>
#include <string.h>

#ifdef CONFIG_ENABLE_DEBUG

#define DEBUG(STATEMENT)    \
    do {                    \
        STATEMENT;          \
    } while (0)
#else
#define DEBUG(STATEMENT)
#endif /* CONFIG_ENABLE_DEBUG */

priority_queue create_queue(void) {
    priority_queue new_queue = {NULL, NULL, 0};
    return new_queue;
}

void clear_queue(priority_queue *queue) {
    assert(queue != NULL);
    priority_queue_item *to_remove = queue->top;
    while (to_remove != NULL) {
        priority_queue_item *next = to_remove->next;
        free(to_remove);
        to_remove = next;
    }
    queue->top = queue->bottom = NULL;
    queue->size = 0;
}

bool alloc_queue(priority_queue *source_copy, const priority_queue *source) {
    priority_queue_item *last = NULL;
    priority_queue_item *current = source->top;
    while (current != NULL) {
        priority_queue_item *item_copy = malloc(sizeof(priority_queue_item));
        if (item_copy == NULL) { return false; }
        memcpy(item_copy, current, sizeof(priority_queue_item));
        if (source_copy->top == NULL) { source_copy->top = item_copy; }
        else { last->next = item_copy; }

        item_copy->prev = last;
        item_copy->next = NULL;
        source_copy->bottom = item_copy;
        source_copy->size++;
        last = item_copy;
        current = current->next;
    }
    return true;
}

bool copy_queue(priority_queue *dest, const priority_queue *source) {
    assert(dest != NULL);
    assert(source != NULL);
    priority_queue source_copy = {NULL, NULL, 0};
    if (!alloc_queue(&source_copy, source)) {
        clear_queue(&source_copy);
        return false;
    }
    *dest = source_copy;
    return true;
}

unsigned int inverse_priority(process_type process) {
    return process.remaining_time * process.niceness;
}

int processors(process_type process) {
    int count = 0;
    while (process.cpu_mask) {
        if ((process.cpu_mask & 1) == 1) { count++; }
        process.cpu_mask >>= 1;
    }
    return count;
}

enum push_result already_exists(process_type old, process_type new) {
    bool a = old.remaining_time == new.remaining_time;
    bool b = old.niceness == new.niceness;
    bool c = old.cpu_mask == new.cpu_mask;
    if (a && b && c) { return push_duplicate; }
    return push_inconsistent;
}

void push_queue_item(priority_queue *queue, priority_queue_item *new_element) {
    priority_queue_item *current = queue->top;
    while (current != NULL) {
        if (inverse_priority(new_element->process) > inverse_priority(current->process) ||
            (inverse_priority(new_element->process) == inverse_priority(current->process) &&
             processors(new_element->process) > processors(current->process))) {
            current = current->next;
        } else { break; }
    }
    if (queue->top == NULL) {
        // push item to empty queue
        queue->top = new_element;
        queue->bottom = new_element;
        new_element->prev = NULL;
        new_element->next = NULL;

    } else if (current == queue->top) {
        // push item to the top
        priority_queue_item *second = queue->top;
        queue->top = new_element;
        new_element->prev = NULL;
        new_element->next = second;
        second->prev = queue->top;

    } else if (current == NULL) {
        // push item to to the bottom
        queue->bottom->next = new_element;
        new_element->prev = queue->bottom;
        new_element->next = NULL;
        queue->bottom = new_element;

    } else {
        // push item somewhere in the middle
        current->prev->next = new_element;
        new_element->prev = current->prev;
        new_element->next = current;
        current->prev = new_element;
    }
}

enum push_result push_to_queue(priority_queue *queue, process_type process) {
    assert(queue != NULL);
    assert(10 <= process.niceness && process.niceness < 50);
    priority_queue_item *current = queue->top;
    while (current != NULL) {
        if (current->process.callback == process.callback &&
            current->process.context == process.context) {
            return already_exists(current->process, process);
        }
        current = current->next;
    }
    priority_queue_item *new_element = malloc(sizeof(priority_queue_item));
    if (new_element == NULL) { return push_error; }
    new_element->process = process;

    push_queue_item(queue, new_element);
    queue->size++;
    return push_success;
}

priority_queue_item *get_top_item(const priority_queue *queue, uint16_t cpu_mask) {
    priority_queue_item *current = queue->top;
    while (current != NULL) {
        if (cpu_mask & current->process.cpu_mask) { return current; }
        current = current->next;
    }
    return NULL;
}

process_type *get_top(const priority_queue *queue, uint16_t cpu_mask) {
    assert(queue != NULL);
    priority_queue_item *top = get_top_item(queue, cpu_mask);
    if (top == NULL) { return NULL; }
    return &top->process;
}

void pop_queue_item(priority_queue *queue, priority_queue_item *item) {
    if (item == queue->top) {
        queue->top = item->next;
        if (queue->top != NULL) { queue->top->prev = NULL; }
        else { queue->bottom = NULL; }

    } else if (item == queue->bottom) {
        queue->bottom = item->prev;
        queue->bottom->next = NULL;

    } else {
        item->prev->next = item->next;
        item->next->prev = item->prev;
    }
}

bool pop_top(priority_queue *queue, uint16_t cpu_mask, process_type *out) {
    assert(queue != NULL);
    priority_queue_item *top = get_top_item(queue, cpu_mask);
    if (top == NULL) { return false; }
    if (out != NULL) { *out = top->process; }

    pop_queue_item(queue, top);
    free(top);
    queue->size--;
    return true;
}

void move_queue_item(priority_queue *queue, priority_queue_item *to_move) {
    pop_queue_item(queue, to_move);
    push_queue_item(queue, to_move);
}

unsigned int run_top(priority_queue *queue, uint16_t cpu_mask, unsigned int run_time) {
    assert(queue != NULL);
    priority_queue_item *top = get_top_item(queue, cpu_mask);
    if (top == NULL) { return 0; }
    unsigned int cb_ret = top->process.callback(run_time, top->process.context);
    if (cb_ret == 0) {
        pop_queue_item(queue, top);
        free(top);
        queue->size--;
        return 0;
    }
    unsigned int max = 0;
    if (top->process.remaining_time > run_time) { max = top->process.remaining_time - run_time; }
    top->process.remaining_time = max + cb_ret;
    move_queue_item(queue, top);
    return top->process.remaining_time;
}

bool renice(priority_queue *queue, cb_type callback, void *context, unsigned int niceness) {
    assert(queue != NULL);
    assert(10 <= niceness && niceness < 50);
    priority_queue_item *current = queue->top;
    while (current != NULL) {
        if (current->process.callback == callback && current->process.context == context) {
            current->process.niceness = niceness;
            move_queue_item(queue, current);
            return true;
        }
        current = current->next;
    }
    return false;
}
