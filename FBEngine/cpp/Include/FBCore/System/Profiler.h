#ifndef Profiler_h__
#define Profiler_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Interface/System/IProfiler.h>
#include <FBCore/Thread/Threading.h>
#include <FBCore/Base/Array.h>

#if ENABLE_PROFILER
#    define FB_PROFILE_START( profile ) profile->start();
#    define FB_PROFILE_END( profile ) profile->end();
#else
#    define FB_PROFILE_START( profile )
#    define FB_PROFILE_END( profile )
#endif

namespace fb
{

    /**
     * @brief Manages profiling of code execution.
     *
     * This class provides profiling capabilities for measuring the performance of code execution.
     * It allows adding and removing profiles, getting individual profiles and all profiles, and logging results.
     */
    class Profiler : public CSharedObject<IProfiler>
    {
    public:
        /**
         * @brief Constructor.
         *
         * Creates a new instance of the profiler.
         */
        Profiler();

        /**
         * @brief Destructor.
         *
         * Destroys the profiler and cleans up any resources associated with it.
         */
        ~Profiler() override;

        /**
         * @brief Updates the profiler.
         *
         * This method updates the profiler by checking if it is time to log results.
         */
        void update() override;

        /**
         * @brief Adds a profile to the profiler.
         *
         * @return The added profile.
         */
        SmartPtr<IProfile> addProfile() override;

        /**
         * @brief Removes a profile from the profiler.
         *
         * @param profile The profile to remove.
         */
        void removeProfile( SmartPtr<IProfile> profile ) override;

        /**
         * @brief Gets a profile by ID.
         *
         * @param id The ID of the profile to get.
         * @return The found profile, or null if not found.
         */
        SmartPtr<IProfile> getProfile( hash64 id ) const override;

        /**
         * @brief Gets all profiles in the profiler.
         *
         * @return An array of all profiles.
         */
        Array<SmartPtr<IProfile>> getProfiles() const override;

        /**
         * @brief Logs the profiling results.
         *
         * This method logs the profiling results to an output stream.
         */
        void logResults() override;

    protected:
        time_interval m_nextUpdate = 0.0;      ///< The time of the next update.
        Array<SmartPtr<IProfile>> m_profiles;  ///< The array of profiles managed by the profiler.
        time_interval m_nextUpdateTime = 0.0;  ///< The time of the next results log update.

        FB_MUTEX_MUTABLE( Mutex );  ///< The mutex used to protect the profiler data.
    };

}  // end namespace fb

#endif  // Profiler_h__
