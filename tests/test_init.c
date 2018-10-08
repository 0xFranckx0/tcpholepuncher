#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <thp.h>


const char *msg_01 = "test-init: ";

void
log_warn(const char *msg)
{
        fprintf(stdout, "WARNING: %s\n", msg);
}

void
log_error(const char *msg)
{
        fprintf(stderr, "ERROR: %s\n", msg);
}
void
punch_cb(int event, int sock, void *data) {
        
}

static struct thp_punch *test_punch(char *, char *, char *);
static 

int main()
{
        struct thp_punch *punch = NULL;
        punch = test_punch("192.168.0.12", "10000", NULL);
        if (punch == NULL) {
                log_error("Punch failed");
        }
        punch = test_punch("192.168.0.12", "10000", "tcp");
        if (punch == NULL) {
                log_error("Punch failed");
        }
        return 0;
}


struct thp_punch *
test_punch(char *address, char *ports, char *type)
{
        struct thp_punch *punch = NULL;

	thp_log_setcb(log_warn, log_error);
        punch = thp_punch_start(address, ports, type, punch_cb, NULL);
        if (punch == NULL)
                return NULL;

        return punch;
}