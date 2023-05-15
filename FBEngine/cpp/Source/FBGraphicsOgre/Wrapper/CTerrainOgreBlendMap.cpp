#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CTerrainOgreBlendMap.h>
#include <FBGraphicsOgre/Wrapper/CTerrainOgre.h>
#include <FBGraphicsOgre/Terrain/TerrainComponent/OgreTerrain.h>
#include <FBGraphicsOgre/Terrain/TerrainComponent/OgreTerrainGroup.h>
#include <FBGraphicsOgre/Terrain/TerrainComponent/OgreTerrainQuadTreeNode.h>
#include <FBGraphicsOgre/Terrain/TerrainComponent/OgreTerrainMaterialGeneratorA.h>
#include <FBGraphicsOgre/Terrain/TerrainComponent/OgreTerrainPaging.h>
#include <FBGraphicsOgre/Terrain/TerrainComponent/OgreTerrainLayerBlendMap.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>
#include <OgreSharedPtr.h>
#include <OgreShadowCameraSetup.h>

namespace fb
{
    namespace render
    {
        CTerrainOgreBlendMap::CTerrainOgreBlendMap() : m_terrain( nullptr ), m_blendMap( nullptr )
        {
            auto engine = core::IApplicationManager::instance();

            // m_stateObject = platformMgr->createStateObject();
            // m_stateObject->add();

            m_stateListener = SmartPtr<IStateListener>( new TerrainBlendMapStateListener( this ) );
            m_stateObject->addStateListener( m_stateListener );
        }

        CTerrainOgreBlendMap::~CTerrainOgreBlendMap()
        {
        }

        void CTerrainOgreBlendMap::initialise( SmartPtr<ITerrain> terrain, u32 index )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            ISharedObject::ScopedLock lock( graphicsSystem );

            m_terrain = static_cast<CTerrainOgre *>( terrain.get() );

            Ogre::TerrainGroup *terrainGroup = m_terrain->getTerrainGroup();

            Ogre::TerrainGroup::TerrainIterator ti = terrainGroup->getTerrainIterator();
            while( ti.hasMoreElements() )
            {
                Ogre::Terrain *t = ti.getNext()->instance;
                m_blendMap = t->getLayerBlendMap( index );
                break;
            }

            m_index = index;
        }

        void CTerrainOgreBlendMap::initialise( Ogre::TerrainLayerBlendMap *blendMap )
        {
            m_blendMap = blendMap;
        }

        void CTerrainOgreBlendMap::updateModifications()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            ISharedObject::ScopedLock lock( graphicsSystem );
            m_blendMap->update();
        }

        void CTerrainOgreBlendMap::loadImage( const String &fileName,
                                              const String &path /*= StringUtil::EmptyString*/ )
        {
        }

        void CTerrainOgreBlendMap::saveImage( const String &fileName,
                                              const String &path /*= StringUtil::EmptyString*/ )
        {
        }

        f32 CTerrainOgreBlendMap::getBlendValue( u32 x, u32 y )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            ISharedObject::ScopedLock lock( graphicsSystem );

            return m_blendMap->getBlendValue( x, y );
        }

        void CTerrainOgreBlendMap::setBlendValue( u32 x, u32 y, f32 val )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            ISharedObject::ScopedLock lock( graphicsSystem );

            Ogre::TerrainGroup *terrainGroup = m_terrain->getTerrainGroup();
            Ogre::TerrainGroup::TerrainIterator ti = terrainGroup->getTerrainIterator();
            while( ti.hasMoreElements() )
            {
                Ogre::Terrain *t = ti.getNext()->instance;
                m_blendMap = t->getLayerBlendMap( m_index );
                break;
            }

            m_blendMap->setBlendValue( x, y, val );
        }

        u32 CTerrainOgreBlendMap::getSize() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            ISharedObject::ScopedLock lock( graphicsSystem );

            return m_blendMap->getParent()->getLayerBlendMapSize();
        }

        SmartPtr<ITerrain> CTerrainOgreBlendMap::getOwner() const
        {
            return m_terrain;
        }

        Ogre::TerrainLayerBlendMap *CTerrainOgreBlendMap::getBlendMap() const
        {
            return m_blendMap;
        }

        void CTerrainOgreBlendMap::setBlendMap( Ogre::TerrainLayerBlendMap *val )
        {
            m_blendMap = val;
        }

        CTerrainOgreBlendMap::TerrainBlendMapStateListener::TerrainBlendMapStateListener(
            CTerrainOgreBlendMap *terrainBlendMap ) :
            m_terrainBlendMap( terrainBlendMap )
        {
        }

        CTerrainOgreBlendMap::TerrainBlendMapStateListener::~TerrainBlendMapStateListener()
        {
        }

        void CTerrainOgreBlendMap::TerrainBlendMapStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            // if(message->isExactly(StateMessageBlendMapValue::TYPE_INFO))
            //{
            //	StateMessageBlendMapValuePtr msg = message;
            //	Vector2I vec = msg->getCoordinates();
            //	m_terrainBlendMap->setBlendValue(vec.X(), vec.Y(), msg->getBlendValue());
            // }
            // else if(message->isExactly(StateMessageDirty::TYPE_INFO))
            //{
            //	m_terrainBlendMap->updateModifications();
            // }
        }

        void CTerrainOgreBlendMap::TerrainBlendMapStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
        }

        void CTerrainOgreBlendMap::TerrainBlendMapStateListener::handleQuery(
            SmartPtr<IStateQuery> &query )
        {
        }
    }  // end namespace render
}  // end namespace fb
