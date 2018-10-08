#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#include <thp.h>

#include "log.h"
#include "queue.h"

struct thp_punch {

};

struct entry {
        int port;
        LIST_ENTRY(entry) entries;
};
LIST_HEAD(port_list, entry) head;

static int punch_init(const char *, const char *, const char *);
static int parse_ports(const char *);
static int str2int(char *);

struct thp_punch *
thp_punch_start(const char * address, const char *ports, const char *type, 
                thp_punch_cb cb, void *data)
{
        struct thp_punch *punch = NULL;

        if (( address == NULL ) || (ports == NULL) || ( type == NULL)) {
                        log_error("Missing parameters to call the punch");
                        goto error;
                }

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

int
parse_ports(const char *ports_str)
{
        LIST_INIT(&head);
        return 0;
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