#ifndef FB_KeyFrame_h__
#define FB_KeyFrame_h__



namespace fb
{

   
    class KeyFrame : public ISharedObject
    {
    public:
        KeyFrame()
        {
        }

        virtual ~KeyFrame()
        {
        }

        f32 getTime() const
        {
            return m_time;
        }
        void setTime( f32 val )
        {
            m_time = val;
        }

    protected:
        f32 m_time;
    };

}  // end namespace fb

#endif  // KeyFrame_h__
