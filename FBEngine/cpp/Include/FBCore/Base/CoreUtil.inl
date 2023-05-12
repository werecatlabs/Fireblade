namespace fb
{
    //-------------------------------------------------
    template <class T>
    Set<T> CoreUtil::createSet( const Array<T> &data )
    {
        Set<T> set;
        for(u32 i = 0; i < data.size(); ++i)
        {
            set.insert( data[i] );
        }

        return set;
    }


    //-------------------------------------------------
    template <class T>
    bool CoreUtil::erase( Array<T> &array, const T &element )
    {
        if(array.empty())
        {
            return false;
        }

        auto hasElement = false;

        typename Array<T>::iterator it = std::find( array.begin(), array.end(), element );
        if(it != array.end())
        {
            hasElement = true;
        }

        if(hasElement)
        {
            Array<T> newArray;
            newArray.reserve( array.size() );

            for(size_t i = 0; i < array.size(); ++i)
            {
                if(element != array[i])
                {
                    newArray.push_back( array[i] );
                }
            }

            array = newArray;
        }

        return hasElement;
    }


#if FB_USE_TBB
    //-------------------------------------------------
    template <class T>
    bool CoreUtil::erase( tbb::concurrent_vector<T> &array, const T &element )
    {
        if(array.empty())
        {
            return false;
        }

        bool hasElement = false;
        typename tbb::concurrent_vector<T>::iterator it = std::find(
            array.begin(), array.end(), element );
        if(it != array.end())
        {
            hasElement = true;
        }

        if(hasElement)
        {
            tbb::concurrent_vector<T> newArray;
            for(unsigned int i = 0; i < array.size(); ++i)
            {
                if(element != array[i])
                    newArray.push_back( array[i] );
            }

            array = newArray;
        }

        return hasElement;
    }


    //-------------------------------------------------
    template <class T, class ElemType>
    bool CoreUtil::eraseElementOfType( tbb::concurrent_vector<T> &array, const ElemType &element )
    {
        if(array.empty())
        {
            return false;
        }

        auto hasElement = false;

        typename tbb::concurrent_vector<T>::iterator it = std::find(
            array.begin(), array.end(), element );
        if(it != array.end())
        {
            hasElement = true;
        }

        if(hasElement)
        {
            tbb::concurrent_vector<T> newArray;
            newArray.reserve( array.size() );

            for(size_t i = 0; i < array.size(); ++i)
            {
                if(element != array[i])
                {
                    newArray.push_back( array[i] );
                }
            }

            array = newArray;
        }

        return hasElement;
    }
#endif


    //-------------------------------------------------
    template <typename T>
    T CoreUtil::alignToNextMultiple( T offset, T alignment )
    {
        return ( ( offset + alignment - 1u ) / alignment ) * alignment;
    }


    //-------------------------------------------------
    template <class IT_TYPE, class T>
    void CoreUtil::lowest( IT_TYPE begin, IT_TYPE end, T &val )
    {
        val = T( 1e10 );
        IT_TYPE i = begin;
        for(; i != end; ++i)
        {
            val = std::min( val, *i );
        }
    }


    //-------------------------------------------------
    template <class IT_TYPE, class T>
    void CoreUtil::highest( IT_TYPE begin, IT_TYPE end, T &val )
    {
        val = T( -1e10 );
        IT_TYPE i = begin;
        for(; i != end; ++i)
        {
            val = std::max( val, *i );
        }
    }


    //-------------------------------------------------
    template <class T>
    T CoreUtil::average( const Array<T> &v )
    {
        T sum = T( 0.0 );
        typename Array<T>::const_iterator it = v.begin();
        typename Array<T>::const_iterator endIt = v.end();
        for(; it != endIt; ++it)
        {
            sum += *it;
        }

        return sum / v.size();
    }


    //-------------------------------------------------
    template <class T>
    T CoreUtil::average( const std::deque<T> &v )
    {
        T sum = T( 0.0 );
        typename std::deque<T>::const_iterator it = v.begin();
        typename std::deque<T>::const_iterator endIt = v.end();
        for(; it != endIt; ++it)
        {
            sum += *it;
        }

        return sum / static_cast<T>(v.size());
    }


    //-------------------------------------------------
    template <class T>
    bool CoreUtil::eraseElement( Array<T> &array, const T &element )
    {
        auto iter = array.begin();
        for(; iter != array.end(); ++iter)
        {
            if(element == ( *iter ))
            {
                array.erase( iter );
                return true;
            }
        }

        return false;
    }


    //-------------------------------------------------
    template <class T>
    void fill_n( T *d, size_t size, T val )
    {
        for(size_t i = 0; i < size; ++i)
        {
            d[i] = val;
        }
    }
} // end namespace fb
