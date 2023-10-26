#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/LOD/LodPage.h"
#include "FBGraphicsOgreNext/LOD/LodObject.h"

namespace fb
{
    namespace render
    {

        LodPage::LodPage()
        {
        }

        LodPage::~LodPage()
        {
        }

        void LodPage::addObject( SmartPtr<LodObject> lodObject )
        {
            lodObject->setLodPage( this );

            m_lodObjects[lodObject->getId()] = lodObject;
        }

        void LodPage::removeObject( SmartPtr<LodObject> lodObject )
        {
            lodObject->setLodPage( nullptr );
            m_lodObjects.erase( lodObject->getId() );
        }

    }  // namespace render
}  // namespace fb
