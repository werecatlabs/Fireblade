#ifndef MessageBox_h__
#define MessageBox_h__

#include <FBCore/Core/StringTypes.h>

namespace fb
{

    /** Used to display a message box on supported platforms. */
    class MessageBoxUtil
    {
    public:
        /** Shows a message box with the given text. 
        @param text The text to display.
        */
        static void show( const String &text );

        /** Shows a message box with the given text. 
        @param text The text to display.
        */
        static void show( const char *text );
    };

}  // end namespace fb

#endif  // MessageBox_h__
