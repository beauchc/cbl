#ifndef IACCESSOR_FACTORY_H
#define IACCESSOR_FACTORY_H

#include "iaccessor.h"

#include <cbl/adt/unique_ptr.h>

//==============================================================================
// CLASS IAccessorFactory
//==============================================================================

class IAccessorFactory {
public:
    template <typename T>
    static cbl::UniquePtr<IAccessor> make_accessor(T t) {
        return {cbl::make_unique<Accessor<T>>(std::move(t))};
    }
};

#endif
