#ifndef _CDynamicLines_H_
#define _CDynamicLines_H_

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IDynamicLines.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsObjectOgre.h>

namespace fb
{
    namespace render
    {

        class CDynamicLines : public CGraphicsObjectOgre<IDynamicLines>
        {
        public:
            CDynamicLines() = default;
            CDynamicLines( SmartPtr<IGraphicsScene> creator );
            ~CDynamicLines() override;

            void initialise();

            void update() override;

            void setMaterialName( const String &materialName, s32 index = -1 ) override;
            
            SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const override;

            void _getObject( void **ppObject ) const override;

            void addPoint( const Vector3F &point ) override;
            void setPoint( u32 index, const Vector3F &point ) override;
            Vector3F getPoint( u32 index ) const override;
            u32 getNumPoints() const override;
            void clear() override;
            void setDirty() override;

            void setOperationType( u32 opType ) override;
            u32 getOperationType() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            DynamicLines *m_dynamicLines = nullptr;

            SmartPtr<IGraphicsScene> m_creator;
        };
    }  // end namespace render
}  // end namespace fb

#endif
