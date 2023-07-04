#ifndef LodObject_h__
#define LodObject_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace render
    {
        
        class LodObject : public SharedObject<ISharedObject>
        {
        public:
            LodObject();
            ~LodObject();

            u32 getId() const
            {
                return m_id;
            }
            void setId( u32 val )
            {
                m_id = val;
            }

            LodPage *getLodPage() const
            {
                return m_lodPage;
            }
            void setLodPage( LodPage *val )
            {
                m_lodPage = val;
            }

            std::string getMeshName() const;
            void setMeshName( const std::string &val );

            Vector3F getPosition() const
            {
                return m_position;
            }
            void setPosition( const Vector3F &val )
            {
                m_position = val;
            }

            Vector3F getScale() const
            {
                return m_scale;
            }
            void setScale( const Vector3F &val )
            {
                m_scale = val;
            }

            QuaternionF getOrientation() const
            {
                return m_orientation;
            }
            void setOrientation( const QuaternionF &val )
            {
                m_orientation = val;
            }

        protected:
            LodPage *m_lodPage;

            Ogre::Entity *m_entity;
            Ogre::SceneNode *m_node;

            u32 m_id;

            Vector3F m_position;
            Vector3F m_scale;
            QuaternionF m_orientation;

            std::string m_meshName;

            static u32 m_idExt;
        };

    }  // namespace render
}  // end namespace fb

#endif  // LodObject_h__
