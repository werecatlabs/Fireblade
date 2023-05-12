#ifndef ICityBlock_h__
#define ICityBlock_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Procedural/IProceduralObject.h>
#include <FBCore/Math/Polygon2.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace procedural
    {
        class ICityBlock : public IProceduralObject
        {
        public:
            /** Virtual destructor. */
            ~ICityBlock() override = default;

            virtual void addPoint( const Vector3<real_Num> &point ) = 0;
            virtual void removePoint( const Vector3<real_Num> &point ) = 0;
            virtual Array<Vector3<real_Num>> getPoints() const = 0;

            virtual Polygon2<real_Num> getPolygon() const = 0;

            virtual bool equals( SmartPtr<ICityBlock> other ) const = 0;

            virtual Array<SmartPtr<ILot>> getLots() const = 0;
            virtual void setLots( Array<SmartPtr<ILot>> lots ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // ICityBlock_h__
