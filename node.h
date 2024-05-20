#include "container.h"

namespace Approach
{
    namespace Render
    {
        using std::string;
        class Node : public Container
        {
            public:
            string content;

            Node(const string &content) : content(content){};
            Node(string content) : content(content) {};
            Node(): content("") {};
        };
    };
};
