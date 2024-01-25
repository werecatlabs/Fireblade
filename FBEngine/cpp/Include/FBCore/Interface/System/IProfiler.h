#ifndef IProfiler_h__
#define IProfiler_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>

/**
 * @file
 * @brief Contains macros and interfaces related to profiling.
 */

#if FB_PROFILING
/**
 * @brief Start a profile.
 * @param id The ID of the profile.
 */
#    define FB_PROFILE_START( id ) \
        if( profiler ) \
            profiler->start( id );

/**
 * @brief Start a profile with a description.
 * @param id The ID of the profile.
 * @param desc The description of the profile.
 */
#    define FB_PROFILE_START_DESC( id, desc ) \
        if( profiler ) \
            profiler->start( id, desc );

/**
 * @brief End a profile.
 * @param id The ID of the profile.
 */
#    define FB_PROFILE_END( id ) \
        if( profiler ) \
            profiler->end( id );
#else
#    define FB_PROFILE_START( id )
#    define FB_PROFILE_START_DESC( id, desc )
#    define FB_PROFILE_END( id )
#endif

namespace fb
{
    /**
     * @brief Interface for a profiling system.
     */
    class IProfiler : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IProfiler() override = default;

        /**
         * @brief Add a profile to the profiler.
         */
        virtual SmartPtr<IProfile> addProfile() = 0;

        /**
         * @brief Remove a profile from the profiler.
         * @param profile The profile to remove.
         */
        virtual void removeProfile( SmartPtr<IProfile> profile ) = 0;

        /**
         * @brief Get a profile by its ID.
         * @param id The ID of the profile.
         * @return The profile with the given ID.
         */
        virtual SmartPtr<IProfile> getProfile( hash64 id ) const = 0;

        /**
         * @brief Get all profiles.
         * @return An array of all profiles.
         */
        virtual Array<SmartPtr<IProfile>> getProfiles() const = 0;

        /**
         * @brief Log profiling results.
         */
        virtual void logResults() = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IProfiler_h__
