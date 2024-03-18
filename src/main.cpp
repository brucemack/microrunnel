#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <arpa/inet.h>

#define PORT 8100

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Client {
    int fd;
    sockaddr_in addr;
    bool dead = false;
};

int main(int argc, const char** argv) {

    std::vector<Client> clients;

    // Open a socket to receive client connections
    struct sockaddr_in serverAddr; 
   
    // socket create and verification 
    int serverFd = socket(AF_INET, SOCK_STREAM, 0); 
    if (serverFd == -1) { 
        cerr << "Server socket creation failed" << endl;
        return -1;
    } 

    bzero(&serverAddr, sizeof(serverAddr));    
    // assign IP, PORT 
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serverAddr.sin_port = htons(PORT); 
   
    // Binding newly created socket to given IP and verification 
    if ((bind(serverFd, (sockaddr*)&serverAddr, sizeof(serverAddr))) != 0) { 
        cerr << "Server socket bind failed" << endl;
        return -2;
    } 
   
    // Now server is ready to listen and verification 
    if ((listen(serverFd, 5)) != 0) { 
        cerr << "Server socket listen failed" << endl;
        return -2;
    } 
   
    // Make the listening socket non-blocking
    int flags = fcntl(serverFd, F_GETFL, 0);
    if (flags == -1) return -1;
    flags = (flags | O_NONBLOCK);
    fcntl(serverFd, F_SETFL, flags);

    int waitms = 2;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = waitms * 1000;
    fd_set rfds, wfds;

    while (true) {

        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_SET(serverFd, &rfds);

        // Add clients
        int maxFd = serverFd;
        for (const Client& client : clients) {
            maxFd = std::max(maxFd, client.fd);
            FD_SET(client.fd, &rfds);
        }

        int active = select(maxFd + 1, &rfds, &wfds, NULL, &tv);
        if (active > 0) {
            // Check for inbound requests
            if (FD_ISSET(serverFd, &rfds)) {

                struct sockaddr_in clientAddr; 
                int addrLen = sizeof(clientAddr);
                int clientFd = accept(serverFd, (sockaddr*)&clientAddr, &addrLen); 
                if (clientFd < 0) { 
                    cout << "Client accept failed" << endl;
                } 
                else {
                    // Make client non-blocking
                    int flags = fcntl(clientFd, F_GETFL, 0);
                    flags = (flags | O_NONBLOCK);
                    fcntl(serverFd, F_SETFL, flags);

                    clients.push_back({ clientFd, clientAddr });
                    char buf[32];
                    inet_ntop(AF_INET, &(clients.at(0).addr.sin_addr), buf, 32);
                    cout << "Client from " << buf << endl;
                }
            }

            // Check all clients
            for (Client& client : clients) {
                if (FD_ISSET(client.fd, &rfds)) {
                    char data[256];
                    int rc = read(client.fd, data, 256);
                    if (rc == 0)                     {
                        cout << "Client disconnected" << endl;
                        client.dead = true;
                        close(client.fd);
                    }
                }
            }
        }

        // Cleanup dead sockets
        if (!clients.empty()) {
            clients.erase(
                std::remove_if(
                    clients.begin(), 
                    clients.end(),
                    [](const Client& x) { return x.dead; }
                ),
                clients.end()
            );
        }
    }
}
