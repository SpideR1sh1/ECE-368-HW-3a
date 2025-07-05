#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

/*********
* parse_list
* parameters: char *line - string containing a Python-style list like "[1, 2, 3]"
* what it outputs: ListNode* - pointer to head of linked list, or NULL if parsing fails
*********/
static ListNode* parse_list(char *line) {
    char *p = line;
    while (*p && *p != '[') p++;
    if (!*p) return NULL;
    p++;
    ListNode *head = NULL, *tail = NULL;
    while (1) {

        while (*p == ' ' || *p == '\t') p++;

        if (*p == ']' || *p == '\0') break;
        
        char *end;
        long v = strtol(p, &end, 10);
        if (p == end) break;
        
        ListNode *node = malloc(sizeof *node);
        if (!node) {
            while (head) {
                ListNode *temp = head;
                head = head->next;
                free(temp);
            }
            return NULL;
        }
        node->val = (int)v;
        node->next = NULL;
        if (!head) head = tail = node;
        else { tail->next = node; tail = node; }
        
        p = end;

        while (*p == ',' || *p == ' ' || *p == '\t') p++;
    }
    return head;
}

/*********
* main
* parameters: int argc - number of command line arguments, char **argv - array of command line arguments
* what it outputs: int - return code (0 for success, 1 for error)
*********/
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    char line[1024];
    ListNode **lists = NULL;
    int cap = 0, n = 0;

    while (fgets(line, sizeof(line), f)) {
        ListNode *l = parse_list(line);
        if (n == cap) {
            cap = cap ? cap * 2 : 4;
            ListNode **new_lists = realloc(lists, cap * sizeof *lists);
            if (!new_lists) {
                for (int i = 0; i < n; i++) {
                    while (lists[i]) {
                        ListNode *temp = lists[i];
                        lists[i] = lists[i]->next;
                        free(temp);
                    }
                }
                free(lists);
                fclose(f);
                return 1;
            }
            lists = new_lists;
        }
        lists[n++] = l;
    }
    fclose(f);


    ListNode dummy = {0, NULL}, *tail = &dummy;
    while (1) {
        int minIdx = -1;
        for (int i = 0; i < n; i++) {
            if (lists[i] && (minIdx < 0 || lists[i]->val < lists[minIdx]->val))
                minIdx = i;
        }
        if (minIdx < 0) break;
        ListNode *m = lists[minIdx];
        lists[minIdx] = m->next;
        m->next = NULL;
        tail->next = m;
        tail = m;
    }

    printf("[");
    ListNode *cur = dummy.next;
    int first = 1;
    while (cur) {
        if (!first) printf(", ");
        printf("%d", cur->val);
        first = 0;
        ListNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    printf("]\n");

    free(lists);
    return 0;
}
