![ironServer logo](logo.png)
# IronServer - Documentation

## Table of contents
1. [Server Object](#Server-Object)

	- [Listen](#Listen)
	- [Server Routers Functions](#Server-Routers-Functions)
	- [Static Server](#Static-Server)
2. [Request Object](#Request-Object)

	- [Response* Request::response](#Response*-Request::response)
	- [void Request::setData(string key, string value)](void-Request::setData(string-key,-string-value))
	- [string Request::getData(string key)](#string-Request::getData(string-key))
	- [information about request](#information-about-request)
3. [Response Object](#Response-Object)
	
	- [Request* Response::Request](#Request*-Response::Request)
	- [Response::done](#Response::done)
	- [Sending](#sending)
	- [Set header](#Set-header)
4. [IronServer Namespace](#IronServer-Namespace)

	- [void IronServer::addTemplate()](void-IronServer::addTemplate())
	- [IronServer::internal](IronServer::internal)

## Server Object
The server object stores the server. It can be created with `IronServer::Server myServerName;`
### Listen
`void IronServer::Server::listen(int port=8080)` is a function, that binds and listens on `port`. It will write a message to stderr depending on the success/failure of the function.

Note: Upon execution, it blocks the main thread.
### Server Routers Functions
Server router functions are functions, that decide how a request is handled. When a request is made, it will match every router function to its method and request path, in the same order as they were added. If a match is found, the request and response objects will be passed to the router function. The router functions are stored in `IronServer::Server::callbacks`.

`IronServer::Server::use()` is the method to add router functions, that work with all request methods. The `path`-argument decides, which request path will be matched. `callbackfunc` is a function. It must be compatible with the following template: `void (IronServer::Request &, IronServer:: Response &)`. `additionalInformation` adds additional information to the request, that can then be accessed with `string IronServer::Request::getData(string key)`.

There's also `get()`, `post()`, `del()`, `put()`, and `patch()`. They are all method specific router functions that work the same way. The `path`-argument decides, which request path will be matched. `callbackfunc` is a function. It must be compatible with the following template: `void (IronServer::Request &, IronServer:: Response &)`.

### Static Server
A static server is a server, that serves files statically. It can be initiated using `void IronServer::Server::staticServer(string path = "/", string folder = ".", bool useTemplates = false)`. The path-argument decides, which request path root should be used. For example, if the path is `/static` and the user requests `/static/somefile`, then `somefile` is sent. The `folder`-argument decides, which folder should be used for the server. Note: it is relative to the shell position, not the file position. `useTemplates` decides if templates will be used or not. More documentation about templates: templates
## Request Object
The request object is an object, that contains information about the request and is passed in router functions. 

### Response* Request::response
pointer access to the according response object
### void Request::setData(string key, string value)
set data, that is persistent across multiple router functions.

for example, one could make a router function that parses cookies and setData()s them, and later another function can getData() them.
### string Request::getData(string key)
extract data, that was set using setData()
### information about request
####  IronServer::Request::body
Parsed request body, mainly sent in forms and other POST requests.
####  IronServer::Request::headers
Parsed HTTP body
####  IronServer::Request::httpVersion
HTTP version (usually HTTP/1.1 or HTTP/2.0).
####  IronServer::Request::ip
IPv4 address of request.
####  IronServer::Request::method
HTTP method. (Usually GET or POST	)
####  IronServer::Request::path
Path of HTTP request
####  IronServer::Request::query
Parsed HTTP GET parameters 
####  IronServer::Request::rawBody
Unparsed request body, mainly sent in forms and other POST requests.
####  IronServer::Request::rawHeaders
Unparsed HTTP body
## Response Object
The request object is an object, that provides methods for sending a response to the client and is passed in router functions.
### Request* Response::Request
pointer access to the according request object
### Response::done
if set to true, it will not be passed to any more router functions. Is set automatically when sending something.
### Sending
#### void IronServer::Response::send(string body, int status = 200, std::string httpVersion = "HTTP/1.1")
Sends HTTP body and headers.`body`-argument: http body. `status`: http status. `httpVersion`: http version.
#### void IronServer::Response::sendError(int status)
Send an error such as 404. A txt error will be shown automatically
#### void IronServer::Response::sendFile(file = "index.html", string mime = "auto/detect", bool useTemplates = false)
send file as response. Argument `file`: path to file that will be sent. (Relative to shell, not to server binary location). `mime`: mime type of sent file. By default, autodetect from file extension. useTemplate: use templates or not
#### void IronServer::Response::sendRaw(string response)
as send raw, as in raw TPC string
### Set header
You can set a header that will be sent using `void IronServer::Response::setHeader(string key, string value)`
## IronServer Namespace
### void IronServer::addTemplate()
add a template. The function handleFunc should return a std::string, that will be used. Will be used in `Response::sendFile` (`implicitly also staticSerer`) as `${{{templateName}}}` if enabled.
### IronServer::internal
functions used internally, but also accessible from the outside. Documented in comments.