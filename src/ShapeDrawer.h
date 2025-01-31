#ifndef SHAPE_DRAWER_H
#define SHAPE_DRAWER_H

#include <wx/wx.h>
#include <wx/dc.h>
#include <wx/colour.h>
using namespace std;
class ShapeDrawer
{
public:
    ShapeDrawer(wxDC &dc, const wxColour &penColor, int penSize, const wxColour &fillcolor, const wxPen &penType, bool fillshape);
    void DrawSquare(const wxPoint &startPoint, const wxPoint &endPoint, const wxColour &fillcolor);
    void DrawCircle(const wxPoint &startPoint, const wxPoint &endPoint);
    void DrawLine(const wxPoint &startPoint, const wxPoint &endPoint);
    void DrawTriangle(const wxPoint &startPoint, const wxPoint &endPoint);
    // void PrepareBrush(const wxColour &fillcolor);

    void DrawEllipse(const wxPoint &startPoint, const wxPoint &endPoint);
    void DrawStar(const wxPoint &startPoint, const wxPoint &endPoint);

private:
    wxDC &dc;
    wxPen pen;
};

#endif // SHAPE_DRAWER_H
