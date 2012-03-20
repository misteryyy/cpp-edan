// ------------------------------------------------------------------
//
//                 Client/Server communication package
//
//                      Server implementation file
//
// Change log
//  950323  RH  Initial version
//  951212  RH  Modified to allow subclassing of class Connection
//  970127  RH  Added extra include to make the file compile under Linux
//  990125  PH  Changed function names: Read -> read, ...
//  000114  PH  int -> bool, virtual destructors, other minor changes
//  010129  PH  added void type to initConnection
//  011212  PH  changed char* arguments to const char*
//              changed connection closed handling to exception
//              unsigned char instead of char/int in write/read
//  020102  PH  split into separate file for each class
//  040421  PH  added namespace, new casts, cleaned up a lot
//  050113  PH  added deregisterConnection, new registration (vector),
//              added check for server shutdown, many more changes
//
// ------------------------------------------------------------------

#include "server.h"

#include <iostream>
#include <algorithm>
#include <sys/types.h>   /* socket(), bind(), select() */
#include <sys/socket.h>  /* socket(), bind(), getsockname(), listen() */
#include <arpa/inet.h>   /* ntohs() */
#include <unistd.h>      /* close(), select() */
#include <sys/time.h>    /* select() */
#include <netinet/in.h>  /* sockaddr_in */

namespace client_server {
    Server::Server(int port) {
        no_of_connections = 0;
        pending_socket    = -1;
        
        my_socket = socket(AF_INET,SOCK_STREAM, 0);
        if (my_socket < 0)
            my_socket = -1;
        else {
            sockaddr_in server;
            server.sin_family      = AF_INET;
            server.sin_addr.s_addr = INADDR_ANY;
            server.sin_port        = htons(port);
            if (bind(my_socket, reinterpret_cast<sockaddr*>(&server), 
                     sizeof(server)) < 0)
                my_socket = -1;
            else {
                int length = sizeof(server);
                if (getsockname(my_socket, 
                                reinterpret_cast<sockaddr*>(&server),
                                reinterpret_cast<socklen_t*>(&length)) < 0)
                    my_socket = -1;
                else {
                    if (ntohs(server.sin_port) != port)
                        my_socket = -1;
                    else
                        listen(my_socket, 5);
                }
            }
        }
    }
    
    Server::~Server() {
        for (size_t i = 0; i < connections.size(); ++i) 
            delete connections[i];
        close(my_socket);
        my_socket = -1;
    }
    
    bool Server::isReady() const {
        return my_socket != -1;
    }
    
    Connection* Server::waitForActivity() const {
        if (my_socket == -1)
            error("WaitForActivity attempted on a not "
                  "properly opened Server");
        
        Connection* return_conn = 0;
        bool do_exit = false;
        while (! do_exit) {
            fd_set read_template;
            
            FD_ZERO(&read_template);
            FD_SET(my_socket, &read_template);
            for (size_t i = 0; i < connections.size(); ++i)
                FD_SET(connections[i]->getSocket(), &read_template);
            
            select(FD_SETSIZE, &read_template, 0, 0, 0);
            if (FD_ISSET(my_socket, &read_template)) {
                if (no_of_connections < 50) {
                    int new_socket = accept(my_socket, 0, 0);
                    if (new_socket != -1) {
                        if (pending_socket != -1)
                            error("waitForActivity: registerConnection "
                                  "not called to process previous connection from a client");
                        pending_socket = new_socket;
                        do_exit = true;
                    }
                } 
            }
            else {
                size_t i = 0;
                while (i < connections.size() && 
                       ! FD_ISSET(connections[i]->getSocket(), 
                                  &read_template))
                    ++i;
                return_conn = i < connections.size() ? connections[i] : 0;
                do_exit = true;
            }
        }
        return return_conn;
    }
    
    void Server::registerConnection(Connection* conn) {
        if (conn->getSocket() != -1)
            error("registerConnection called with a busy "
                  "Connection as parameter");
        if (pending_socket == -1)
            error("registerConnection called even though no "
                  "client is trying to connect");
        conn->initConnection(pending_socket);
        connections.push_back(conn);
        no_of_connections++;
        pending_socket = -1;
    }
    
    void Server::deregisterConnection(Connection* conn) {
        connections.erase(std::remove(connections.begin(), 
                                      connections.end(), 
                                      conn),
                          connections.end());
        no_of_connections--;
    }
    
    void Server::error(const char* msg) const{
        std::cerr << "Class Server: " << msg << std::endl;
        exit(1);
    }
}
