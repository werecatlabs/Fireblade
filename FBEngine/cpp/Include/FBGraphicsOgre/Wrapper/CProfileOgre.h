#ifndef CProfileOgre_h__
#define CProfileOgre_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/System/IProfile.h>


namespace fb
{
    namespace render
    {

        class CProfileOgre : public IProfile
        {
        public:
            CProfileOgre();
            ~CProfileOgre();

            void start();
            void end();

            String getDescription() const;

            void setDescription( const String &description );

            time_interval getTimeTaken() const;

            void setTimeTaken( time_interval timeTaken );

            hash32 getId() const;
            void setId( hash32 id );

            String getLabel() const;
            void setLabel( const String &label );

        protected:
            time_interval m_start;
            time_interval m_end;
            time_interval m_timeTaken;
            time_interval m_nextUpdate;
            String m_description;
            String m_label;
        };

    }  // namespace render
}  // namespace fb

#endif  // CProfileOgre_h__
