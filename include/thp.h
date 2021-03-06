#ifndef THP_H
#define THP_H

#include <event2/event.h>

struct thp_punch;

typedef void (*thp_logcb)(const char *);
typedef void (*thp_punch_cb)(int, int, void *);	/* event, socket, data */

void			 thp_log_setcb(thp_logcb);
struct thp_punch        *thp_punch_start(struct event_base *, const char *, const char *,
			    thp_punch_cb, void *);
void			 thp_punch_stop(struct thp_punch *);

#endif
