#ifndef FBOISKeyConverter_h__
#define FBOISKeyConverter_h__

#include <FBCore/Core/Singleton.h>
#include <FBCore/Core/StringTypes.h>
#include <OISKeyboard.h>

namespace fb
{
    class OISKeyConverter : public Singleton<OISKeyConverter>
    {
    public:
        OISKeyConverter();
        ~OISKeyConverter();

        OIS::KeyCode getCodeFromString( const String &key ) const;

    private:
        using Keys = std::map<String, OIS::KeyCode>;
        Keys keys;
    };
}  // end namespace fb

#endif  // FBOISKeyConverter_h__
