# learn2pipe

This is a reference to remind people how to use a terminal

## flag.c

flag.c is a simple program that was made for kgbfskfsb. The intent was to require a more sophisticated
exfiltration technique since the buffers aren't flushed. 

This led to intense frustration as players found the flag binary but were unable to use it.

```
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
        printf("ctf{FLAG GOES HERE}\n");
    } else {
        printf("wrong\n");
    }
}

```

To understand the problem, consider the following usages:
```
#in a terminal
adc@ctf:~/ctf$ ./flag 
-1959229331 +1 = ?
```


```
#when no terminal is available
adc@ctf:~/ctf$ ./flag |cat
```

This is really unix 101 but apparently "impossible" and "guessy". 

## Unintended solution bonus

As an aside, one clever team noticed that theres missing error checking, Using ulimit it is possible to bypass the check.
Well done.

(echo 1 | /bin/sh -c "ulimit -n 1; ./flag")


## forkpty to the rescue

So if a terminal is expected, make one. Many teams asked for help debugging. Unfortunately since other teams had already scored,
it would have been unfair to add more hints on how to work a pty. 


```
#include <util.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

int main () {
  int tfd;
  pid_t pid;

  pid = forkpty(&tfd, NULL, NULL, NULL);

  struct termios terminal;
  tcgetattr(tfd, &terminal);
  terminal.c_lflag &= ~ECHO;
  tcsetattr(tfd, TCSANOW, &terminal);

  if (pid == 0) {
    execl("./flag", "./flag", NULL);
  } else {

    char buf[100];
    memset(&buf, 0, sizeof(buf));
    read(tfd, buf, 100);

    int g = 0;
    sscanf(buf, "%d", &g);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d\n", g+1);
    write(tfd, buf, sizeof(buf));
    read(tfd, buf, 100);
    printf("got reply -> %s\n", buf);
    return 0;
  }

}
```

```
adc@ctf:~/ctf$ ./a.out 
got reply -> ctf{FLAG GOES HERE}

```
