#ifndef DisableScreenSave_h__
#define DisableScreenSave_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief Disable screen saver
     *
     * @note
     *  This class is used to disable screen saver.
     *  It is used in the following way:
     *  @code
     *  CDisableScreenSave disableScreenSave;
     *  // do something
     *  @endcode
     *  When the object is destroyed, the screen saver is enabled again.
     */
    class DisableScreenSave : public ISharedObject
    {
    public:
        /**
         * @brief Constructor
         */
        DisableScreenSave();

        /**
         * @brief Destructor
         */
        ~DisableScreenSave() override;

    protected:
        s32 *m_pValue = nullptr;
    };

}  // end namespace fb

#endif  // DisableScreenSave_h__
