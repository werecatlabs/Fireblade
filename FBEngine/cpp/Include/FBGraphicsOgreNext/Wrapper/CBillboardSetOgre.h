#ifndef _CBillboardSet_H
#define _CBillboardSet_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include "FBCore/Interface/Graphics/IBillboardSet.h"
#include "FBCore/Interface/Graphics/IGraphicsScene.h"
#include "FBGraphicsOgreNext/Wrapper/CGraphicsObjectOgreNext.h"
#include "FBGraphicsOgreNext/Wrapper/CBillboardOgre.h"
#include <vector>
#include "FBCore/Core/Pool.h"
#include <list>

namespace fb
{
    namespace render
    {
        //--------------------------------------------
        class CBillboardSetOgre : public CGraphicsObjectOgreNext<IBillboardSet>
        {
        public:
            CBillboardSetOgre( SmartPtr<IGraphicsScene> creator );
            ~CBillboardSetOgre() override;

            void initialise( Ogre::v1::BillboardSet *bbSet );
            
            void setMaterialName( const String &materialName, s32 index = -1 ) override;
            String getMaterialName( s32 index = -1 ) const override;

            /** @copydoc IBillboardSet::setMaterial */
            void setMaterial( SmartPtr<IMaterial> material, s32 index = -1 ) override;

            /** @copydoc IBillboardSet::getMaterial */
            SmartPtr<IMaterial> getMaterial( s32 index = -1 ) const override;

            void setRenderQueueGroup( u8 renderQueue );

            SmartPtr<IGraphicsObject>
            clone( const String &name = StringUtil::EmptyString ) const override;

            void _getObject( void **ppObject ) const override;

            // IBillboardSet functions
            void clear() override;

            SmartPtr<IBillboard> createBillboard( const Vector3F &position = Vector3F::zero() ) override;
            bool removeBillboard( SmartPtr<IBillboard> billboard ) override;

            void setCullIndividually( bool cullIndividually ) override;
            void setSortingEnabled( bool sortingEnabled ) override;
            void setUseAccurateFacing( bool useAccurateFacing ) override;

            void setBounds( const AABB3F &box, f32 radius ) override;

            void setDefaultDimensions( const Vector2F &dimension );

            void setDefaultDimensions( const Vector3F &dimension ) override;

        protected:
            template <class T>
            void expandPool( Pool<CBillboardPtr> &pool, u32 numNewElements );

            Array<SmartPtr<IBillboard>> getBillboards() const override;

            void handleEvent( SmartPtr<IEvent> event ) ;

            Ogre::v1::BillboardSet *m_bbSet;
            SmartPtr<IGraphicsScene> m_creator;
            String m_name;

            using BillboardList = std::list<CBillboardPtr>;
            BillboardList m_bbs;

            Pool<CBillboardPtr> m_billboardPool;

            SpinRWMutex ( BBSetMutex );
        };
    } // end namespace render
}     // end namespace fb

#endif
