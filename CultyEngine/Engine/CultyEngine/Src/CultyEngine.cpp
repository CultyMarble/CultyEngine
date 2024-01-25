#include "Precompiled.h"
#include "CultyEngine.h"

namespace CultyEngine
{
    Application& MainApplication()
    {
        static Application sApplication;
        return sApplication;
    }
}