#ifndef __FBCore_PropertiesUtil_h__
#define __FBCore_PropertiesUtil_h__

#include <FBCore/Base/Properties.h>
#include <FBCore/Base/DataBinding.h>
#include <FBCore/Base/DataMacros.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    class PropertiesUtil
    {
    public:
        template <class T>
        static SmartPtr<Properties> getProperties( T *ptr );

        template <class T>
        static void fromProperties( SmartPtr<Properties> properties, T *ptr );

        template <class X>
        static void bind( Properties *properties, bool from, Array<X> *ANI, const char *tagname );

        static void bind( Properties *properties, bool from, Array<data::property> *ANI,
                          const char *tagname );
    };
}  // end namespace fb

#include <FBCore/Base/PropertiesUtil.inl>

#endif  // PropertiesUtil_h__
