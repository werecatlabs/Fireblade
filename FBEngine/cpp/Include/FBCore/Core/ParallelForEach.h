#ifndef ParallelForEach_h__
#define ParallelForEach_h__

#include <FBCore/FBCoreTypes.h>

#if FB_USE_TBB

#    include <tbb/parallel_for_each.h>

namespace fb
{
    template <typename InputIterator, typename Function>
    void parallel_for_each( InputIterator first, InputIterator last, const Function &f )
    {
        tbb::parallel_for_each( first, last, f );
    }
}  // end namespace fb

#else

namespace fb
{

    // The class calls user function in operator()
    template <typename Function, typename Iterator>
    class parallel_for_each_body
    {
        const Function &my_func;

    public:
        parallel_for_each_body( const Function &_func ) : my_func( _func )
        {
        }

        parallel_for_each_body( const parallel_for_each_body<Function, Iterator> &_caller ) :
            my_func( _caller.my_func )
        {
        }

        void operator()( typename std::iterator_traits<Iterator>::reference value ) const
        {
            my_func( value );
        }
    };

    template <typename InputIterator, typename Function>
    void parallel_for_each( InputIterator first, InputIterator last, const Function &f )
    {
        parallel_for_each_body<Function, InputIterator> body( f );

        InputIterator it = first;
        for( ; it != last; ++it )
        {
            body( *it );
        }
    }

}  // end namespace fb

#endif

#endif  // ParallelForEach_h__
