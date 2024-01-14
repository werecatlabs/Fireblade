#ifndef MessageBox_h__
#define MessageBox_h__

#include <FBCore/Core/StringTypes.h>

namespace fb
{

    class MessageBoxUtil
    {
    public:
        static void show( const String &text );
        static void show( const char *text );
    };

}  // end namespace fb

#endif  // MessageBox_h__
