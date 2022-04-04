#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main () {
        int fd = open("/dev/urandom", O_RDONLY);
        int x;
        read(fd, &x, 4);
        printf("%d +1 = ?\n", x);
        int y;
        scanf("%d", &y);
        if (y == x+1) {
                printf("midnight{DNS_Dennis_Thanksy0uforY0urService}\n");
        } else {
                printf("wrong\n");
        }
}
