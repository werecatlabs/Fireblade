#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CBillboardSetOgre.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <OgreBillboardSet.h>
#include <OgreSceneManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        CBillboardSetOgre::CBillboardSetOgre( SmartPtr<IGraphicsScene> creator ) : m_creator( creator )
        {
        }

        CBillboardSetOgre::~CBillboardSetOgre()
        {
            SmartPtr<CSceneManagerOgreNext> smgr;  // = m_creator;
            Ogre::SceneManager *ogreSmgr = smgr->getSceneManager();
            ogreSmgr->destroyBillboardSet( m_bbSet );

            smgr->removeGraphicsObject( this );
        }

        void CBillboardSetOgre::initialise( Ogre::v1::BillboardSet *bbSet )
        {
            m_bbSet = bbSet;
            expandPool<CBillboardOgre>( m_billboardPool, m_bbSet->getPoolSize() );
        }

        void CBillboardSetOgre::setMaterialName( const String &materialName, s32 index )
        {
            // m_bbSet->setMaterialName(materialName.c_str());
        }

        String CBillboardSetOgre::getMaterialName( s32 index ) const
        {
            return StringUtil::EmptyString;
        }

        void CBillboardSetOgre::setMaterial( SmartPtr<IMaterial> material, s32 index /*= -1*/ )
        {
        }

        SmartPtr<IMaterial> CBillboardSetOgre::getMaterial( s32 index /*= -1*/ ) const
        {
            return nullptr;
        }

        void CBillboardSetOgre::setRenderQueueGroup( u8 renderQueue )
        {
            m_bbSet->setRenderQueueGroup( renderQueue );
        }

        SmartPtr<IGraphicsObject> CBillboardSetOgre::clone( const String &name ) const
        {
            return nullptr;
        }

        void CBillboardSetOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_bbSet;
        }

        void CBillboardSetOgre::clear()
        {
            m_bbSet->clear();
            m_bbs.clear();
        }

        SmartPtr<IBillboard> CBillboardSetOgre::createBillboard( const Vector3F &position )
        {
            Ogre::v1::Billboard *bb =
                m_bbSet->createBillboard( Ogre::Vector3( position.X(), position.Y(), position.Z() ) );

            CBillboardPtr *pBillboard;  // = m_billboardPool.getFreeElement();
            if( !pBillboard )
            {
                expandPool<CBillboardOgre>( m_billboardPool, 100 );
                //pBillboard = m_billboardPool.getFreeElement();
            }

            CBillboardPtr billboard = *pBillboard;

            // billboard->initialise(bb);

            {
                SpinRWMutex::ScopedLock lock( BBSetMutex );
                m_bbs.push_back( billboard );
            }

            return billboard;
        }

        bool CBillboardSetOgre::removeBillboard( SmartPtr<IBillboard> billboard )
        {
            {
                Ogre::Billboard *bb = nullptr;
                billboard->_getObject( (void **)&bb );
                // m_bbSet->removeBillboard(bb);
            }

            {
                SpinRWMutex::ScopedLock lock( BBSetMutex );
                auto it = m_bbs.begin();
                for( ; it != m_bbs.end(); ++it )
                {
                    CBillboardPtr &bb = *it;
                    if( bb == billboard )
                    {
                        m_bbs.erase( it );
                        break;
                    }
                }
            }

            return true;
        }

        void CBillboardSetOgre::setCullIndividually( bool cullIndividually )
        {
            m_bbSet->setCullIndividually( cullIndividually );
        }

        void CBillboardSetOgre::setSortingEnabled( bool sortingEnabled )
        {
            m_bbSet->setSortingEnabled( sortingEnabled );
        }

        void CBillboardSetOgre::setUseAccurateFacing( bool useAccurateFacing )
        {
            m_bbSet->setUseAccurateFacing( useAccurateFacing );
        }

        void CBillboardSetOgre::setBounds( const AABB3F &box, f32 radius )
        {
            Ogre::AxisAlignedBox ogreBox;  // (Ogre::Vector3(box.Min), Ogre::Vector3(box.Max));
            // m_bbSet->setBounds(ogreBox, radius);
        }

        void CBillboardSetOgre::setDefaultDimensions( const Vector2F &dimension )
        {
            m_bbSet->setDefaultDimensions( dimension.X(), dimension.Y() );
        }

        void CBillboardSetOgre::setDefaultDimensions( const Vector3F &dimension )
        {
        }

        template <class T>
        void CBillboardSetOgre::expandPool( Pool<CBillboardPtr> &pool, u32 numNewElements )
        {
            // expand pool
            u32 numAllocated = 0;  //pool.getNumAllocated();
            //pool.setNumElements( numAllocated + numNewElements );
            u32 newNumAllocated = 0;  //pool.getNumAllocated();
            for( u32 i = 0; i < newNumAllocated; ++i )
            {
                /*			if(!pool[i])
                            {
                                SmartPtr<T> message = new T;
                                pool[i] = message;
                                RemoveReference(message);
                            }*/
            }
        }

        Array<SmartPtr<IBillboard>> CBillboardSetOgre::getBillboards() const
        {
            return Array<SmartPtr<IBillboard>>();
        }

        void CBillboardSetOgre::handleEvent( SmartPtr<IEvent> event )
        {
        }
    }  // end namespace render
}  // end namespace fb
