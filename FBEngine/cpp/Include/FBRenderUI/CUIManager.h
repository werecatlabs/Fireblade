#ifndef FBGUI_CGUIMANAGER_H
#define FBGUI_CGUIMANAGER_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Base/Map.h>
#include <FBCore/Base/Parameter.h>

namespace fb
{
    namespace ui
    {
        /** Implementation of a ui system the uses the render system. */
        class CUIManager : public CSharedObject<IUIManager>
        {
        public:
            CUIManager();
            ~CUIManager() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise();

            size_t messagePump( SmartPtr<ISharedObject> data ) override;

            bool OnEvent( const SmartPtr<IInputEvent> &event );

            void update() override;

            SmartPtr<IUIApplication> addApplication() override;

            void removeApplication( SmartPtr<IUIApplication> application ) override;

            /** @copydoc IUIManager::addElement */
            SmartPtr<IUIElement> addElement( hash64 type ) override;

            /** @copydoc IUIManager::addElement */
            SmartPtr<IUIElement> addElement( SmartPtr<IUIElement> parent, u8 type );

            /** @copydoc IUIManager::addElement */
            SmartPtr<IUIElement> addElement( SmartPtr<IUIElement> parent, u8 type,
                                             Properties &properties );

            /** @copydoc IUIManager::removeElement */
            void removeElement( SmartPtr<IUIElement> element ) override;

            /** @copydoc IUIManager::clear */
            void clear() override;

            SmartPtr<IUIElement> getRoot() const;

            //bool load( const String &filePath );

            void reloadCurrentLayout();

            SmartPtr<IUICursor> getCursor() const override;

            SmartPtr<IUIElement> findElement( const String &id ) const override;

            SmartPtr<IUIApplication> getApplication() const override;
            void setApplication( SmartPtr<IUIApplication> application ) override;

            Array<SmartPtr<IUIWindow>> getWindows() const;
            void setWindows( Array<SmartPtr<IUIWindow>> val );

            Array<SmartPtr<IUIRenderWindow>> getRenderWindows() const;
            void setRenderWindows( Array<SmartPtr<IUIRenderWindow>> val );

            Array<SmartPtr<IUIFileBrowser>> getFileBrowsers() const;
            void setFileBrowsers( Array<SmartPtr<IUIFileBrowser>> val );

            bool isDragging() const override;

            void setDragging( bool dragging ) override;

            virtual s32 setProperty( hash32 hash, const String &value );
            virtual s32 getProperty( hash32 hash, String &value ) const;
            virtual s32 setProperty( hash32 hash, const Parameter &param );
            virtual s32 setProperty( hash32 hash, const Parameters &params );
            virtual s32 setProperty( hash32 hash, void *param );
            virtual s32 getProperty( hash32 hash, Parameter &param ) const;
            virtual s32 getProperty( hash32 hash, Parameters &params ) const;
            virtual s32 getProperty( hash32 hash, void *param ) const;
            virtual void setObject( hash32 hash, SmartPtr<ISharedObject> object );
            virtual void getObject( hash32 hash, SmartPtr<ISharedObject> &object ) const;
            virtual s32 callFunction( u32 hash, const Parameters &params, Parameters &results );
            virtual s32 callFunction( u32 hash, SmartPtr<ISharedObject> object, Parameters &results );

            SmartPtr<IUIWindow> getMainWindow() const override;

            void setMainWindow( SmartPtr<IUIWindow> uiWindow ) override;

            void lock() override;

            void unlock() override;

        private:
            class InputListener : public CSharedObject<IEventListener>
            {
            public:
                InputListener( CUIManager *mgr );

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                CUIManager *m_mgr;
            };

            void createGUIItem( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            Array<SmartPtr<IUIElement>> getElements() const;

            void setElements( const Array<SmartPtr<IUIElement>> &elements );

            SharedPtr<Array<SmartPtr<IUIElement>>> getElementsPtr() const;
            void setElementsPtr( SharedPtr<Array<SmartPtr<IUIElement>>> ptr );

            void addElementPtr( SmartPtr<IUIElement> element );

            void setRoot( SmartPtr<IUIElement> root );

            mutable RecursiveMutex m_mutex;

            AtomicSharedPtr<Array<SmartPtr<IUIElement>>> m_elements;

            SmartPtr<IUIWindow> m_uiWindow;

            AtomicSmartPtr<IUIElement> m_root;

            SmartPtr<IUIElement> m_itemInFocus;

            SmartPtr<IEventListener> m_inputListener;

            using SoundMap = std::map<String, SmartPtr<ISound2>>;
            SoundMap m_sounds; // sounds

            using LayoutFileMap = std::map<String, String>;
            LayoutFileMap m_layoutFileMap;

            Array<SmartPtr<IUIWindow>> m_windows;

            String m_currentFileName;

            SmartPtr<IUICursor> m_cursor;

            bool m_dragging = false;
        };
    } // end namespace ui
}     // end namespace fb

#endif
