//
// Created by Zane Desir on 10/11/2021.
//

#ifndef FB_IFRAMESTATISTICS_H
#define FB_IFRAMESTATISTICS_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    /** An interface class to show frame statistics.
     */
    class IFrameStatistics : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IFrameStatistics() override = default;

        /** Tells this object whether to be visible or not, if it has a renderable component. */
        virtual void setVisible( bool visible ) = 0;

        /** Return a boolean indicating whether or not this object is visible. */
        virtual bool isVisible() const = 0;
    };
}  // end namespace fb

#endif  // FB_IFRAMESTATISTICS_H
