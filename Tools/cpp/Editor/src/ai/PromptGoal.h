#ifndef PromptGoal_h__
#define PromptGoal_h__

#include <FBCore/Interface/AI/IAiGoal.h>

namespace fb
{
    namespace editor
    {

        class PromptGoal : public IAiGoal
        {
        public:
            PromptGoal();

            PromptGoal( const String &label, const Array<String> &tags );

            void start() override;

            void finish() override;

            void setState( u32 state ) override;

            u32 getState() const override;

            u32 getType() const override;

            void setType( u32 type ) override;

            std::vector<std::string> m_tags;
            std::string m_label;
        };

    }  // namespace editor
}  // namespace fb

#endif  // PromptGoal_h__
