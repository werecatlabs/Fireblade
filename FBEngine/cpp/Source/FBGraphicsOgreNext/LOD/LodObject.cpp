#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/LOD/LodObject.h"

namespace fb
{
    namespace render
    {

        u32 LodObject::m_idExt = 0;

        LodObject::LodObject() : m_lodPage( nullptr ), m_entity( nullptr ), m_node( nullptr )
        {
            m_id = m_idExt++;
        }

        LodObject::~LodObject()
        {
        }

        std::string LodObject::getMeshName() const
        {
            return m_meshName;
        }

        void LodObject::setMeshName( const std::string &val )
        {
            m_meshName = val;
        }

    }  // namespace render
}  // namespace fb
