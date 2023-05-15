#ifndef LodPage_h__
#define LodPage_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace render
    {
        
        class LodPage : public CSharedObject<ISharedObject>
        {
        public:
            LodPage();
            ~LodPage();

            void addObject( SmartPtr<LodObject> lodObject );
            void removeObject( SmartPtr<LodObject> lodObject );

        protected:
            typedef std::map<std::string, Ogre::InstanceManager *> InstanceManagers;
            InstanceManagers m_instanceManagers;

            typedef std::map<u32, SmartPtr<LodObject>> LodObjects;
            LodObjects m_lodObjects;
        };

    }  // namespace render
}  // end namespace fb

#endif  // LodPage_h__
