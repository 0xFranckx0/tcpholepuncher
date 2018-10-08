#ifndef THP_H
#define THP_H

#include <stdarg.h>

#define PUNCH_START     0
#define PUNCH_SUCCESS   1
#define PUNCH_FAILURE   2
typedef void (*thp_logcb)(const char *);
typedef void (*thp_logcb_err)(const char *);

void	 thp_log_setcb(thp_logcb, thp_logcb_err);

typedef void (*thp_punch_cb)(int, int, void *); /* event, socket, data */
struct thp_punch;

struct thp_punch        *thp_punch_start(const char *, const char *,
                                 const char *, thp_punch_cb, void *);
int                      thp_punch_stop(struct thp_punch *);

#endif