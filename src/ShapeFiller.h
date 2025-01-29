#ifndef SHAPEFILLER_H
#define SHAPEFILLER_H

#include <wx/colour.h>
#include "drawingcanvas.h"

class ShapeFiller
{
public:
    ShapeFiller();
    void FillShapeAtPoint(DrawingCanvas &canvas, const wxPoint &point, const wxColour &color);

private:
    bool IsPointInsideShape(const ShapeData &shape, const wxPoint &point) const;
};

#endif // SHAPEFILLER_H
