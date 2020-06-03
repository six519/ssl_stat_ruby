#include "ruby.h"
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

char *get_second_part(char *str) {
    char * token = strtok(str, ":");
    token = strtok(NULL, ":");
    return token;
}

char *get_datetime(char *str) {
    char * retstr = (char *) malloc(30);
    char * token = strtok(str, ":");
    strcpy(retstr, "");
    int ignore_first = 1;
    while( token != NULL ) {
        if (ignore_first) {
            ignore_first = 0;
            token = strtok(NULL, " ");
            continue;
        }
        strcat(retstr, token);
        strcat(retstr, " ");
        token = strtok(NULL, " ");
    }
    return retstr;
}

static size_t wrfu(void *ptr,  size_t  size,  size_t  nmemb,  void *stream) {
    (void)stream;
    (void)ptr;
    return size * nmemb;
}

VALUE SSLStat = Qnil;
void Init_ssl_stat();
VALUE method_check(VALUE, VALUE);

void Init_ssl_stat(){
    VALUE SSLStat = rb_define_module("SSLStat");
    rb_define_method(SSLStat, "check", method_check, 1);
}

VALUE method_check(VALUE self, VALUE arg_url) {
    char *url_to_check = StringValueCStr(arg_url);

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url_to_check);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wrfu);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_CERTINFO, 1L);

        res = curl_easy_perform(curl);

        if(!res) {
            struct curl_certinfo *certinfo;
            int need_break = 0;
            char *serial_number, *expire_date, *start_date;

            res = curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);

            if(!res && certinfo) {
                int i;

                for(i = 0; i < certinfo->num_of_certs; i++) {
                    struct curl_slist *slist;

                    for(slist = certinfo->certinfo[i]; slist; slist = slist->next) {
                        if (strstr(slist->data, "Serial Number:") != NULL) {
                            serial_number = get_second_part(slist->data);
                        }

                        if(strstr(slist->data, "Start date:") != NULL) {
                            start_date = get_datetime(slist->data);
                        } 

                        if(strstr(slist->data, "Expire date:") != NULL) {
                            expire_date = get_datetime(slist->data);
                        } 
                        if(strstr(slist->data, "Subject Alternative Name") != NULL) {
                            need_break = 1;
                        }
                    }

                    if (need_break) {
                        VALUE hash = rb_hash_new();

                        rb_hash_aset(hash, rb_str_new2("serial_number"), rb_str_new_cstr(serial_number));
                        rb_hash_aset(hash, rb_str_new2("start_date"), rb_str_new_cstr(start_date));
                        rb_hash_aset(hash, rb_str_new2("expire_date"), rb_str_new_cstr(expire_date));

                        curl_easy_cleanup(curl);
                        curl_global_cleanup();
                        return hash;
                    }

                }
            }

        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return rb_hash_new();
}