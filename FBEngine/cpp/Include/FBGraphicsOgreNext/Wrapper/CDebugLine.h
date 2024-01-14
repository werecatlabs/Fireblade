#ifndef CDebugLine_h__
#define CDebugLine_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IDebugLine.h>

namespace fb
{
    namespace render
    {

        class CDebugLine : public IDebugLine
        {
        public:
            CDebugLine() = default;
            CDebugLine( const CDebugLine &other ) = delete;
            ~CDebugLine() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::update */
            void update() override;

            /** @copydoc IDebugLine::getVector */
            Vector3F getVector() const override;

            /** @copydoc IDebugLine::setVector */
            void setVector( const Vector3F &vector ) override;

            /** @copydoc IDebugLine::getPosition */
            Vector3F getPosition() const override;

            /** @copydoc IDebugLine::setPosition */
            void setPosition( const Vector3F &position ) override;

            /** @copydoc IDebugLine::getLifeTime */
            f64 getLifeTime() const override;

            /** @copydoc IDebugLine::setLifeTime */
            void setLifeTime( f64 lifeTime ) override;

            /** @copydoc IDebugLine::getMaxLifeTime */
            f64 getMaxLifeTime() const override;

            /** @copydoc IDebugLine::setMaxLifeTime */
            void setMaxLifeTime( f64 maxLifeTime ) override;

            /** @copydoc IDebugLine::isVisible */
            bool isVisible() const override;

            /** @copydoc IDebugLine::setVisible */
            void setVisible( bool visible ) override;

            /** @copydoc IDebugLine::getId */
            hash32 getId() const override;

            /** @copydoc IDebugLine::setId */
            void setId( hash32 id ) override;

            Ogre::ObjectMemoryManager *getMemoryManager() const;
            void setMemoryManager( Ogre::ObjectMemoryManager *memoryManager );

            Ogre::HlmsUnlitDatablock *getDatablock() const;
            void setDatablock( Ogre::HlmsUnlitDatablock *datablock );

            String getMaterialName() const;

            void setMaterialName( const String &materialName );

            u32 getColour() const;

            void setColour( u32 colour );

            bool isDirty() const;

            void setDirty( bool dirty );

            FB_CLASS_REGISTER_DECL;

        protected:
            hash32 m_id = 0;

            Ogre::ManualObject *m_manualObject = nullptr;

            Ogre::HlmsUnlitDatablock *m_datablock = nullptr;

            Ogre::ObjectMemoryManager *m_memoryManager = nullptr;

            Vector3F m_vector = Vector3F::unitY();
            Vector3F m_position = Vector3F::zero();

            RawPtr<Ogre::SceneNode> m_sceneNode;

            f64 m_lifeTime = 0.0;
            f64 m_maxLifeTime = 1.0;

            atomic_u32 m_colour = 1;

            atomic_bool m_isDirty = true;
            atomic_bool m_isVisible = false;

            String m_materialName;

            static u32 m_nameExt;
        };
    }  // namespace render
}  // end namespace fb

#endif  // CDebugLine_h__
