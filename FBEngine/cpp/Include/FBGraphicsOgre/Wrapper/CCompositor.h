#ifndef _CCompositor_H_
#define _CCompositor_H_

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        class CCompositor : public ISharedObject
        {
        public:
            CCompositor();
            ~CCompositor() override;

            /** */
            void setEnabled( bool isEnabled );

            /** */
            bool isEnabled() const;

        protected:
            bool m_isEnabled;
        };
    }  // end namespace render
}  // end namespace fb

#endif
