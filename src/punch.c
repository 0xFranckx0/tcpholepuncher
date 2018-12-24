#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#include <event2/event.h>
#include <event2/listener.h>

#include <thp.h>

#include "log.h"

#include "net.h"
#include "queue.h"

#define MINPORT 0
#define MAXPORT 65535
struct thp_punch {

};

struct port_list;
struct entry {
        evutil_socket_t sock;
        char *port_str;
         LIST_ENTRY(entry) entries;
};
LIST_HEAD(port_list, entry) head;

static int
punch_init(const char *, const char *, const char *);
static int
parse_ports(const char *, const char *);
static int
str2int(char *);
static void
free_port_list(struct port_list *);
static void
delete_entry(struct entry *);
static void
print_port_list(struct port_list *);

static struct event_base *ev_base = NULL;

static void
port_free();
static struct port *
port_new();
static void
port_list_free(struct port_list *);
static struct port_list *
port_list_new(const char *);
static void
punch_timeout_cb(int, short, void *);
static void
punch_free();
static struct thp_punch *
punch_new();
static void
listen_error_cb(struct evconnlistener *, void *);
static void
listen_conn_cb(struct evconnlistener *, int, struct sockaddr *, int, void *);

struct port *
port_new()
{
        struct port *p = NULL;

        if ((p = malloc(sizeof(*p))) == NULL) {
                log_warn("%s: malloc", __func__);
                goto error;
        }
        p->listener = NULL;
        p->str = NULL;

        return (p);

 error:
        port_free(p);
        return (NULL);
}

void
port_free(struct port *p)
{
        if (p == NULL)
                return;

        if (p->listener != NULL)
                evconnlistener_free(p->listener);
        free(p->str);
        free(p);
}

void
port_list_free(struct port_list *l)
{
        struct port *p;

        if (l == NULL)
                return;

        while (!LIST_EMPTY(l)) {
                p = LIST_FIRST(l);
                LIST_REMOVE(p, entry);
                port_free(p);
        }

        free(l);
}

struct port_list *
port_list_new(const char *ports)
{
        struct port_list *l = NULL;
        struct port *p;

        if ((l = malloc(sizeof(*l))) == NULL) {
                log_warn("%s: malloc", __func__);
                goto error;
        }
        LIST_INIT(l);

        /* XXX hardcoded for testing */

        p = port_new();
        p->num = 8080;
        p->str = strdup("8080");

        LIST_INSERT_HEAD(l, p, entry);

        p = port_new();
        p->num = 9090;
        p->str = strdup("9090");

        LIST_INSERT_HEAD(l, p, entry);

        /* TODO:
         * parse ports
         * port_new() LIST_INSERT_HEAD()
         */

        return (l);

 error:
        port_list_free(l);
        return (NULL);
}

void
punch_timeout_cb(int fd, short event, void *arg)
{

}

void
punch_free(struct thp_punch *thp)
{
        if (thp == NULL)
                return;

        event_free(thp->timeout);
        port_list_free(thp->ports);

        free(thp);
}

struct thp_punch *
punch_new()
{
        struct thp_punch *thp;

        if ((thp = malloc(sizeof(*thp))) == NULL) {
                log_warn("%s: malloc", __func__);
                goto error;
        }
        thp->timeout = NULL;
        thp->ports = NULL;

        if ((thp->timeout =
             evtimer_new(ev_base, punch_timeout_cb, thp)) == NULL) {
                log_warnx("%s: evtimer_new", __func__);
                goto error;
        }

        return (thp);

 error:
        punch_free(thp);
        return (NULL);
}

void
listen_error_cb(struct evconnlistener *l, void *arg)
{
        return;
}

void
listen_conn_cb(struct evconnlistener *l, int fd, struct sockaddr *address,
               int socklen, void *arg)
{

}

struct thp_punch *
thp_punch_start(struct event_base *evb, const char *ip, const char *ports,
                thp_punch_cb cb, void *data)
{
        struct port *p;
        struct thp_punch *thp = NULL;
        struct addrinfo hints, *ai = NULL;
        int ret;

        ev_base = evb;

        if ((thp = punch_new()) == NULL) {
                log_warnx("%s: punch_new", __func__);
                goto error;
        }

        if ((thp->ports = port_list_new(ports)) == NULL) {
                log_warnx("%s: port_list_new: %s", __func__);
                goto error;
        }

        LIST_FOREACH(p, thp->ports, entry) {

                memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;

                /* listen on every interfaces */
                if ((ret = getaddrinfo("0.0.0.0", p->str, &hints, &ai)) != 0) {
                        log_warnx("%s: getaddrinfo: %s", __func__,
                                  gai_strerror(ret));
                        goto error;
                }

                if ((p->listener =
                     evconnlistener_new_bind(ev_base, listen_conn_cb, thp,
                                             LEV_OPT_CLOSE_ON_FREE |
                                             LEV_OPT_REUSEABLE, -1, ai->ai_addr,
                                             ai->ai_addrlen)) == NULL) {
                        log_warnx("%s: evconnlistener_new_bind", __func__);
                        goto error;
                }

                evconnlistener_set_error_cb(p->listener, listen_error_cb);
                freeaddrinfo(ai);
                ai = NULL;

                /* TODO: connect() */
        }

        return (thp);

 error:
        freeaddrinfo(ai);
        punch_free(thp);
        return (NULL);
}

void
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

        err = parse_ports(address, ports);
        if (err < 0) {
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
parse_ports(const char *address, const char *ports_str)
{
        int code = 0;
        int port, counter = 0;
        char *token = NULL;
        char *current = strdup(ports_str);
        char *p;
        struct entry *port_entry;

        LIST_INIT(&head);
        token = strtok(current, ",");
        while (token != NULL) {
                port_entry = malloc(sizeof(struct entry));
                if (port_entry == NULL) {
                        log_error("malloc failed while allocating memory for \
                                port_entry");
                        goto cleanup;
                }
                port_entry->port_str = strdup(token);
                if (port_entry == NULL) {
                        log_error("strdup failed while duplicating token");
                        if (counter == 0)
                                free(port_entry);
                        goto cleanup;
                }
                port = str2int(token);
                if (port > MINPORT && port < MAXPORT) {
                        port_entry->sock =
                            new_receiver(address, port_entry->port_str);
                }
                LIST_INSERT_HEAD(&head, port_entry, entries);
                token = strtok(NULL, ",");
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
        if (str[0] == '\0' || *buf != '\0') {
                log_error("#1 strtol failed");
                return -1;
        }
        if ((errno == ERANGE && (lval == LONG_MAX || lval == LONG_MIN))
            || (lval > 65535 || lval < 0)) {
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
        if (e != NULL) {
                if (e->port_str != NULL)
                        free(e->port_str);

                free(e);
        }
}

void
print_port_list(struct port_list *list)
{
        struct entry *e;
        if (list != NULL) {
                LIST_FOREACH(e, list, entries)
                    printf("Port string: %s\n", e->port_str);
        }
}
