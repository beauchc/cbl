#ifndef IACCESSOR_FACTORY_H
#define IACCESSOR_FACTORY_H

#include "iaccessor.h"
#include "unique_ptr.h"

//==============================================================================
// CLASS IAccessorFactory
//==============================================================================

class IAccessorFactory {
public:
    template <typename T>
    static UniquePtr<IAccessor> make_accessor(T t) {
        return {make_unique<Accessor<T>>(std::move(t))};
    }
};

#endif
