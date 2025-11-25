#ifndef CZ_CZCOLORSCHEMEEVENT_H
#define CZ_CZCOLORSCHEMEEVENT_H

#include <CZ/Core/Events/CZEvent.h>
#include <CZ/Core/CZColorScheme.h>

class CZ::CZColorSchemeEvent : public CZEvent
{
public:
    CZ_EVENT_DECLARE_COPY
    CZColorSchemeEvent(CZColorScheme scheme) noexcept : CZEvent(Type::ColorScheme), scheme(scheme) {};
    CZColorScheme scheme;
};


#endif // CZ_CZCOLORSCHEMEEVENT_H
