#include <CZ/Core/CZObject.h>
#include <CZ/Core/CZWeak.h>

using namespace CZ;

std::vector<void *> &CZWeakUtils::objectRefs(const CZObject *object) noexcept
{
    return object->m_weakRefs;
}

bool CZWeakUtils::isObjectDestroyed(const CZObject *object) noexcept
{
    return object->m_destroyed;
}
