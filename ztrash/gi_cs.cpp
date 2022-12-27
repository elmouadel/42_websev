#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

void signal_handler(int sig) {
  // handle the signal
  std::cout << "Caught signal" << sig << std::endl;
  printf("Caught signal %d\n", sig);
  perror("perr: ");
}

  int main(int argc, char *argv[]) {
  struct sigaction sa;
  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGPIPE, &sa, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
  signal(SIGPIPE, SIG_IGN);

  char **args;
  args = (char**)malloc(3);
  args[0] = (char*)malloc(100);
  args[1] = (char*)malloc(100);
  memcpy(args[0], "./webserv\0", 10);
  memcpy(args[1], "./config/default.conf\0", 22);
  args[2] = NULL;
  if (!fork())
  {
    execve(args[0], args, NULL);
    exit(0);
  }

  int status;
  waitpid(-1, &status, 0);

  if (WIFSIGNALED(status)) {
    int sig = WTERMSIG(status);
    printf("Program terminated due to signal %d\n", sig);
  }

  return 0;
}