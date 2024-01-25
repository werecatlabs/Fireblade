#ifndef __IProfile_h__
#define __IProfile_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{

    /**
     * @class IProfile
     * An interface for profiling code.
     */
    class IProfile : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IProfile() override = default;

        /**
         * Gets the label for the profile.
         *
         * @return The label for the profile.
         */
        virtual String getLabel() const = 0;

        /**
         * Sets the label for the profile.
         *
         * @param timeTaken The label for the profile.
         */
        virtual void setLabel( const String &timeTaken ) = 0;

        /**
         * Gets the description for the profile.
         *
         * @return The description for the profile.
         */
        virtual String getDescription() const = 0;

        /**
         * Sets the description for the profile.
         *
         * @param description The description for the profile.
         */
        virtual void setDescription( const String &description ) = 0;

        /**
         * Gets the total time taken for the profile.
         *
         * @return The total time taken for the profile.
         */
        virtual f64 getTotal() const = 0;

        /**
         * Sets the total time taken for the profile.
         *
         * @param total The total time taken for the profile.
         */
        virtual void setTotal( f64 total ) = 0;

        /**
         * Starts profiling the code.
         */
        virtual void start() = 0;

        /**
         * Ends profiling the code.
         */
        virtual void end() = 0;

        /**
         * Gets the average time taken for the profile.
         *
         * @return The average time taken for the profile.
         */
        virtual f64 getAverageTimeTaken() const = 0;

        /** Clears the profile. */
        virtual void clear() = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IProfile_h__
