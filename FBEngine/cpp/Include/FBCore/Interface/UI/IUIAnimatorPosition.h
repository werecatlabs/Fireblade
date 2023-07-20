#ifndef _IGUIAnimatorPosition_H
#define _IGUIAnimatorPosition_H

#include "FBCore/Interface/UI/IUIAnimator.h"

namespace fb
{
    namespace ui
    {

        /** Animates a gui item.
         */
        class IUIAnimatorPosition : public IUIAnimator
        {
        public:
            /** Virtual destructor. */
            ~IUIAnimatorPosition() override = default;

            /** */
            virtual void setStart( const Vector2F &position ) = 0;

            /** */
            virtual Vector2F getStart() const = 0;

            /** */
            virtual void setEnd( const Vector2F &position ) = 0;

            /** */
            virtual Vector2F getEnd() const = 0;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
