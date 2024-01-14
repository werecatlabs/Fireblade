#ifndef CreateRigidBodies_h__
#define CreateRigidBodies_h__

#include <FBCore/System/Job.h>

namespace fb
{
    /** Create rigid bodies. */
    class CreateRigidBodies : public Job
    {
    public:
        /** Constructor */
        CreateRigidBodies() = default;

        /** Destructor */
        ~CreateRigidBodies() override = default;

        /** @copydoc Job::execute */
        void execute() override;

        /** Gets the actor.
        @return The actor.
        */
        SmartPtr<scene::IActor> getActor() const;

        /** Sets the actor.
        @param actor The actor.
        */
        void setActor( SmartPtr<scene::IActor> actor );

        /** Gets the cascade flag.
        @return True if the rigid bodies should be created for all children of the actor.
        */
        bool getCascade() const;

        /** Sets the cascade flag.
        @param cascade True if the rigid bodies should be created for all children of the actor.
        */
        void setCascade( bool cascade );

        bool getMakeStatic() const;

        void setMakeStatic( bool makeStatic );

        bool isConvex() const;

        void setConvex( bool convex );

        FB_CLASS_REGISTER_DECL;

    protected:
        /** Creates the rigid bodies. */
        void createRigidBodies( SmartPtr<scene::IActor> actor, bool cascade );

        // The actor.
        SmartPtr<scene::IActor> m_actor;

        // True if the rigid bodies should be created for all children of the actor.
        bool m_cascade = false;

        bool m_makeStatic = false;

        bool m_isConvex = false;
    };
}  // namespace fb

#endif  // CreateRigidBodies_h__
