#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/MathBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    lua_Integer _rangedRandom( lua_Integer min_, lua_Integer max_ )
    {
        return Math<lua_Integer>::RangedRandom( min_, max_ );
    }

    lua_Number __rangedRandomNumber( lua_Number min_, lua_Number max_ )
    {
        return Math<lua_Number>::RangedRandom( min_, max_ );
    }

    template <class T>
    void register_sphere3( lua_State *L, const char *name )
    {
        using namespace luabind;

        module( L )[class_<Sphere3<T> >( name )
                        .def( constructor<>() )
                        .def( constructor<const Vector3<T> &, T>() )
                        .def( constructor<const Sphere3<T> &>() )
                        .def( "operator=", &Sphere3<T>::operator= )
                        .def( "getCenter", &Sphere3<T>::getCenter )
                        .def( "setCenter", &Sphere3<T>::setCenter )
                        .def( "getRadius", &Sphere3<T>::getRadius )
                        .def( "setRadius", &Sphere3<T>::setRadius )
                    //.def( "intersects", &Sphere3<T>::intersects )
                    //.def( "intersectsSQ", &Sphere3<T>::intersectsSQ )
                    //.def( "intersects", &Sphere3<T>::intersects )
        ];
    }

    void bindMath( lua_State *L )
    {
        using namespace luabind;

        typedef Math<s32> math_int;
        typedef Math<f64> math_num;

        module( L )[class_<math_int>( "MathI" )
                        .scope[def( "Round", &math_int::Round ), def( "Min", &math_int::min ),
                               def( "Max", &math_int::max ), def( "Sin", &math_int::Sin ),
                               def( "Cos", &math_int::Cos ),
                               //def("Tan", &math_int::Tan ),
                               def( "Atan", &math_int::Atan ), def( "Atan2", &math_int::ATan2 ),
                               def( "RangedRandom", &math_int::RangedRandom ),
                               def( "RangedRandom", _rangedRandom )]];

        module( L )[class_<math_num>( "Math" )
                        .scope[def( "Round", &math_num::Round ), def( "Min", &math_num::min ),
                               def( "Max", &math_num::max ), def( "Sin", &math_num::Sin ),
                               def( "Cos", &math_num::Cos ),
                               //def("Tan", &math_num::Tan ),
                               def( "Atan", &math_num::Atan ), def( "Atan2", &math_num::ATan2 ),
                               def( "Abs", &math_num::Abs ), def( "wrap", &math_num::wrap ),
                               def( "RadToDeg", &math_num::RadToDeg ),
                               def( "RangedRandom", &math_num::RangedRandom ),
                               def( "RangedRandom", __rangedRandomNumber )]];

        register_sphere3<int>( L, "Sphere3i" );
        register_sphere3<float>( L, "Sphere3f" );
        //register_sphere3<double>( L, "Sphere3d" );
    }

}  // end namespace fb
