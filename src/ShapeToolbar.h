#ifndef SHAPE_TOOLBAR_H
#define SHAPE_TOOLBAR_H

#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/wrapsizer.h>
class DrawingCanvas;

class ShapeToolbar : public wxPanel
{
public:
    ShapeToolbar(wxWindow *parent, DrawingCanvas *canvas);
    // ShapeToolbar(wxWindow *parent);

    void OnShapeSelected(wxCommandEvent &event);
    void setcanvas(DrawingCanvas *canvas);
    void disableButtonHighlight();

private:
    wxString selectedShape; //
    bool shapestatus = false;
    DrawingCanvas *canvas;

    // wxButton *selectedButton;

    wxBitmapButton *selectedButton;
    wxBitmapButton *squareButton; // Declare squareButton here
    wxBitmapButton *circleButton; // Declare other buttons if needed
    wxBitmapButton *lineButton;
    wxBitmapButton *triangleButton;
    wxBitmapButton *ellipseButton;

    wxBitmapButton *starButton;
};

#endif // SHAPE_TOOLBAR_H
