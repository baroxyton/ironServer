#ifndef CLASSESH
#define CLASSESH
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../internal/requestParser.h"
#include "../internal/filesystem.h"
#include "../internal/templates.h"
#include "../internal/errorHandler.h"
#include "../internal/templates.h"
#include "../internal/detectMime.h"
using namespace std;
namespace IronServer
{
    //predeclare classes
    class Request;
    class Response;
    //request object, contains information about a request.
    class Request
    {
    private:
        //to store information accross multiple router functions
        vector<internal::Param> additionalData;

    public:
        //pointer access to Response object
        Response *response;
        string ip, rawHeaders, rawBody, httpVersion, method, path;
        vector<internal::Header> headers;
        vector<internal::Param> body, query;
        Request(string request, struct sockaddr_in client_address)
        {
            ip = inet_ntoa(client_address.sin_addr);
            struct internal::parsedRequest parsedrq = internal::parseRequest(request);
            rawHeaders = parsedrq.rawHeaders;
            rawBody = parsedrq.rawBody;
            httpVersion = parsedrq.httpVersion;
            method = parsedrq.method;
            path = parsedrq.path;
            headers = parsedrq.headers;
            query = parsedrq.parsedGetParams;
            if (!rawBody.empty())
            {
                body = internal::parseGetParams(rawBody);
            }
        }
        //set data, that is persistent across multiple router functions.
        //for example, one could make a router function that parses cookies and setData()s them, and later another function can getData() them.
        void setData(string dataName, string data)
        {
            //loop through vector, if finds correct data: set and exit.
            for (int i = 0; i < additionalData.size(); i++)
            {
                if (additionalData[i].name == dataName)
                {
                    additionalData[i].value = data;
                    return;
                }
            }
            //didn't exit, meaning it wasn't set. Add new pair
            additionalData.push_back(internal::Param{dataName, data});
            return;
        }
        //repeated setData(string,string)
        void setData(vector<internal::Param> params)
        {
            for (int i = 0; i < params.size(); i++)
            {
                setData(params[i].name, params[i].value);
            }
        }
        //extract data, that was set using setData()
        string getData(string dataName)
        {
            //loop through vector, looking for correct data pair
            for (int i = 0; i < additionalData.size(); i++)
            {
                if (additionalData[i].name == dataName)
                {
                    return additionalData[i].value;
                }
            }
            //wasn't found
            return "null";
        }
    };
    //response object, can be used to send a response
    class Response
    {
    private:
        //needed for c socket
        string ip;
        int status;
        int client_fd;

    public:
        //pointer access to Request object
        Request *request;
        //will be set to true when the request is done, blocking the next router function from seeing it. Set after sending
        bool done = false;
        vector<internal::Header> headers;
        Response(sockaddr_in client_address, int client_filedescriptor)
        {
            //needed for c socket
            ip = inet_ntoa(client_address.sin_addr);
            client_fd = client_filedescriptor;
        };
        //set HTTP header that will be sent.
        void setHeader(string key, string value)
        {
            internal::Header customHeader;
            customHeader.name = key;
            customHeader.value = value;
            headers.push_back(customHeader);
        }
        //send raw, as in raw TPC string
        void sendRaw(string response)
        {
            //write to socket using file descriptor of client
            write(client_fd, response.c_str(), response.size());
            close(client_fd);
            //skip further router functions
            done = true;
        };
        //send response, with set headers and body
        void send(string body, int status = 200, string httpVersion = "HTTP/1.1")
        {
            headers.push_back(internal::Header{"Content-Length", to_string(body.size())});
            string response = createResponse(headers, body, httpVersion, status);
            sendRaw(response);
        };
        //send file as response
        //string file: path to file
        void sendFile(string file = "index.html", string mime = "auto/detect", bool useTemplates = false)
        {
            if (mime == "auto/detect")
            {
                mime = internal::extensionToMime(file);
            }
            string fileToSend = internal::readFile(file);
            //if templates are enabled: loop through templates and find correct one.
            if (useTemplates)
            {
                for (int i = 0; i < internal::templates.size(); i++)
                {
                    internal::TemplateHolder currentTemplate = internal::templates[i];
                    string templateString = "${{{" + currentTemplate.templateName + "}}}";
                    //replace every instance of ${{{templateName}}} with output of template function
                    while (fileToSend.find(templateString) != string::npos)
                    {
                        fileToSend = internal::stringReplace(fileToSend, templateString, currentTemplate.templateFunc(*request, currentTemplate.templateName));
                    }
                }
            }
            setHeader("Content-Type", mime);
            send(fileToSend);
        }
        //send http error (eg. 404)
        void sendError(int code)
        {
            //int to string
            string stringCode = to_string(code);
            //string that will later be sent
            string returnString;
            //look for correct error message
            for (int i = 0; i < internal::errorMessages.size(); i++)
            {
                if (internal::errorMessages[i].name == stringCode)
                {
                    returnString = internal::errorMessages[i].value;
                }
            }
            //unknown status code. Send generic error message
            if (returnString.empty())
            {
                returnString = "Error: " + stringCode;
            }
            //send it
            send(returnString, code);
        }
    };
}
#endif