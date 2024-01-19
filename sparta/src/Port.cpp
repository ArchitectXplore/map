// <Port.cpp> -*- C++ -*-


#include "sparta/ports/Port.hpp"

#include "sparta/ports/PortSet.hpp"

#include "sparta/ports/PortVec.hpp"

void sparta::Port::ensureParentIsPortSet_(TreeNode * parent)
{
    if((dynamic_cast<sparta::PortSet*>(parent) != nullptr) || 
        (dynamic_cast<sparta::PortVec*>(parent) != nullptr)) {
        return;
    }
    throw SpartaException("Port ") << getLocation()
                                 << " parent node is not a PortSet. Ports can only be added as "
                                 << "children of a sparta::PortSet ";
}
