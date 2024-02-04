#ifndef __Game_h__
#define __Game_h__

#include <FBCore/Application.h>

namespace fb
{
    
    class Game : public core::Application
    {
    public:
        Game();
        ~Game() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

    protected:
        void createPlugins() override;
    };
} // end namespace fb

#endif  // __Game_h__
