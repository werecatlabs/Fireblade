#ifndef _CDynamicLines_H_
#define _CDynamicLines_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IDynamicLines.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsObjectOgreNext.h>

namespace fb
{
    namespace render
    {
        
        class CDynamicLines : public CGraphicsObjectOgreNext<IDynamicLines>
        {
        public:
            CDynamicLines( SmartPtr<IGraphicsScene> creator );
            ~CDynamicLines() override;

            void initialise();

            void update() override;

            //
            // IGraphicsObject functions
            //
            void setName( const String &name );
            String getName() const;
            u32 getId() const;

            void setRenderQueueGroup( u8 renderQueue );

            SmartPtr<IGraphicsObject>
            clone( const String &name = StringUtil::EmptyString ) const override;

            void _getObject( void **ppObject ) const override;

            //
            // IDynamicLines functions
            //
            void addPoint( const Vector3F &point ) override;
            void setPoint( u32 index, const Vector3F &point ) override;
            Vector3F getPoint( u32 index ) const override;
            u32 getNumPoints() const override;
            void clear() override;
            void setDirty() override;

            void setOperationType( u32 opType ) override;
            u32 getOperationType() const override;

            Ogre::ObjectMemoryManager *getMemoryManager() const;
            void setMemoryManager( Ogre::ObjectMemoryManager *val );

        protected:
            Ogre::ObjectMemoryManager *m_memoryManager = nullptr;
            DynamicLines *m_dynamicLines = nullptr;

            SmartPtr<IGraphicsScene> m_creator;
            String m_name;
        };
    } // end namespace render
}     // end namespace fb

#endif
