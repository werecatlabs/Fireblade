#ifndef IAnimationTrack_h__
#define IAnimationTrack_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IAnimationTrack : public ISharedObject
    {
    public:
        /** Listener allowing you to override certain behavior of a track,
            for example to drive animation procedurally.
        */
        class Listener
        {
        public:
            virtual ~Listener()
            {
            }

            /** Get an interpolated keyframe for this track at the given time.
            @return true if the KeyFrame was populated, false if not.
            */
            virtual bool getInterpolatedKeyFrame( SmartPtr<IAnimationTrack> t,
                                                  const SmartPtr<IAnimationTimeIndex> &timeIndex,
                                                  SmartPtr<IAnimationKeyFrame> kf ) = 0;
        };

        ~IAnimationTrack() override = default;

        /** Returns the number of keyframes in this animation. */
        virtual u16 getNumKeyFrames() const = 0;

        /** Returns the KeyFrame at the specified index. */
        virtual SmartPtr<IAnimationKeyFrame> getKeyFrame( u16 index ) const = 0;

        /** Gets the 2 KeyFrame objects which are active at the time given, and the blend value between
        them.
        @remarks
            At any point in time  in an animation, there are either 1 or 2 keyframes which are 'active',
            1 if the time index is exactly on a keyframe, 2 at all other times i.e. the keyframe before
            and the keyframe after.
        @par
            This method returns those keyframes given a time index, and also returns a parametric
            value indicating the value of 't' representing where the time index falls between them.
            E.g. if it returns 0, the time index is exactly on keyFrame1, if it returns 0.5 it is
            half way between keyFrame1 and keyFrame2 etc.
        @param timeIndex The time index.
        @param keyFrame1 Pointer to a KeyFrame pointer which will receive the pointer to the
            keyframe just before or at this time index.
        @param keyFrame2 Pointer to a KeyFrame pointer which will receive the pointer to the
            keyframe just after this time index.
        @param firstKeyIndex Pointer to an unsigned short which, if supplied, will receive the
            index of the 'from' keyframe in case the caller needs it.
        @return Parametric value indicating how far along the gap between the 2 keyframes the timeIndex
            value is, e.g. 0.0 for exactly at 1, 0.25 for a quarter etc. By definition the range of this
            value is:  0.0 <= returnValue < 1.0 .
        */
        virtual f32 getKeyFramesAtTime( const SmartPtr<IAnimationTimeIndex> &timeIndex,
                                        SmartPtr<IAnimationKeyFrame> &keyFrame1,
                                        SmartPtr<IAnimationKeyFrame> &keyFrame2,
                                        u16 *firstKeyIndex = nullptr ) const = 0;

        /** Creates a new KeyFrame and adds it to this animation at the given time index.
        @remarks
            It is better to create KeyFrames in time order. Creating them out of order can result
            in expensive reordering processing. Note that a KeyFrame at time index 0.0 is always created
            for you, so you don't need to create this one, just access it using getKeyFrame(0);
        @param timePos The time from which this KeyFrame will apply.
        */
        virtual SmartPtr<IAnimationKeyFrame> createKeyFrame( f32 timePos ) = 0;

        /** Removes a KeyFrame by it's index. */
        virtual void removeKeyFrame( u16 index ) = 0;

        /** Removes all the KeyFrames from this track. */
        virtual void removeAllKeyFrames() = 0;

        /** Gets a KeyFrame object which contains the interpolated transforms at the time index
        specified.
        @remarks
            The KeyFrame objects held by this class are transformation snapshots at
            discrete points in time. Normally however, you want to interpolate between these
            keyframes to produce smooth movement, and this method allows you to do this easily.
            In animation terminology this is called 'tweening'.
        @param timeIndex The time (in relation to the whole animation sequence)
        @param kf Keyframe object to store results
        */
        virtual void getInterpolatedKeyFrame( const SmartPtr<IAnimationTimeIndex> &timeIndex,
                                              SmartPtr<IAnimationKeyFrame> &kf ) const = 0;

        /** Applies an animation track to the designated target.
        @param timeIndex The time position in the animation to apply.
        @param weight The influence to give to this track, 1.0 for full influence, less to blend with
          other animations.
        @param scale The scale to apply to translations and scalings, useful for
            adapting an animation to a different size target.
        */
        virtual void apply( const SmartPtr<IAnimationTimeIndex> &timeIndex, f32 weight = 1.0,
                            f32 scale = 1.0f ) = 0;

        /** Internal method used to tell the track that keyframe data has been
            changed, which may cause it to rebuild some internal data. */
        virtual void _keyFrameDataChanged() const = 0;

        /** Method to determine if this track has any KeyFrames which are
        doing anything useful - can be used to determine if this track
        can be optimised out.
        */
        virtual bool hasNonZeroKeyFrames() const = 0;

        /** Optimise the current track by removing any duplicate keyframes. */
        virtual void optimise() = 0;

        /** Internal method to collect keyframe times, in unique, ordered format. */
        virtual void _collectKeyFrameTimes( Array<f32> &keyFrameTimes ) = 0;

        /** Internal method to build keyframe time index map to translate global lower
            bound index to local lower bound index. */
        virtual void _buildKeyFrameIndexMap( const Array<f32> &keyFrameTimes ) = 0;

        /** Internal method to re-base the keyframes relative to a given keyframe. */
        virtual void _applyBaseKeyFrame( const SmartPtr<IAnimationKeyFrame> &base ) = 0;

        /** Set a listener for this track. */
        virtual void setListener( Listener *l ) = 0;

        /** Returns the parent Animation object for this track. */
        virtual IAnimation *getParent() const = 0;
    };

}  // end namespace fb

#endif  // IAnimationTrack_h__
