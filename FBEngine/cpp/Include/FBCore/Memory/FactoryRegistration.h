#ifndef FactoryRegistration_h__
#define FactoryRegistration_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    template <class T>
    class FactoryRegistration
    {
    public:
        FactoryRegistration()
        {
            T::registerFactory();
        }

        ~FactoryRegistration()
        {
            T::unregisterFactory();
        }
    };
}  // end namespace fb

#endif  // FactoryRegistration_h__
