#ifndef IParticleComponent_h__
#define IParticleComponent_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    namespace render
    {
        /**
         * @brief The base class for all nodes in a particle system.
         */
        class IParticleNode : public ISharedObject
        {
        public:
            /** */
            ~IParticleNode() override = default;

            /**
             * @brief Adds a child node.
             */
            virtual void addChild( SmartPtr<IParticleNode> child ) = 0;

            /**
             * @brief Adds a child node at the given index.
             */
            virtual void addChild( SmartPtr<IParticleNode> child, s32 index ) = 0;

            /**
             * @brief Removes a child node.
             */
            virtual void removeChild( SmartPtr<IParticleNode> child ) = 0;

            /**
             * @brief Removes this node.
             */
            virtual void remove() = 0;

            /**
             * @brief Returns the number of child nodes.
             */
            virtual u32 getNumChildren() const = 0;

            /**
             * @brief Returns the child node at the given index.
             */
            virtual SmartPtr<IParticleNode> getChildByIndex( u32 index ) const = 0;

            /**
             * @brief Returns the child node with the given ID.
             */
            virtual SmartPtr<IParticleNode> getChildById( hash32 id ) const = 0;

            /**
             * @brief Returns an array of all child nodes.
             */
            virtual Array<SmartPtr<IParticleNode>> getChildren() const = 0;

            /**
             * @brief Returns the parent node.
             */
            virtual SmartPtr<IParticleNode> getParent() const = 0;

            /**
             * @brief Sets the parent node.
             */
            virtual void setParent( SmartPtr<IParticleNode> parent ) = 0;

            /**
             * @brief Sets the position of the node.
             */
            virtual void setPosition( const Vector3F &position ) = 0;

            /**
             * @brief Returns the position of the node.
             */
            virtual Vector3F getPosition() const = 0;

            /**
             * @brief Returns the absolute position of the node.
             */
            virtual Vector3F getAbsolutePosition() const = 0;

            /**
             * @brief Returns the particle system this node belongs to.
             */
            virtual SmartPtr<IParticleSystem> getParticleSystem() const = 0;

            /**
             * @brief Sets the particle system this node belongs to.
             */
            virtual void setParticleSystem( SmartPtr<IParticleSystem> particleSystem ) = 0;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IParticleComponent_h__
