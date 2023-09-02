#ifndef LODManager_h__
#define LODManager_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Grid2.h>

namespace fb
{
    namespace render
    {
        
        class LodManager : public ISharedObject
        {
        public:
            LodManager();
            ~LodManager();

            void addObject( SmartPtr<LodObject> lodObject );
            void removeObject( SmartPtr<LodObject> lodObject );

        protected:
            SmartPtr<LodPage> getPage( const Vector2I &index );

            typedef std::map<Vector2I, SmartPtr<LodPage>> LodPages;
            LodPages m_lodPages;

            Grid2 m_grid;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // LODManager_h__
