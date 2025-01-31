#include "ShapeDrawer.h"
#include <wx/dc.h>
#include <wx/pen.h>
#include <cmath> // For circle radius calculation
#include "PenManager.h"

ShapeDrawer::ShapeDrawer(wxDC &dc, const wxColour &penColor, int penSize, const wxColour &fillcolor, const wxPen &penType, bool fillshape)
    : dc(dc), pen(penType)
// pen(penColor, penSize)
{

    // pen(penType);
    pen.SetColour(penColor);
    pen.SetWidth(penSize);
    wxPenStyle style;
    if (!penType.IsOk())
    {
        // If the pen is invalid, initialize it with a default valid pen
        style = wxPENSTYLE_SOLID;
    }
    else
    {
        style = penType.GetStyle();
    }
    pen.SetStyle(style);
    dc.SetPen(pen);

    if (fillshape == true)
    {
        dc.SetBrush(wxBrush(fillcolor));
    }
    else
    {
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
    }

    // if (fillcolor == *wxWHITE)             // Set the pen color and size
    //     dc.SetBrush(*wxTRANSPARENT_BRUSH); // Set the brush to transparent for no fill
    // else                                   // dc.SetBrush(pen.GetColour());
    //     dc.SetBrush(wxBrush(fillcolor));   // Set the brush to the fill color
}

void ShapeDrawer::DrawSquare(const wxPoint &startPoint, const wxPoint &endPoint, const wxColour &fillcolor)
{
    wxCoord width = abs(endPoint.x - startPoint.x);
    wxCoord height = abs(endPoint.y - startPoint.y);
    wxCoord size = std::min(width, height);
    dc.DrawRectangle(startPoint.x, startPoint.y, size, size);
}

void ShapeDrawer::DrawCircle(const wxPoint &startPoint, const wxPoint &endPoint)
{
    wxCoord radius = std::hypot(endPoint.x - startPoint.x, endPoint.y - startPoint.y);
    dc.DrawCircle(startPoint, radius);
}

void ShapeDrawer::DrawLine(const wxPoint &startPoint, const wxPoint &endPoint)
{
    dc.DrawLine(startPoint, endPoint);
}

void ShapeDrawer::DrawTriangle(const wxPoint &startPoint, const wxPoint &endPoint)
{
    wxPoint points[3] = {
        wxPoint(startPoint.x, startPoint.y),
        wxPoint(endPoint.x, endPoint.y),
        wxPoint(startPoint.x, endPoint.y)};
    dc.DrawPolygon(3, points);
}

// void ShapeDrawer::PrepareBrush(const wxColour &fillcolor)
// {
//     if (fillcolor == *wxWHITE)
//         dc.SetBrush(*wxTRANSPARENT_BRUSH);
//     else
//         dc.SetBrush(wxBrush(fillcolor));
// }

void ShapeDrawer::DrawEllipse(const wxPoint &startPoint, const wxPoint &endPoint)
{
    int left = std::min(startPoint.x, endPoint.x);
    int top = std::min(startPoint.y, endPoint.y);
    int width = std::abs(startPoint.x - endPoint.x);
    int height = std::abs(startPoint.y - endPoint.y);

    // dc.SetBrush(wxBrush(fillcolor));
    // dc.SetPen(wxPen(color, size));
    dc.DrawEllipse(left, top, width, height);
}

void ShapeDrawer::DrawStar(const wxPoint &startPoint, const wxPoint &endPoint)
{
    // Calculate the center of the bounding box
    wxPoint center((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);

    // Calculate the outer radius as half the diagonal distance
    long double dx = static_cast<long double>(std::abs(endPoint.x - startPoint.x));
    long double dy = static_cast<long double>(std::abs(endPoint.y - startPoint.y));
    //  double squaredDistance = dx * dx + dy * dy;

    // double outerRadius =std::sqrt(squaredDistance) / 2;

    double outerRadius = sqrt(static_cast<long double>(dx * dx + dy * dy)) / 2;

    // Inner radius is a fraction of the outer radius (e.g., 0.5 times)
    double innerRadius = outerRadius * 0.5;

    // Calculate points for the star
    std::vector<wxPoint> points;
    for (int i = 0; i < 10; ++i)
    {
        long double angle = i * M_PI / 5; // 5-point star (10 vertices)
        double radius = (i % 2 == 0) ? outerRadius : innerRadius;

        points.emplace_back(
            center.x + radius * std::cos(angle),
            center.y - radius * std::sin(angle));
    }

    // Draw the star

    dc.DrawPolygon(points.size(), points.data());
}
