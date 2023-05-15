#ifndef FBLuabindPrerequisites_h__
#define FBLuabindPrerequisites_h__

//#include <luabind/lua_state_fwd.hpp>

struct lua_State;

namespace boost
{
    template <class T>
    const T *get_pointer( const std::shared_ptr<T> &ptr )
    {
        return ptr.get();
    }

    template <class T>
    T *get_pointer( std::shared_ptr<T> &ptr )
    {
        return ptr.get();
    }
}  // namespace boost

#endif  // FBLuabindPrerequisites_h__
