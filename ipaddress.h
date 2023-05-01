#define MAX_LINE_LENGTH (1024)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int extract_ip_address_from_header(char *line, char *address) {
  int found = 0;
  char *ptr;
  char *name;
  char *value;

  name = strndup(line, MAX_LINE_LENGTH);
  ptr = index(name, (int)':');
  if (ptr == NULL) {
    return 0;
  }
  // end the string at the colon
  *ptr = '\0';

  // get the value part of the header field
  ptr = index(line, (int) ':');
  value = strndup(ptr + 2, MAX_LINE_LENGTH);

  // most ugly way to remove \r\n from the end of the string
  value[strlen(value)-2] = '\0';

  if (strncmp(name, "X-Forwarded-For", MAX_LINE_LENGTH) == 0) {
    found = 1;
    strncpy(address, value, MAX_LINE_LENGTH);
  }

  free(name);
  free(value);

  return found;
}

char * get_ip_address(int client_fd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char *ip_address = malloc(INET_ADDRSTRLEN);

    if (getsockname(client_fd, (struct sockaddr *)&client_addr, &client_len) == -1) {
        perror("getsockname");
        return NULL;
    }

    if (inet_ntop(AF_INET, &client_addr.sin_addr, ip_address, INET_ADDRSTRLEN) == NULL) {
        perror("inet_ntop");
        return NULL;
    }

  return ip_address;
}