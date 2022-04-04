#include <stdio.h>
#include <pty.h>
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
