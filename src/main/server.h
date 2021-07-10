#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
#include <iostream>
#include <regex>
#include <thread>
#include <future>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <sys/stat.h>
#include "../internal/filesystem.h"
#include "../internal/vectorOperations.h"
#include "../internal/stringReplacing.h"
#include "../internal/hexDecoders.h"
#include "../internal/stringMatch.h"
#include "../internal/urlDecoders.h"
#include "../internal/requestParser.h"
#include "../internal/errorHandler.h"
#include "../internal/templates.h"
#include "classes.h"
#include "../additional/staticServer.h"
using namespace std;

namespace IronServer
{
    //constructor for function used for callbacks
    typedef void (*callbackfunc)(Request &, Response &);
    //callback are used for routing requests
    struct callback
    {
        string path, method;
        callbackfunc func;
        vector<internal::Param> additionalInformation;
    };
    //server class
    //optional argument: (int)port
    class Server
    {
    private:
        //needed for c server
        int one = 1;
        int client_filedescriptor;
        struct sockaddr_in server_address, client_address;
        socklen_t socketin_address_size = sizeof(client_address);
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        //server start function: ran as thread by class initiator
    public:
        void listen(int port = 8080)
        {
            //SOL_SOCKET: socket level. Allow modifying socket
            //SO_REUSEADDR: reuse local address
            setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
            //AF_INET = ipv4
            server_address.sin_family = AF_INET;
            //accept all incoming addresses
            server_address.sin_addr.s_addr = INADDR_ANY;
            //htons: unsigned int to network bytes
            server_address.sin_port = htons(port);
            //bind to port and listen
            int bindSuccess = bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
            if(bindSuccess != 0){
                cerr << "Error binding to port " << port << endl;
                return;
            }
            int success = ::listen(sock, 5);
            if (success == 0)
            {
                cerr << "Listening on localhost:" << port << "..." << endl;
            }
            else
            {
                cerr << "Error listening on port " << port << endl;
                return;
            }
            //listen forever
            while (true)
            {
                //on request: accept
                client_filedescriptor = accept(sock, (struct sockaddr *)&client_address, &socketin_address_size);
                //stores request safely. Max: 10kb
                char requestBuffer[10000];
                recv(client_filedescriptor, requestBuffer, 10000, 0);
                Request rq{requestBuffer, client_address};
                Response res{client_address, client_filedescriptor};
                rq.response = &res;
                res.request = &rq;
                //loop through callbacks specified by user
                for (int i = 0; i < callbacks.size(); i++)
                {
                    auto currentCallback = callbacks[i];
                    //check if using correct method
                    if (currentCallback.method != "ALL" && currentCallback.method != rq.method)
                    {
                        continue;
                    }
                    //check if using correct path
                    if (!internal::doesMatchRegex(rq.path, internal::URLToRegex(currentCallback.path)))
                    {
                        continue;
                    }
                    //passed tests: processing request
                    rq.setData(currentCallback.additionalInformation);
                    currentCallback.func(rq, res);
                    //res.done is set when a response is sent
                    //after a response is sent, no more callbacks should be done
                    if (res.done)
                    {
                        break;
                    };
                }
                //all callbacks are done, but nothing was sent. Send 404 error
                if (!res.done)
                {
                    res.sendError(404);
                }
            };
        }

        //all callbacks that route requests
        vector<struct callback> callbacks;

        //custom router
        //path: request path
        //func: reference to function with IronServer::Request& and IronServer::Response& arguments
        //additionalInformation: information that is associated with the current function. It will be applied to the request using request.setData() automatically, and can be accessed using request.getData().
        void use(string path, callbackfunc func, vector<internal::Param> additionalInformation = {})
        {
            struct callback currentCallback = {path, "ALL", func, additionalInformation};
            callbacks.push_back(currentCallback);
        }
        //GET request router
        //path: request path
        //func: reference to function with IronServer::Request& and IronServer::Response& arguments
        void get(string path, callbackfunc func)
        {
            struct callback currentCallback = {path, "GET", func};
            callbacks.push_back(currentCallback);
        };
        //POST request router
        //path: request path
        //func: reference to function with IronServer::Request& and IronServer::Response& arguments
        void post(string path, callbackfunc func)
        {
            struct callback currentCallback = {path, "POST", func};
            callbacks.push_back(currentCallback);
        }
        //DELETE request router
        //path: request path
        //func: reference to function with IronServer::Request& and IronServer::Response& arguments
        void del(string path, callbackfunc func)
        {
            struct callback currentCallback = {path, "DELETE", func};
            callbacks.push_back(currentCallback);
        }
        //PUT request router
        //path: request path
        //func: reference to function with IronServer::Request& and IronServer::Response& arguments
        void put(string path, callbackfunc func)
        {
            struct callback currentCallback = {path, "PUT", func};
            callbacks.push_back(currentCallback);
        }
        //PATCH request router
        //path: request path
        //func: reference to function with IronServer::Request& and IronServer::Response& arguments
        void patch(string path, callbackfunc func)
        {
            struct callback currentCallback = {path, "PATCH", func};
            callbacks.push_back(currentCallback);
        }
        //Static Server router
        //path: request path
        //folder: static server folder
        //useTemplates: use templates or not
        void staticServer(string path = "/", string folder = ".", bool useTemplates = false)
        {
            //turn string representation of boolean into native c++ boolean
            string useTemplatesStr = "false";
            if (useTemplates)
            {
                useTemplatesStr = "true";
            }
            struct callback currentCallback = {path, "GET", internal::staticServer, vector<internal::Param>{{"staticserver_path", path}, {"staticserver_folder", folder}, {"staticserver_use_templates", useTemplatesStr}}};
            callbacks.push_back(currentCallback);
        };
    };
};