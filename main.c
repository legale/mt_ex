#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
 
typedef struct exec_thread_arg {
    const char *cmd;
    char *buf;
    volatile size_t bufsize; 
    volatile size_t bytes_read; /* volatile to force compiler not to optimize this */
    volatile char thread_is_alive;
} exec_thread_arg_t;


void *exec_thread(void *vargp){
    FILE *fp;
    exec_thread_arg_t *args = (exec_thread_arg_t *)vargp;
    printf("exec: %s\n", args->cmd);

    fp = popen(args->cmd, "r");
    if (fp == NULL) {
        fprintf(stderr, "error: Unable to open process %s\n", args->cmd);
        return NULL;
    }

    //fread will return number of bytes read

    while((args->bytes_read = fread(args->buf, 1, args->bufsize, fp))) {    
        args->buf[args->bytes_read] = '\0'; //set terminator as last byte
    }

    /* status code */
    int ret = WEXITSTATUS(pclose(fp));
    fprintf(stderr, "status code: %d\n", ret);
    args->thread_is_alive = 0;
    return NULL;
}


/* useful to skip some code optimization
#pragma GCC push_options
#pragma GCC optimize ("O2")
*/


int main() {
    size_t bufsize = 2048;
    char *buf[bufsize];
    *buf = '\0';

    exec_thread_arg_t args = {
        .cmd = "sudo tcpdump -nni $(ls /sys/class/net/ | head -1) -c 2",
        .bufsize = bufsize,
        .buf = (char *)buf,
        .bytes_read = 0,
        .thread_is_alive = 1,
    };

    pthread_t thread_id;
    printf("Before Thread create\n");
    pthread_create(&thread_id, NULL, exec_thread, (void *)&args);

    printf("After Thread create\n");

    while(args.thread_is_alive){
        if(args.bytes_read){
            printf("OUT:\n%s\n", args.buf);
        }
    }

    //wait for pthread finished
    //pthread_join(thread_id, NULL);
    printf("After tcpdump finished\n");
    return 0;


}

/* useful to skip some code optimization
#pragma GCC pop_options
*/