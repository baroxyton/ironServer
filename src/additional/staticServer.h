#ifndef STATICSERVERH
#define STATICSERVERH
#include <string>
#include <vector>
using namespace std;
namespace IronServer
{
    namespace internal
    {
        //static server function
        void staticServer(Request &req, Response &res)
        {
            // additional information: path, folder, templates enabled
            string requestPath = req.getData("staticserver_path");
            string folderPath = req.getData("staticserver_folder");
            vector<string> requestPathV, folderPathV, urlPathV;
            //transform string boolean to native c++ boolean
            bool useTemplates = false;
            if (req.getData("staticserver_use_templates") == "true")
            {
                useTemplates = true;
            }
            requestPathV = pathToVector(requestPath);
            folderPathV = pathToVector(folderPath);
            urlPathV = pathToVector(req.path);
            //extract the actual file path from the url path and the router path
            vector<string> realPathV(urlPathV.begin() + (int)requestPathV.size(), urlPathV.end());
            string realPath = folderPath + "/" + joinVector(realPathV, "/");
            //send error page if file doesn't exist
            if (!fileDoesExist(realPath))
            {
                res.sendError(404);
                return;
            }
            //if directory is opened, send index.html. Check if exists again
            if (fileIsDirectory(realPath))
            {
                realPath = realPath + "/index.html";
                if (!fileDoesExist(realPath))
                {
                    res.sendError(404);
                    return;
                }
            }
            string mime = "auto/detect";
            res.sendFile(realPath, mime, useTemplates);
        }
    }
}
#endif