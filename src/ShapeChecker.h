#ifndef SHAPE_CHECKER_H
#define SHAPE_CHECKER_H

#include <wx/wx.h>
#include <vector>
#include <string>
#include "drawingcanvas.h"

// struct DrawRecord
// {
//     enum Type
//     {
//         Shape,
//         Squiggle
//     };
//     Type type;
//     int index;
// };

// struct Shape
// {
//     std::string shapeType;
//     wxPoint startPoint;
//     wxPoint endPoint;
//     wxColour color;
//     int size;
// };

class ShapeChecker
{
public:
    static void CheckClickedShape(const wxPoint &clickedPoint, std::vector<DrawingCanvas::DrawRecord> &drawOrder, std::vector<DrawingCanvas::ShapeData> &shapes, const wxColour &color, DrawingCanvas *canvas);

private:
    // Shape checking functions for different types
    static bool IsPointInSquare(const wxPoint &point, const wxPoint &start, const wxPoint &end);
    static bool IsPointInCircle(const wxPoint &point, const wxPoint &start, const wxPoint &end);

    static bool IsPointInTriangle(const wxPoint &point, const wxPoint &a, const wxPoint &b);

    static bool IsPointInEllipse(const wxPoint &point, const wxPoint &startPoint, const wxPoint &endPoint);

    // static bool IsPointInStar(const wxPoint &point, const wxPoint &start, const wxPoint &end, int numPoints = 5);
    // static bool IsPointInAnyTriangle(const wxPoint &point, const wxPoint &a, const wxPoint &b, const wxPoint &c);

    static bool IsPointInStar(const wxPoint &testPoint, const wxPoint &startPoint, const wxPoint &endPoint);
};

#endif // SHAPE_CHECKER_H
