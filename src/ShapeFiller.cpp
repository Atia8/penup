#include "ShapeFiller.h"

ShapeFiller::ShapeFiller()
{
    // Constructor: Add initialization logic if needed
}

void ShapeFiller::FillShapeAtPoint(DrawingCanvas &canvas, const wxPoint &point, const wxColour &color)
{
    auto &shapes = canvas.GetShapes(); // Assume you have a getter for `shapes` in `DrawingCanvas`

    for (auto &shape : shapes)
    {
        if (IsPointInsideShape(shape, point))
        {
            shape.color = color;
            canvas.Refresh(); // Redraw the canvas to reflect the color change
            break;
        }
    }
}

bool ShapeFiller::IsPointInsideShape(const ShapeData &shape, const wxPoint &point) const
{
    if (shape.shapeType == "Square")
    {
        wxRect rect(shape.startPoint, shape.endPoint);
        return rect.Contains(point);
    }
    if (shape.shapeType == "Circle")
    {
        wxPoint center((shape.startPoint.x + shape.endPoint.x) / 2,
                       (shape.startPoint.y + shape.endPoint.y) / 2);
        int radius = std::abs(shape.endPoint.x - shape.startPoint.x) / 2;
        return (std::pow(point.x - center.x, 2) + std::pow(point.y - center.y, 2)) <= std::pow(radius, 2);
    }
    // Add checks for other shapes like Triangle or Line if needed
    return false;
}
