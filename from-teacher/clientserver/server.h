// ------------------------------------------------------------------
//
//                 Client/Server communication package
//
//                         Server header file
//
// Change log
//  950323  RH  Initial version
//  951212  RH  Modified to allow subclassing of class Connection
//  970127  RH  Changed "private" to "protected"
//  990125  PH  Changed function names: Read -> read, etc.
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

#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include "connection.h"

namespace client_server {
    /* A server listens to a port and handles multiple connections */
    class Server {
        friend class Connection;
        
    public:
        /* Create a server that listens to the port 'port' */
        Server(int port);
        
        /* Delete all registered connections */
        virtual ~Server();
        
        /* Returns true if the server has been initialized correctly */
        bool isReady() const;
        
        /* Wait for activity on the port. Returns a previously registered
        connection object if an "old" connection wishes to communicate,
        0 if a new client wishes to communicate */
        Connection* waitForActivity() const;
        
        /* Register a new connection */
        void registerConnection(Connection* conn);
        
        /* Deregister a connection (nothing happens if conns isn't 
        registered */
        void deregisterConnection(Connection* conn);
        
    protected:
        /* The number of the communication socket */
        int my_socket;

        /* Number of registered connections */
        int no_of_connections;
        
        /* List of registered connections */
        std::vector<Connection*> connections;
        
        /* ? */
        mutable int pending_socket;
        
        /* Print error message and exit */
        void error(const char* msg) const;
    };
}

#endif
