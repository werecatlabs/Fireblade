#ifndef IMaterialNodeAnimatedTexture_h__
#define IMaterialNodeAnimatedTexture_h__

#include <FBCore/Interface/Graphics/IMaterialNode.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    namespace render
    {
        //-------------------------------------------------
        class IMaterialNodeAnimatedTexture : public IMaterialNode
        {
        public:
            /** Virtual destructor. */
            ~IMaterialNodeAnimatedTexture() override = default;

            virtual String getTextureName( u32 index ) const = 0;
            virtual void setTextureName( u32 index, const String &name ) = 0;

            virtual String getShaderName() const = 0;
            virtual void setShaderName( const String &val ) = 0;

            virtual SmartPtr<ITexture> &getTexture( u32 index ) = 0;
            virtual const SmartPtr<ITexture> &getTexture( u32 index ) const = 0;
            virtual void setTexture( u32 index, SmartPtr<ITexture> val ) = 0;
            virtual void addTexture( SmartPtr<ITexture> val ) = 0;

            virtual void setScale( const Vector3<real_Num> &scale ) = 0;

            virtual SmartPtr<IAnimator> &getAnimator() = 0;
            virtual const SmartPtr<IAnimator> &getAnimator() const = 0;
            virtual void setAnimator( SmartPtr<IAnimator> animator ) = 0;

            virtual time_interval getTime() const = 0;
            virtual void setTime( time_interval val ) = 0;

            virtual time_interval getTotalTime() const = 0;
            virtual void setTotalTime( time_interval val ) = 0;

            virtual void _getObject( void **ppObject ) = 0;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IMaterialNodeAnimatedTexture_h__
