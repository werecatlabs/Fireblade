#ifndef Profile_h__
#define Profile_h__


#include <FBCore/Thread/Threading.h>
#include <FBCore/Interface/System/IProfile.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /**
     * @brief Represents a profiling data for a piece of code execution.
     *
     * This class represents profiling data for a piece of code execution, including start and end times,
     * total time taken, and average time taken over multiple executions.
     */
    class Profile : public IProfile
    {
    public:
        /** The default number of samples for calculating the average time taken. */
        static const s32 DEFAULT_NUM_SAMPLES;

        /** Constructor. */
        Profile();

        /** Destructor. */
        ~Profile() override;

        /** Starts the profiling timer. */
        void start() override;

        /** Ends the profiling timer. */
        void end() override;

        /**
         * @brief Gets the description of the profile.
         *
         * @return The description of the profile.
         */
        String getDescription() const override;

        /**
         * @brief Sets the description of the profile.
         *
         * @param description The description to set.
         */
        void setDescription( const String &description ) override;

        /** Gets the time taken for the profile. */
        float getTimeTaken() const;

        /**
         * @brief Sets the time taken for the profile.
         *
         * @param timeTaken The time taken to set.
         */
        void setTimeTaken( float timeTaken );

        /** Gets the user data associated with the profile. */
        void *getUserData() const override;

        /**
         * @brief Sets the user data associated with the profile.
         *
         * @param userData The user data to set.
         */
        void setUserData( void *userData ) override;

        /**
         * @brief Gets the label of the profile.
         *
         * @return The label of the profile.
         */
        String getLabel() const override;

        /**
         * @brief Sets the label of the profile.
         *
         * @param label The label to set.
         */
        void setLabel( const String &label ) override;

        /** Gets the average time taken for the profile. */
        f64 getAverageTimeTaken() const override;

        /** Clears the profiling data. */
        void clear();

        /** Gets the total time taken for the profile. */
        f64 getTotal() const override;

        /**
         * @brief Sets the total time taken for the profile.
         *
         * @param total The total time taken to set.
         */
        void setTotal( f64 total ) override;

    protected:
        Array<f32> m_averageTimeTaken;  ///< The array of average time taken over multiple executions.

        u64 m_start = 0;  ///< The start time of the profiling.
        u64 m_end = 0;    ///< The end time of the profiling.

        time_interval m_total = time_interval( 0.0 );  ///< The total time taken for the profiling.

        f32 m_timeTaken = 0.0f;  ///< The time taken for the profiling.

        f32 m_nextUpdate = 0.0f;  ///< The time of the next update.

        void *m_userData = nullptr;  ///< The user data associated with the profile.

        String m_description;  ///< The description of the profile.

        String m_label;  ///< The label of the profile.

        mutable RecursiveMutex m_mutex;  ///< The mutex used to protect the profile data.

        static u32 m_idExt;  ///< The ID of the profile.
    };

}  // end namespace fb

#endif  // Profile_h__
