#include <stdlib.h>
#include <stdio.h>
#include <thp.h>

#include "log.h"

struct thp_punch {

};

static int punch_init(const char *, const char *, const char *);

struct thp_punch *
thp_punch_start(const char * address, const char *ports, const char *type, 
                thp_punch_cb cb, void *data)
{
        struct thp_punch *punch = NULL;

        if (( address == NULL ) || (ports == NULL) || ( type == NULL)) {
        //if (type == NULL) {
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

        return PUNCH_FAILURE;
}