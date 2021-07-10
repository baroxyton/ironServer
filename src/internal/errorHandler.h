#ifndef ERRORHANDLERH
#define ERRORHANDLERH
#include <string>
#include <vector>
#include "requestParser.h"
using namespace std;
namespace IronServer{
        namespace internal
    {
        //messages sent when using Request::sendError(int)
        vector<Param> errorMessages{
            Param{"400", "Error 400: Bad Request"},
            Param{"404", "Error 404: Not found"},
            Param{"401", "Error 401: Unauthorized"},
            Param{"500", "Error 500: Internal Server Error"},
            Param{"200", "Error 200: Task Failed Successfully"},
            Param{"403", "Error 403: Forbidden"},
            Param{"301", "Error 301: Moved Permanently"},
            Param{"418", "Error 418: I'm a teapot"},
        };
        //set custom message for http status
        void setErrorMessage(string errorCode, string errorMessage)
        {
            //loop through error message vector. If correct item is found, set desired message and exit
            for (int i = 0; i < errorMessages.size(); i++)
            {
                if (errorMessages[i].name == errorCode)
                {
                    errorMessages[i].value = errorMessage;
                    return;
                }
            }
            //still in function, meaning wasn't found: add new item to vector
            errorMessages.push_back(Param({errorCode, errorMessage}));
            return;
        }
    }
}
#endif