#ifndef REQUESTPARSERH
#define REQUESTPARSERH
#include <string>
#include <regex>
#include <iostream>
#include <vector>
#include "urlDecoders.h"
#include "vectorOperations.h"
using namespace std;
namespace IronServer
{
    namespace internal
    {
        //used to store http headers
        class Header
        {
        public:
            string name;
            string value;
            Header(string namearg = "", string valuearg = "") : name(namearg), value(valuearg){};
        };
        //used to store get request parameters and various other string/string pares
        class Param
        {
        public:
            string name;
            string value;
            Param(string namearg = "", string valuearg = "") : name(namearg), value(valuearg){};
        };
        //parse get params
        //example: hello=world&success=true results in vector<Param>{Param{"hello","world"},Param{"success","true"}}
        vector<Param> parseGetParams(string raw)
        {
            vector<Param> result;
            vector<string> rawParam = splitAll(raw, "&");
            for (int i = 0; i < rawParam.size(); i++)
            {
                string currentParam = rawParam[i];
                vector<string> what = splitAll(currentParam, "=");
                //prevent segmentation fault from too small vector
                if (what.size() >= 2)
                {
                    result.push_back(Param{unescapeURL(what[0]), unescapeURL(what[1])});
                }
            }
            return result;
        }
        //struct to store fully parsed request
        struct parsedRequest
        {
            string rawHeaders, rawBody, httpVersion, method, path;
            vector<Header> headers;
            vector<Param> parsedGetParams;
        };
        //create a response using headers, body and status line components
        string createResponse(vector<Header> headers, string body, string httpVersion = "HTTP/1.1", int status = 200)
        {
            //generate status line
            // \r\n is the correct header seperator
            string response = httpVersion + " " + to_string(status) + "\r\n";
            //generate & append headers
            for (int i = 0; i < headers.size(); i++)
            {
                response += headers[i].name + ": " + headers[i].value + "\r\n";
            }
            //append body, return
            response += "\r\n" + body;
            return response;
        }
        //parse headers, extraced from the clients request
        vector<Header> parseHeaders(string rawHeadersArg)
        {
            vector<string> splitHeaders = splitAll(rawHeadersArg, "\r\n");
            vector<Header> parsedHeaders;
            for (int i = 0; i < splitHeaders.size(); i++)
            {
                string name, value;
                string header = splitHeaders[i];
                if (i == 0)
                {
                    name = "header";
                    value = header;
                }
                else
                {
                    if (header.find(": ") != string::npos)
                    {
                        name = header.substr(0, header.find(": "));
                        value = header.substr(header.find(": ") + 2, header.size());
                    }
                }
                parsedHeaders.push_back(Header{name, value});
            }
            return parsedHeaders;
        }
        //put previous components together to parse the request
        struct parsedRequest parseRequest(string rawInput)
        {
            string rawHeaders, rawBody, rawQuery;
            //check if the request has a body (has body/header seperator that isn't at the end)
            if (rawInput.find("\r\n\r\n") == string::npos || rawInput.find("\r\n\r\n") == rawInput.size() - 4)
            {
                //no body: procceed
                rawHeaders = rawInput;
            }
            else
            {
                //has body: seperate header and body
                rawHeaders = rawInput.substr(0, rawInput.find("\r\n\r\n"));
                rawBody = rawInput.substr(rawInput.find("\r\n\r\n"), rawInput.size());
            }
            vector<Header> parsedHeaders = parseHeaders(rawHeaders);
            //extract status line and remove it from header vector
            string rawStatus = parsedHeaders[0].value;
            parsedHeaders.erase(parsedHeaders.begin());
            //regex explained: a word (request method), followed by a space, followed by anything (request path), followed by a space, followed by a string of
            regex statusParser("^(\\w+)\\s(.*?)\\s(([a-zA-Z0-9./]){1,})$");
            std::smatch regexMatch;
            //extract the method, path and http-version from the status line
            //if the status line is invalid, replace to prevent vector segfault
            bool hasMatch = regex_search(rawStatus, regexMatch, statusParser);
            if (!hasMatch)
            {
                rawStatus = "GET /error/ HTTP/1.0";
                regex_search(rawStatus, regexMatch, statusParser);
            }
            string method = regexMatch[1];
            string path = regexMatch[2];
            vector<string> splitPath = pathToVector(path);
            for(int i = 0; i < splitPath.size(); i++){
                string pathPart = splitPath[i];
                //prevent path traversial
                if(pathPart == ".."||pathPart == "."){
                    path = "/error/";
                }
            }
            string httpVersion = regexMatch[3];
            vector<Param> query;
            //includes "?": Parse GET parameters
            if (path.find("?") != string::npos)
            {
                vector<string> splitPath = splitAll(path, "?");
                path = unescapeURL(splitPath[0]);
                query = parseGetParams(splitPath[1]);
            }
            struct parsedRequest result = {rawHeaders, rawBody, httpVersion, method, path, parsedHeaders, query};
            return result;
        }
    }
}
#endif