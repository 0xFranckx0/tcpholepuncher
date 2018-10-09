#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#include <thp.h>

#include "log.h"
#include "queue.h"

struct thp_punch {

};

struct port_list;
struct entry {
        int port;
        char *port_str;
        LIST_ENTRY(entry) entries;
};
LIST_HEAD(port_list, entry) head;

static int punch_init(const char *, const char *, const char *);
static int parse_ports(const char *);
static int str2int(char *);
static void free_port_list(struct port_list *);
static void delete_entry(struct entry *);
static void print_port_list(struct port_list *);

struct thp_punch *
thp_punch_start(const char * address, const char *ports, const char *type, 
                thp_punch_cb cb, void *data)
{
        struct thp_punch *punch = NULL;

        if (( address == NULL ) || (ports == NULL) || ( type == NULL)) {
                        log_error("Missing parameters to call the punch");
                        goto error;
                }
        punch_init(address, ports, type);
        return punch;

        error:
                return NULL;
}
int
thp_punch_stop(struct thp_punch *p)
{
        return 0;
}


/*
 * static functions
 */

int
punch_init(const char *address, const char *ports, const char *type)
{
        int err;

        err = parse_ports(ports);
        if (err < 0){
                log_error("an error occured while parsing ports");
                goto error;
        }

        return PUNCH_SUCCESS;

        error:
                return PUNCH_FAILURE;
}

/*
 * parse_ports:
 *      string format: 10,12-34,450,5000,4000-4010
 *      Each comma delimits a token. If the token 
 *      contains a dash it will mean a range including
 */
int
parse_ports(const char *ports_str)
{
        int code = 0;
        int port, counter = 0;
        char *token = NULL;
        char *current = strdup(ports_str);
        char *p;
        struct entry *port_entry;
        
        LIST_INIT(&head);
        token = strtok (current,",");
        while (token != NULL){
                port_entry = malloc(sizeof(struct entry));
                if (port_entry == NULL) {
                        log_error("malloc failed while allocating memory for \
                                   port_entry");
                        goto cleanup;
                }
                port_entry->port_str = strdup(token);
                if (port_entry == NULL){
                        log_error("strdup failed while duplicating token");
                        if (counter == 0)
                                free(port_entry);
                        goto cleanup;
                }
                port_entry->port = str2int(token);
                if (port_entry->port < 0) {
                        log_error("An error occured while converting port \
                                   to an integer");
                        code = port_entry->port;
                        goto cleanup;
                }
                LIST_INSERT_HEAD(&head, port_entry, entries);
                token = strtok (NULL, ",");
                counter++;
        }
        print_port_list(&head);

        cleanup:
                if (counter > 0)
                        free_port_list(&head);
                if (current != NULL)
                        free(current);

                return code;
}

int
str2int(char *str)
{
        long lval;
        char *buf;

        errno = 0; 
        lval = strtol(str, &buf, 10);
        if (str[0] == '\0' || *buf != '\0'){ 
                log_error("#1 strtol failed");
                return -1;
        }
        if ((errno == ERANGE && (lval == LONG_MAX || 
                lval == LONG_MIN)) || (lval > 65535 || lval < 0)){ 
                log_error("#2 strtol failed");
                return -1;
        }

        return (int)lval;
}

void
free_port_list(struct port_list *list)
{
        struct entry *e;

        while (!LIST_EMPTY(list)) {
                e = LIST_FIRST(list);
                LIST_REMOVE(e, entries);
                delete_entry(e);
        }
}

void
delete_entry(struct entry *e)
{
        if (e != NULL){
                if (e->port_str != NULL)
                        free(e->port_str);

                free(e);
        }
}

void
print_port_list(struct port_list *list)
{
        struct entry *e;
        if (list != NULL){
                LIST_FOREACH(e, list, entries)
                        printf("Port string: %s\nPort Integer: %d\n",
                                 e->port_str, e->port);
        }
}