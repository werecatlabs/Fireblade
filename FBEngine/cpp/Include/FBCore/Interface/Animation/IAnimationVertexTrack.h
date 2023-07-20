#ifndef IAnimationVertexTrack_h__
#define IAnimationVertexTrack_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Animation/IAnimationTrack.h>

namespace fb
{

    class IAnimationVertexTrack : public IAnimationTrack
    {
    public:
        /** Type of vertex animation.
            Vertex animation comes in 2 types, morph and pose. The reason
            for the 2 types is that we have 2 different potential goals - to encapsulate
            a complete, flowing morph animation with multiple keyframes (a typical animation,
            but implemented by having snapshots of the vertex data at each keyframe),
            or to represent a single pose change, for example a facial expression.
            Whilst both could in fact be implemented using the same system, we choose
            to separate them since the requirements and limitations of each are quite
            different.
        @par
            Morph animation is a simple approach where we have a whole series of
            snapshots of vertex data which must be interpolated, e.g. a running
            animation implemented as morph targets. Because this is based on simple
            snapshots, it's quite fast to use when animating an entire mesh because
            it's a simple linear change between keyframes. However, this simplistic
            approach does not support blending between multiple morph animations.
            If you need animation blending, you are advised to use skeletal animation
            for full-mesh animation, and pose animation for animation of subsets of
            meshes or where skeletal animation doesn't fit - for example facial animation.
            For animating in a vertex shader, morph animation is quite simple and
            just requires the 2 vertex buffers (one the original position buffer)
            of absolute position data, and an interpolation factor. Each track in
            a morph animation references a unique set of vertex data.
        @par
            Pose animation is more complex. Like morph animation each track references
            a single unique set of vertex data, but unlike morph animation, each
            keyframe references 1 or more 'poses', each with an influence level.
            A pose is a series of offsets to the base vertex data, and may be sparse - ie it
            may not reference every vertex. Because they're offsets, they can be
            blended - both within a track and between animations. This set of features
            is very well suited to facial animation.
        @par
            For example, let's say you modelled a face (one set of vertex data), and
            defined a set of poses which represented the various phonetic positions
            of the face. You could then define an animation called 'SayHello', containing
            a single track which referenced the face vertex data, and which included
            a series of keyframes, each of which referenced one or more of the facial
            positions at different influence levels - the combination of which over
            time made the face form the shapes required to say the word 'hello'. Since
            the poses are only stored once, but can be referenced may times in
            many animations, this is a very powerful way to build up a speech system.
        @par
            The downside of pose animation is that it can be more difficult to set up.
            Also, since it uses more buffers (one for the base data, and one for each
            active pose), if you're animating in hardware using vertex shaders you need
            to keep an eye on how many poses you're blending at once. You define a
            maximum supported number in your vertex program definition, see the
            includes_pose_animation material script entry.
        @par
            So, by partitioning the vertex animation approaches into 2, we keep the
            simple morph technique easy to use, whilst still allowing all
            the powerful techniques to be used. Note that morph animation cannot
            be blended with other types of vertex animation (pose animation or other
            morph animation); pose animation can be blended with other pose animation
            though, and both types can be combined with skeletal animation. Also note
            that all morph animation can be expressed as pose animation, but not vice
            versa.
        */
        enum class VertexAnimationType
        {
            /// No animation
            VAT_NONE = 0,
            /// Morph animation is made up of many interpolated snapshot keyframes
            VAT_MORPH = 1,
            /// Pose animation is made up of a single delta pose keyframe
            VAT_POSE = 2
        };

        /** The target animation mode */
        enum class TargetMode
        {
            /// Interpolate vertex positions in software
            TM_SOFTWARE,
            /** Bind keyframe 1 to position, and keyframe 2 to a texture coordinate
                for interpolation in hardware */
            TM_HARDWARE
        };

        ~IAnimationVertexTrack() override = default;
    };
}  // end namespace fb

#endif  // IAnimationVertexTrack_h__
