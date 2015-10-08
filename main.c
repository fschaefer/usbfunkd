// #############################################################################
// #                      --- USB Funk CLI Tool ---                            #
// #############################################################################
// # main.c - Main function                                                    #
// #############################################################################
// #                       Version: 1.2 - Compiler: GCC                        #
// #  (c) 2011-2014 by Malte Pöggel - www.MALTEPOEGGEL.de - malte@poeggel.de   #
// #############################################################################
// #  This program is free software; you can redistribute it and/or modify it  #
// #   under the terms of the GNU General Public License as published by the   #
// #        Free Software Foundation; either version 3 of the License,         #
// #                  or (at your option) any later version.                   #
// #                                                                           #
// #      This program is distributed in the hope that it will be useful,      #
// #      but WITHOUT ANY WARRANTY; without even the implied warranty of       #
// #           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.            #
// #           See the GNU General Public License for more details.            #
// #                                                                           #
// #  You should have received a copy of the GNU General Public License along  #
// #      with this program; if not, see <http://www.gnu.org/licenses/>.       #
// #############################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "usbfunk.h"

#include "mongoose.h"

#define BUF_SIZE 100

#define HTTP_200_OK "200 OK"
#define HTTP_400_BAD_REQUEST "400 Bad Request"
#define HTTP_486_BUSY_HERE "486 Busy Here"
#define HTTP_487_IDLE "487 Idle"
#define HTTP_503_SERVICE_UNAVAILABLE "503 Service Unavailable"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void handle_status_call(struct mg_connection *nc, struct http_message *hm) {
    int val;
    char *http_status;

    if ((val = usbfunk_init()) != USBFUNK_SUCCESS) {
        http_status = HTTP_503_SERVICE_UNAVAILABLE ;
    } else if ((val = usbfunk_status()) != USBFUNK_DEV_ERROR) {
        http_status = val ? HTTP_486_BUSY_HERE : HTTP_487_IDLE;
    } else {
        http_status = HTTP_400_BAD_REQUEST;
    }

    mg_printf(nc, "HTTP/1.1 %s\r\nTransfer-Encoding: chunked\r\n\r\n", http_status);
    mg_printf_http_chunk(nc, "%s", http_status);
    mg_send_http_chunk(nc, "", 0);
}

static void handle_setrepeat_call(struct mg_connection *nc, struct http_message *hm) {
    int val;
    char *http_status;

    char repeat[BUF_SIZE];
    mg_get_http_var(&hm->body, "repeat", repeat, sizeof(repeat));

    if ((val = usbfunk_init()) != USBFUNK_SUCCESS) {
        http_status = HTTP_503_SERVICE_UNAVAILABLE ;
    } else if ((val = usbfunk_setrepeat(atoi(repeat)) == USBFUNK_SUCCESS)) {
        http_status = HTTP_200_OK;
    } else {
        http_status = HTTP_400_BAD_REQUEST;
    }

    mg_printf(nc, "HTTP/1.1 %s\r\nTransfer-Encoding: chunked\r\n\r\n", http_status);
    mg_printf_http_chunk(nc, "%s", http_status);
    mg_send_http_chunk(nc, "", 0);
}

static void handle_setled_call(struct mg_connection *nc, struct http_message *hm) {
    int val;
    char *http_status;

    char inverse[BUF_SIZE];
    mg_get_http_var(&hm->body, "inverse", inverse, sizeof(inverse));

    if ((val = usbfunk_init()) != USBFUNK_SUCCESS) {
        http_status = HTTP_503_SERVICE_UNAVAILABLE ;
    } else if ((val = usbfunk_setled(atoi(inverse)) == USBFUNK_SUCCESS)) {
        http_status = HTTP_200_OK;
    } else {
        http_status = HTTP_400_BAD_REQUEST;
    }

    mg_printf(nc, "HTTP/1.1 %s\r\nTransfer-Encoding: chunked\r\n\r\n", http_status);
    mg_printf_http_chunk(nc, "%s", http_status);
    mg_send_http_chunk(nc, "", 0);
}

static void handle_switch_call(struct mg_connection *nc, struct http_message *hm) {
    int val;
    char *http_status;

    char type[BUF_SIZE];
    char housecode[BUF_SIZE];
    char code[BUF_SIZE];
    char status[BUF_SIZE];
    char tone[BUF_SIZE];

    mg_get_http_var(&hm->body, "type", type, sizeof(type));
    mg_get_http_var(&hm->body, "housecode", housecode, sizeof(housecode));
    mg_get_http_var(&hm->body, "code", code, sizeof(code));
    mg_get_http_var(&hm->body, "status", status, sizeof(status));
    mg_get_http_var(&hm->body, "tone", tone, sizeof(tone));

    if ((val = usbfunk_init()) != USBFUNK_SUCCESS) {
        http_status = HTTP_503_SERVICE_UNAVAILABLE ;
    } else if (strcasecmp(type, "pt0") == 0 && (val = usbfunk_switch_0(atoi(code)) == USBFUNK_SUCCESS)) {
        http_status = HTTP_200_OK;
    } else if (strcasecmp(type, "pt2") == 0 && (val = usbfunk_switch_2(atoi(housecode), code[0], strcasecmp(status, "off") == 0 ? 0 : 1) == USBFUNK_SUCCESS)) {
        http_status = HTTP_200_OK;
    } else if (strcasecmp(type, "pt4") == 0 && (val = usbfunk_switch_4(atoi(housecode), code[0], strcasecmp(status, "off") == 0 ? 0 : 1) == USBFUNK_SUCCESS)) {
        http_status = HTTP_200_OK;
    } else if (strcasecmp(type, "he") == 0 && (val = usbfunk_switch_he(atoi(code), strcasecmp(status, "off") == 0 ? 0 : 1) == USBFUNK_SUCCESS)) {
        http_status = HTTP_200_OK;
    } else if (strcasecmp(type, "raw") == 0 && (val = usbfunk_switch_raw(code) == USBFUNK_SUCCESS)) {
        http_status = HTTP_200_OK;
    } else if (strcasecmp(type, "hx") == 0 && (val = usbfunk_bell(atoi(code), atoi(tone)) == USBFUNK_SUCCESS)) {
        http_status = HTTP_200_OK;
    } else {
        http_status = HTTP_400_BAD_REQUEST;
    }

    mg_printf(nc, "HTTP/1.1 %s\r\nTransfer-Encoding: chunked\r\n\r\n", http_status);
    mg_printf_http_chunk(nc, "%s", http_status);
    mg_send_http_chunk(nc, "", 0);
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;

    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            if (mg_vcmp(&hm->uri, "/api/v1/status") == 0) {
                handle_status_call(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/api/v1/setrepeat") == 0) {
                handle_setrepeat_call(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/api/v1/setled") == 0) {
                handle_setled_call(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/api/v1/switch") == 0) {
                handle_switch_call(nc, hm);
            } else {
                mg_serve_http(nc, hm, s_http_server_opts);  /* Serve static content */
            }
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    int i;
#ifdef MG_ENABLE_SSL
    const char *ssl_cert = NULL;
#endif

    mg_mgr_init(&mgr, NULL);
    s_http_server_opts.document_root = "web_root";

    /* Process command line options to customize HTTP server */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-D") == 0 && i + 1 < argc) {
            mgr.hexdump_file = argv[++i];
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            s_http_server_opts.document_root = argv[++i];
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            s_http_port = argv[++i];
        } else if (strcmp(argv[i], "-L") == 0 && i + 1 < argc) {
            s_http_server_opts.enable_directory_listing = argv[++i];
        } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            s_http_server_opts.auth_domain = argv[++i];
#ifdef MG_ENABLE_JAVASCRIPT
        } else if (strcmp(argv[i], "-j") == 0 && i + 1 < argc) {
            const char *init_file = argv[++i];
            mg_enable_javascript(&mgr, v7_create(), init_file);
#endif
        } else if (strcmp(argv[i], "-P") == 0 && i + 1 < argc) {
            s_http_server_opts.global_auth_file = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            s_http_server_opts.per_directory_auth_file = argv[++i];
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            s_http_server_opts.url_rewrites = argv[++i];
#ifndef MG_DISABLE_CGI
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            s_http_server_opts.cgi_interpreter = argv[++i];
#endif
#ifdef MG_ENABLE_SSL
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            ssl_cert = argv[++i];
#endif
        } else {
            fprintf(stderr, "Unknown option: [%s]\n", argv[i]);
            exit(1);
        }
    }

    nc = mg_bind(&mgr, s_http_port, ev_handler);
    mg_set_protocol_http_websocket(nc);

    if (nc == NULL) {
        fprintf(stderr, "Error starting server on port %s\n", s_http_port);
        exit(1);
    }

#ifdef MG_ENABLE_SSL
    if (ssl_cert != NULL) {
        const char *err_str = mg_set_ssl(nc, ssl_cert, NULL);
        if (err_str != NULL) {
            fprintf(stderr, "Error loading SSL cert: %s\n", err_str);
            exit(1);
        }
    }
#endif

    printf("Starting RESTful server on port %s\n", s_http_port);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);

    return 0;
}
