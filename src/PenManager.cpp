#include "PenManager.h"

wxPen PenManager::GetPen(PenType type)
{
    wxPen pen;
    switch (type)
    {
    case PenType::SOLID:
        pen.SetStyle(wxPENSTYLE_SOLID);
        pen.SetColour(*wxBLACK);
        pen.SetWidth(1);
        break;
    case PenType::DASHED:
        pen.SetStyle(wxPENSTYLE_LONG_DASH);
        pen.SetColour(*wxBLUE);
        pen.SetWidth(2);
        break;
    case PenType::DOTTED:
        pen.SetStyle(wxPENSTYLE_DOT);
        pen.SetColour(*wxRED);
        pen.SetWidth(1);
        break;
    case PenType::BRUSH:
        pen.SetStyle(wxPENSTYLE_CROSSDIAG_HATCH);
        pen.SetColour(*wxGREEN);
        pen.SetWidth(5); // Thicker brush
        break;

    case PenType::CUSTOM_DASH:
        pen.SetStyle(wxPENSTYLE_DOT_DASH);
        pen.SetColour(*wxCYAN);
        pen.SetWidth(3);

        break;
    case PenType::THICK_RED_LINE:
        pen.SetStyle(wxPENSTYLE_BDIAGONAL_HATCH);
        pen.SetColour(*wxRED);
        pen.SetWidth(10);
        break;
        // case PenType::STIPPLED:
        // {
        //     pen.SetStyle(wxPENSTYLE_TRANSPARENT);
        //     pen.SetColour(*wxRED);
        //     pen.SetWidth(10);
        //     break;
        // }

    default:
        pen.SetStyle(wxPENSTYLE_DOT);
        pen.SetColour(*wxGREEN);
        pen.SetWidth(5); // Default brush style
        break;
    }
    return pen;
}
