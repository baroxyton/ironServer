#ifndef TEMPLATESH
#define TEMPLATESH
#include <string>
#include <vector>
using namespace std;
namespace IronServer
{
    //predeclare class
    class Request;
    namespace internal
    {
        //handles templates in static servers
        typedef string (*TemplateHandler)(Request &, string);
        //class for storing templates
        class TemplateHolder
        {
        public:
            string templateName;
            TemplateHandler templateFunc;
            TemplateHolder(string nameArg, TemplateHandler funcArg)
            {
                templateName = nameArg;
                templateFunc = funcArg;
            }
        };
        //holds all templates that are currently in use
        vector<TemplateHolder> templates;
    }
    //add a template. The function handleFunc should return a std::string, that will be used.
    //Will be used in Response::sendFile as ${{{templateName}}} if enabled.
    void addTemplate(string templateName, internal::TemplateHandler handleFunc)
    {
        internal::TemplateHolder handler{templateName, handleFunc};
        internal::templates.push_back(handler);
    };
}
#endif