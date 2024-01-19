// <Port> -*- C++ -*-


/**
 * \file   PortVec.hpp
 *
 * \brief File that defines the PortVec class
 */

#pragma once
#include <set>
#include <list>
#include <vector>

#include "sparta/ports/Port.hpp"

namespace sparta
{

    /**
     * \class PortVec
     * \brief A TreeNode that represents a vector of ports used by a Resource
     *
     * Ports are TreeNodes in themselves, but to prevent clutter
     * within the ResourcTreeNode, use a PortSet to collect the ports
     * under a common structure.
     */
    class PortVec : public TreeNode
    {
    public:

        //! Convenience typedef
        typedef std::vector<Port *> RegisteredPorts;

        /**
         * \brief Construct a PortVec with a given parent.  The parent can be nullptr
         * \param parent The parent of this PortVec
         * \param name Name of this PortVec
         * \param desc Description of this PortVec
         */
        PortVec(TreeNode * portset, const std::string & name, const Port::Direction& dir, const std::string & desc = "Port Vec") :
            TreeNode(nullptr, name, TreeNode::GROUP_NAME_NONE,
                     TreeNode::GROUP_IDX_NONE, desc), registered_ports_dir_(dir)
        {
            sparta_assert(name.length() != 0, "You cannot have an unnamed portVec.");
            sparta_assert(portset != nullptr,
                        "PortVecs must created with a PortSet: " << name);
            setExpectedParent_(portset);
            ensureParentIsPortSet_(portset);
            portset->addChild(this);
        }

        //! Cannot copy PortSets
        PortVec(const PortVec &) = delete;

        //! Cannot assign PortSets
        PortVec & operator=(const PortVec &) = delete;

        /**
         * \brief Get a port by the given index
         * \param index The indexed port to retrieve
         * \return Pointer to the port or nullptr if not found
         */
        Port * getPort(const uint32_t& index) const {
            try {
                auto found_port = registered_ports_[index];
                if(found_port != nullptr) {
                    return found_port;
                } else {
                    //Throw an exception if we could not find the port.
                    throw SpartaException("The port with the index : " + std::to_string(index) + " is nullptr");
                    return nullptr;
                }
            } catch(...) {
                //Throw an exception if we could not find the port.
                throw SpartaException("The port with the index : " + std::to_string(index) + " could not be found");
                return nullptr;
            }
        }

        Port * operator[](const uint32_t& index) const {
            return getPort(index);
        }
        /**
         * \brief Get the ports in this PortVec
         * \return Reference to the internal PortVec
         */
        RegisteredPorts & getPorts() {
            return registered_ports_;
        }

        Port::Direction & getDirection() {
            return registered_ports_dir_;
        };

    private:
        //! The registered ports within this PortVec
        RegisteredPorts registered_ports_;
        Port::Direction registered_ports_dir_;
        /*!
         * \brief React to a child registration
         *
         * \param child TreeNode child that must be downcastable to a
         *        sparta::Port. This is a borrowed reference - child is
         *        *not* copied. Child lifetime must exceed that of
         *        this StatisticSet instance.
         * \pre Must not be finalized
         *
         * Overrides TreeNode::onAddingChild_
         */
        virtual void onAddingChild_(TreeNode* child) override {
            if(isFinalized()){
                throw SpartaException("Cannot add a child Port once a PortVec is finalized. "
                                    "Error with: ")
                    << getLocation();
            }

            Port* port = dynamic_cast<Port*>(child);
            if(nullptr != port){
                if(port->getDirection() == getDirection()) {
                    throw SpartaException("Cannot add TreeNode child ")
                        << child->getName() << " to PortVec " << getLocation()
                        << " because the child don't have the same direction with PortVec";
                    return;
                }
                registered_ports_.push_back(port);
                return;
            }

            throw SpartaException("Cannot add TreeNode child ")
                << child->getName() << " to PortVec " << getLocation()
                << " because the child is not a Port or derivative";
        }
        void ensureParentIsPortSet_(TreeNode * parent);
    };
}
