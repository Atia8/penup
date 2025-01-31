#include "ShapeChecker.h"
#include <algorithm>
#include <cmath>

void ShapeChecker::CheckClickedShape(const wxPoint &clickedPoint, std::vector<DrawingCanvas::DrawRecord> &drawOrder, std::vector<DrawingCanvas::ShapeData> &shapes, const wxColour &color, DrawingCanvas *canvas, std::vector<DrawingCanvas::bgcolorstruct> &backcolor)
{
    // Iterate over the draw order in reverse to check the topmost shape first
    // for (auto it = drawOrder.rbegin(); it != drawOrder.rend(); ++it)
    bool shapefound = false;
    // undoStack.push_back({drawOrder, canvas->GetBackgroundColour(), false});

    for (int i = 0; i < drawOrder.size(); i++)
    {
        // const auto &record = *it;
        const auto &record = drawOrder[i];
        if (record.type == DrawingCanvas::DrawRecord::Shape)
        {

            if (record.index < 0 || record.index >= shapes.size())
            {
                wxLogMessage("Invalid index in drawOrder");
                continue; // Skip invalid entries
            }
            auto &shape = shapes[record.index];

            // Check based on shape type
            if (shape.shapeType == "Square" && IsPointInSquare(clickedPoint, shape.startPoint, shape.endPoint))
            {
                // return &shapes[record.index];

                // wxLogMessage("size:%zu", drawOrder.size());

                int oldIndex = record.index;
                DrawingCanvas::ShapeData newShape;
                // wxLogMessage("inside shapechecker");

                shapes.push_back(newShape);
                shapes.back().shapeType = "Square";
                // newShape.startPoint = shapeStartPoint;
                shapes.back().startPoint = shapes[oldIndex].startPoint;

                shapes.back().endPoint = shapes[oldIndex].endPoint;
                // newShape.endPoint.x += 100;
                // newShape.endPoint.y += 0;
                shapes.back().color = shapes[oldIndex].color;
                shapes.back().size = shapes[oldIndex].size;

                shapes.back().penTyp = shapes[oldIndex].penTyp;
                shapes.back().fillshape = true;

                shapes.back().fillcolor = color;

                // shapes.push_back(newShape);

                drawOrder.push_back({DrawingCanvas::DrawRecord::Shape, shapes.size() - 1});

                // wxLogMessage("size:%zu", drawOrder.size());

                // shape.fillcolor = color;
                // shape.fillshape = true;
                shapefound = true;

                // wxLogMessage("inside shapechecker");

                // wxLogMessage("inside square");
                break;
            }
            else if (shape.shapeType == "Circle" && IsPointInCircle(clickedPoint, shape.startPoint, shape.endPoint))
            {
                // return &shapes[record.index];

                int oldIndex = record.index;
                DrawingCanvas::ShapeData newShape;
                // wxLogMessage("inside shapechecker");

                shapes.push_back(newShape);
                shapes.back().shapeType = "Circle";
                // newShape.startPoint = shapeStartPoint;
                shapes.back().startPoint = shapes[oldIndex].startPoint;

                shapes.back().endPoint = shapes[oldIndex].endPoint;
                // newShape.endPoint.x += 100;
                // newShape.endPoint.y += 0;
                shapes.back().color = shapes[oldIndex].color;
                shapes.back().size = shapes[oldIndex].size;

                shapes.back().penTyp = shapes[oldIndex].penTyp;
                shapes.back().fillshape = true;

                shapes.back().fillcolor = color;

                // shapes.push_back(newShape);

                drawOrder.push_back({DrawingCanvas::DrawRecord::Shape, shapes.size() - 1});

                shapefound = true;
                break;
                // wxLogMessage("inside circle");
            }
            else if (shape.shapeType == "Triangle" && IsPointInTriangle(clickedPoint, shape.startPoint, shape.endPoint))
            {
                // return &shapes[record.index];
                // wxLogMessage("inside triangle");

                int oldIndex = record.index;
                DrawingCanvas::ShapeData newShape;
                // wxLogMessage("inside shapechecker");

                shapes.push_back(newShape);
                shapes.back().shapeType = "Triangle";
                // newShape.startPoint = shapeStartPoint;
                shapes.back().startPoint = shapes[oldIndex].startPoint;

                shapes.back().endPoint = shapes[oldIndex].endPoint;
                // newShape.endPoint.x += 100;
                // newShape.endPoint.y += 0;
                shapes.back().color = shapes[oldIndex].color;
                shapes.back().size = shapes[oldIndex].size;

                shapes.back().penTyp = shapes[oldIndex].penTyp;
                shapes.back().fillshape = true;

                shapes.back().fillcolor = color;

                // shapes.push_back(newShape);

                drawOrder.push_back({DrawingCanvas::DrawRecord::Shape, shapes.size() - 1});

                shapefound = true;
                break;
            }

            else if (shape.shapeType == "Ellipse" && IsPointInEllipse(clickedPoint, shape.startPoint, shape.endPoint))
            {

                // wxLogMessage("inside triangle");

                int oldIndex = record.index;
                DrawingCanvas::ShapeData newShape;
                // wxLogMessage("inside shapechecker");

                shapes.push_back(newShape);
                shapes.back().shapeType = "Ellipse";
                // newShape.startPoint = shapeStartPoint;
                shapes.back().startPoint = shapes[oldIndex].startPoint;

                shapes.back().endPoint = shapes[oldIndex].endPoint;
                // newShape.endPoint.x += 100;
                // newShape.endPoint.y += 0;
                shapes.back().color = shapes[oldIndex].color;
                shapes.back().size = shapes[oldIndex].size;

                shapes.back().penTyp = shapes[oldIndex].penTyp;
                shapes.back().fillshape = true;

                shapes.back().fillcolor = color;

                // shapes.push_back(newShape);

                drawOrder.push_back({DrawingCanvas::DrawRecord::Shape, shapes.size() - 1});

                shapefound = true;
                break;
            }

            else if (shape.shapeType == "Star" && IsPointInStar(clickedPoint, shape.startPoint, shape.endPoint))
            {
                // wxLogMessage("inside star");

                int oldIndex = record.index;
                DrawingCanvas::ShapeData newShape;
                // wxLogMessage("inside shapechecker");

                shapes.push_back(newShape);
                shapes.back().shapeType = "Star";
                // newShape.startPoint = shapeStartPoint;
                shapes.back().startPoint = shapes[oldIndex].startPoint;

                shapes.back().endPoint = shapes[oldIndex].endPoint;
                // newShape.endPoint.x += 100;
                // newShape.endPoint.y += 0;
                shapes.back().color = shapes[oldIndex].color;
                shapes.back().size = shapes[oldIndex].size;

                shapes.back().penTyp = shapes[oldIndex].penTyp;
                shapes.back().fillshape = true;

                shapes.back().fillcolor = color;

                // shapes.push_back(newShape);

                drawOrder.push_back({DrawingCanvas::DrawRecord::Shape, shapes.size() - 1});

                // shape.fillcolor = color;
                // shape.fillshape = true;
                shapefound = true;
                break;
            }
        }
    }
    if (shapefound == false)
    {

        DrawingCanvas::bgcolorstruct bg;
        bg.bgcolor = color;
        backcolor.push_back(bg);
        drawOrder.push_back({DrawingCanvas::DrawRecord::Bgcolor, backcolor.size() - 1});

        canvas->SetBackgroundColour(color);
    }
    // return nullptr; // No shape clicked
}

// Square Containment Check
bool ShapeChecker::IsPointInSquare(const wxPoint &point, const wxPoint &start, const wxPoint &end)
{
    int xMin = std::min(start.x, end.x);
    int xMax = std::max(start.x, end.x);
    int yMin = std::min(start.y, end.y);
    int yMax = std::max(start.y, end.y);
    return point.x >= xMin && point.x <= xMax && point.y >= yMin && point.y <= yMax;
}

// Circle Containment Check
bool ShapeChecker::IsPointInCircle(const wxPoint &point, const wxPoint &a, const wxPoint &b)
{

    double r = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));

    // Calculate squared distance from the point to the center
    double distSquared = (point.x - a.x) * (point.x - a.x) + (point.y - a.y) * (point.y - a.y);

    // Compare with the squared radius
    return distSquared <= r * r;
}

// Triangle Containment Check
double TriangleArea(const wxPoint &a, const wxPoint &b, const wxPoint &c)
{

    return std::abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0;
}

bool ShapeChecker::IsPointInTriangle(const wxPoint &point, const wxPoint &a, const wxPoint &b)

{
    wxPoint c(a.x, b.y);

    double area1 = TriangleArea(a, b, point);
    double area2 = TriangleArea(a, c, point);
    double area3 = TriangleArea(b, c, point);

    double totalarea = TriangleArea(a, b, c);

    return ((area1 + area2 + area3) == totalarea);
}

bool ShapeChecker::IsPointInEllipse(const wxPoint &point, const wxPoint &startPoint, const wxPoint &endPoint)
{
    // Calculate the center of the ellipse
    wxPoint center((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);

    // Calculate the horizontal and vertical radii
    double rx = std::abs(endPoint.x - startPoint.x) / 2.0; // Horizontal radius
    double ry = std::abs(endPoint.y - startPoint.y) / 2.0; // Vertical radius

    // Check if the point is inside the ellipse
    double normalizedX = (point.x - center.x) / rx;
    double normalizedY = (point.y - center.y) / ry;

    return (normalizedX * normalizedX + normalizedY * normalizedY) <= 1.0;
}

bool ShapeChecker::IsPointInStar(const wxPoint &testPoint, const wxPoint &startPoint, const wxPoint &endPoint)
{
    // Calculate the center of the bounding box
    wxPoint center((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);

    // Calculate the outer radius as half the diagonal distance
    long double dx = static_cast<long double>(std::abs(endPoint.x - startPoint.x));
    long double dy = static_cast<long double>(std::abs(endPoint.y - startPoint.y));
    double outerRadius = std::sqrt(dx * dx + dy * dy) / 2;

    // Inner radius is a fraction of the outer radius (e.g., 0.5 times)
    double innerRadius = outerRadius * 0.5;

    // Generate the star points
    std::vector<wxPoint> starPoints;
    for (int i = 0; i < 10; ++i)
    {
        long double angle = i * M_PI / 5; // 5-point star (10 vertices)
        double radius = (i % 2 == 0) ? outerRadius : innerRadius;

        starPoints.emplace_back(
            center.x + radius * std::cos(angle),
            center.y - radius * std::sin(angle));
    }

    // Ray-casting algorithm to check if the test point is inside the star
    int n = starPoints.size();
    bool inside = false;

    for (int i = 0, j = n - 1; i < n; j = i++)
    {
        // Get the endpoints of the current edge
        const wxPoint &pi = starPoints[i];
        const wxPoint &pj = starPoints[j];

        // Check if the test point's ray intersects this edge
        if (((pi.y > testPoint.y) != (pj.y > testPoint.y)) && // Ray crosses edge
            (testPoint.x < (pj.x - pi.x) * (testPoint.y - pi.y) / (pj.y - pi.y) + pi.x))
        {
            inside = !inside; // Toggle the inside state
        }
    }

    return inside;
}
