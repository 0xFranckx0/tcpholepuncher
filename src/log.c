#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <thp.h>

static thp_logcb cb_log_warn = NULL;
static thp_logcb_err cb_log_error = NULL;

void
thp_log_setcb(thp_logcb log_warn, thp_logcb_err log_error)
{
        cb_log_warn = log_warn;
        cb_log_error = log_error;
}

void
log_warnx(const char *format, ...)
{
        va_list list;
        static char buff[512];
        int len;

        va_start(list, format);
        if (cb_log_warn) {
                len = vsnprintf(buff, sizeof(buff), format, list);
                snprintf(buff + len, sizeof(buff) - len, "\n");
                cb_log_warn(buff);
        } else
                vfprintf(stdout, format, list);
        va_end(list);
}

void
log_warn(const char *format, ...)
{
        va_list list;
        static char buff[512];
        int len;

        va_start(list, format);
        if (cb_log_warn) {
                len = vsnprintf(buff, sizeof(buff), format, list);
                snprintf(buff + len, sizeof(buff) - len, ": %s\n",
                         strerror(errno));
                cb_log_warn(buff);
        } else
                vfprintf(stdout, format, list);
        va_end(list);
}

void
log_error(const char *format, ...)
{
        va_list list;
        static char buff[512];
        int len;

        va_start(list, format);
        if (cb_log_error) {
                len = vsnprintf(buff, sizeof(buff), format, list);
                snprintf(buff + len, sizeof(buff) - len, "\n");
                cb_log_warn(buff);
        } else
                vfprintf(stdout, format, list);
        va_end(list);
}

void
log_errorx(const char *format, ...)
{
        va_list list;
        static char buff[512];
        int len;

        va_start(list, format);
        if (cb_log_error) {
                len = vsnprintf(buff, sizeof(buff), format, list);
                snprintf(buff + len, sizeof(buff) - len, ": %s\n",
                         strerror(errno));
                cb_log_warn(buff);
        } else
                vfprintf(stdout, format, list);
        va_end(list);
}
