#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <thp.h>


const char *msg_01 = "test-init: ";

void
log_01(const char *msg)
{
	if (strncmp(msg, msg_01, strlen(msg_01)) != 0)
		exit(-1);
}

void
punch_cb(int event, int sock, void *data) {
        
}

int main()
{
        struct thp_punch *punch;

	thp_log_setcb(log_01);
        punch = thp_punch_start("192.168.0.12", "10000", "tcp", punch_cb, NULL);
        if (punch == NULL) {
                perror("Failed to init the punch");
                return -1;
        }

        return 0;
}