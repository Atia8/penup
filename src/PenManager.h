#ifndef PEN_MANAGER_H
#define PEN_MANAGER_H

#include <wx/pen.h>

enum class PenType
{
    SOLID,
    DASHED,
    DOTTED,
    BRUSH,

    CUSTOM_DASH,    // New
    THICK_RED_LINE, // New
    STIPPLED        // New
};

class PenManager
{
public:
    static wxPen GetPen(PenType type);
};

#endif // PEN_MANAGER_H
