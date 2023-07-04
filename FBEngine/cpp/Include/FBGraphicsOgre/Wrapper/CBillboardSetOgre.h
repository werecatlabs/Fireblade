#ifndef _CBillboardSet_H
#define _CBillboardSet_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IBillboardSet.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsObjectOgre.h>
#include <FBGraphicsOgre/Wrapper/CBillboardOgre.h>
#include <FBCore/Core/Pool.h>
#include <vector>
#include <list>

namespace fb
{
    namespace render
    {
        class CBillboardSetOgre : public CGraphicsObjectOgre<IBillboardSet>
        {
        public:
            CBillboardSetOgre() = default;
            CBillboardSetOgre( SmartPtr<IGraphicsScene> creator );
            ~CBillboardSetOgre() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::BillboardSet *bbSet );

            // IGraphicsObject functions
            void detachFromParent();
            void attachToParent( SmartPtr<ISceneNode> parent );

            void setMaterialName( const String &materialName, s32 index = -1 ) override;
            String getMaterialName( s32 index = -1 ) const override;

            void setCastShadows( bool castShadows ) override;
            bool getCastShadows() const override;

            void setReceiveShadows( bool recieveShadows ) override;
            bool getReceiveShadows() const override;

            void setVisible( bool isVisible ) override;
            bool isVisible() const override;

            void setRenderQueueGroup( u8 renderQueue );

            void setVisibilityFlags( u32 flags ) override;
            u32 getVisibilityFlags() const override;

            SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const override;

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
            void expandPool( Pool<SmartPtr<CBillboardOgre>> &pool, u32 numNewElements );

            Array<SmartPtr<IBillboard>> getBillboards() const override;

            void handleEvent( SmartPtr<IEvent> event );

            void setZOrder( u32 zOrder ) override;

            u32 getZOrder() const override;

            AABB3F getLocalAABB() const override;

            void setLocalAABB( const AABB3F &val ) override;

            Ogre::BillboardSet *m_bbSet = nullptr;
            SmartPtr<IGraphicsScene> m_creator;
            String m_name;

            using BillboardList = std::list<SmartPtr<CBillboardOgre>>;
            BillboardList m_bbs;

            Pool<SmartPtr<CBillboardOgre>> m_billboardPool;

            mutable SpinRWMutex BBSetMutex;
        };
    }  // namespace render
}  // end namespace fb

#endif
