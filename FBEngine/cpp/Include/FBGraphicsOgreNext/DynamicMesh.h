#ifndef __DynamicMesh_H_
#define __DynamicMesh_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBGraphicsOgreNext/DynamicRenderable.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Memory/SmartPtr.h>

namespace fb
{
    namespace render
    {
        
        class DynamicMesh : public DynamicRenderable
        {
        public:
            DynamicMesh( Ogre::SceneNode *owner );
            DynamicMesh( const String &name, Ogre::SceneNode *owner );
            ~DynamicMesh() override;

            void setDirty();

            void setOwner( Ogre::SceneNode *owner );
            Ogre::SceneNode *getOwner() const;

            void setMesh( SmartPtr<ISubMesh> subMesh );
            SmartPtr<ISubMesh> getSubMesh() const;

        private:
            void createVertexDeclaration() override;
            void fillHardwareBuffers() override;

            SmartPtr<IStateListener> m_stateListener;

            SmartPtr<ISubMesh> m_fbMesh;
            Ogre::SceneNode *m_owner;
        };
    } // end namespace render
}     // end namespace fb

#endif
