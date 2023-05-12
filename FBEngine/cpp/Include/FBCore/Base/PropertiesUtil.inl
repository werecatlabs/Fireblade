namespace fb
{
    template <class X>
    void PropertiesUtil::bind( Properties *properties, bool from, Array<X> *ANI, const char *tagname )
    {
        if(from)
        {
            for(auto &ani : *ANI)
            {
                //TXB_attr_bind(properties, from, &ani, tagname); 
                //ANI->push_back(ani); 
                TXB_binding( &ani, properties, from );
            }
        }
        else
        {
            for(size_t i = 0; i < ANI->size(); ++i)
            {
                auto ani = &ANI->at( i );
                TXB_attr_bind( properties, from, ani, tagname );
            }
        }
    }
}
