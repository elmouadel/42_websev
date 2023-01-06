#include <iostream>


// int main()
// {
// 	std::string s = "0123456789012345678901";

// 	std::cout <<(long)&s[0] << std::endl;
// 	s.push_back('\0');
// 	std::cout <<(long)&s[0] << std::endl;
// }


// #include <iostream>
// #include <dirent.h>

// int main()
// {
//     // Open the directory
//     DIR* dir = opendir("/path/to/directory");
//     if (dir == nullptr)
//     {
//         std::cout << "Error opening directory" << std::endl;
//         return 1;
//     }

//     // Read the contents of the directory
//     struct dirent* entry;
//     while ((entry = readdir(dir)) != nullptr)
//     {
//         std::cout << entry->d_name << std::endl;
//     }

//     // Close the directory
//     closedir(dir);

//     return 0;
// }

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PORT "7070"

int main(void) {
  int sockfd;
  struct addrinfo hints, *res, *p;
  int status;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;  // allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // TCP socket
  hints.ai_flags = AI_PASSIVE;  // use wildcard IP address

  // get address information
  status = getaddrinfo(NULL, PORT, &hints, &res);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 1;
  }

  // loop through results and bind to the first available
  for (p = res; p != NULL; p = p->ai_next) {
    // create a socket
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("socket");
      continue;
    }

    // bind the socket to the specified port
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      perror("bind");
      close(sockfd);
      continue;
    }
    break;  // success
  }

  if (p == NULL) {  // no address was successful
    fprintf(stderr, "failed to bind\n");
    return 2;
  }

  // listen for incoming connections
  if (listen(sockfd, 10) == -1) {  // allow up to 10 pending connections
    perror("listen");
    return 3;
  }


// size_t  chunk_len;
    
// chunk_len = 0;
// if (_rlength)
// {
//     chunk_len = _rlength;
    
//     if (chunk.length() < chunk_len)
//         _rlength = chunk_len - chunk.length();
//     else
//         _rlength = 0;
//     _rbody.append(chunk.begin(), chunk.begin() + chunk_len);
// }
// out(chunk_len)

// size_t begin_h = chunk_len;
// while (!_rlength)
// {
//     size_t      pos_h;
//     std::string chunk_len_str;

//     pos_h = chunk_len = chunk.find_first_of("\n", begin_h);
//     if (chunk_len == std::string::npos)
//         return;
//     if (chunk[chunk_len - 1] == '\r')
//         --chunk_len;
        
//     chunk_len_str = chunk.substr(begin_h, chunk_len - begin_h);
//     chunk_len = std::stoi(chunk_len_str, 0, 16);
//     if (!chunk_len)
//     {
//         build_post_response();
//         break;
//     }
    
//     if (chunk.length() < chunk_len + pos_h)
//     {
//         _rlength = chunk_len + pos_h - chunk.length();
//         chunk_len = chunk.length() - pos_h;
//     }
//     else
//     {
//         size_t pos = chunk.find_first_of("\n", chunk_len);
//         if (pos != std::string::npos)
//             begin_h += pos - chunk_len;
//     }
//     _rbody.append(chunk.begin() + pos_h + 1, chunk.begin() + chunk_len + pos_h);
//     begin_h += chunk_len;
    