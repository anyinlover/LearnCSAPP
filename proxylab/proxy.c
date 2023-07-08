#include "csapp.h"
#include <stdio.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";

void doit(int fd);
void parse_uri(char *uri, char *hostname, char *port, char *filename);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
                 char *longmsg);
void transfer_hdrs(int clientfd, rio_t *rio);

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  listenfd = Open_listenfd(argv[1]);
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE,
                0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
    doit(connfd);
    Close(connfd);
  }

  return 0;
}

void doit(int fd) {
  int clientfd;
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE],
      hostname[MAXLINE], port[MAXLINE], filename[MAXLINE];
  rio_t rio, clientrio;

  Rio_readinitb(&rio, fd);
  if (!Rio_readlineb(&rio, buf, MAXLINE))
    return;
  printf("%s", buf);
  sscanf(buf, "%s %s %s", method, uri, version);
  if (strcasecmp(method, "GET")) {
    clienterror(fd, method, "501", "Not Implemented",
                "Proxy does not implement this method");
    return;
  }
  parse_uri(uri, hostname, port, filename);
  clientfd = Open_clientfd(hostname, port);
  Rio_readinitb(&clientrio, clientfd);
  sprintf(buf, "%s %s HTTP/1.0\r\n", method, filename);
  Rio_writen(clientfd, buf, strlen(buf));
  transfer_hdrs(clientfd, &rio);
  Dup2(clientfd, fd);
  return;
}

void parse_uri(char *uri, char *hostname, char *port, char *filename) {}
