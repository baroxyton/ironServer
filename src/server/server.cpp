/*
//  read the docs, and begin modifying the server!
//
*/
#include "../main/server.h"
int viewCount = 0;
string countTemplate(IronServer::Request &request, string templateName)
{
    viewCount++;
    return to_string(viewCount);
}
int main()
{
    int port = 8080;
    IronServer::Server myserver;
    IronServer::addTemplate("view_count", countTemplate);
    myserver.staticServer("/", "./build/static", true);
    myserver.listen(port);
}