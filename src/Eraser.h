#ifndef ERASER_H
#define ERASER_H

#include <wx/dc.h>
#include <wx/event.h>
#include <wx/colour.h>
// #include "drawingcanvas.h"
class DrawingCanvas;
class Eraser
{
public:
    Eraser(DrawingCanvas *canvas);
    void ToggleEraser();
    void OnMouseMove(wxMouseEvent &event);
    int CheckDefaultSize(int size);

private:
    DrawingCanvas *canvas;
    bool isActive;
};

#endif // ERASER_H
