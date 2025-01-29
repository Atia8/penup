#pragma once

#include <wx/wx.h>
#include <vector>
#include "PenManager.h"
#include "Eraser.h"
#include "ShapeToolbar.h"
#include <string>
#include "ShapeDrawer.h"
using namespace std;

class Eraser;
class ShapeChecker;

class DrawingCanvas : public wxWindow
{
public:
    DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    virtual ~DrawingCanvas() noexcept {}
    void SetPenColor(const wxColour &color); // Method to change pen color
    void SetPenType(PenType type);
    void SetPenSize(int size);
    int GetPenSize();
    bool shapestatus(bool status);
    void SelectedShape(wxString shape);
    void DrawOnBuffer(std::function<void(wxDC &)> drawFunc);
    void OnResize(wxSizeEvent &event);
    // void eraserActive(bool eraserToolActive);
    //  void setdefaultcursor();

    void fillShape(bool active);

    bool IsPointInSquare(const wxPoint &point, const wxPoint &start, const wxPoint &end);
    void eraser(bool active);

private:
    bool eraserstatus = false;

    bool fillModeActive;
    bool fillshape;
    wxPoint clickedpoint;

    wxColour penColor = *wxBLACK;
    wxPen currentPen;
    int penSize;
    Eraser *eraserTool;
    bool eraserToolActive;
    wxBitmap backBuffer;

    ShapeToolbar *shapeToolbar;
    wxPoint shapeStartPoint, shapeEndPoint;
    bool shapeactive;
    string selectedShape = "";

    struct ShapeData
    {
        wxString shapeType; // Type of the shape (e.g., "Circle", "Rectangle")
        wxPoint startPoint; // Starting point of the shape
        wxPoint endPoint;   // End point of the shape
        wxColour color;     // Color of the shape
        int size;           // Size (for lines, pen width)

        wxColour fillcolor = *wxWHITE;
        wxPen penTyp;
    };

    std::vector<ShapeData> shapes;

    void OnPaint(wxPaintEvent &);
    void OnMouseDown(wxMouseEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnMouseUp(wxMouseEvent &);
    void OnMouseLeave(wxMouseEvent &);

    bool isDrawing{};

    struct Squiggle
    {
        std::vector<wxPoint2DDouble> points; // Points of the squiggle
        wxColour color;                      // Color of the squiggle
        wxPen penType;
        int Size;

        bool isEraser = false;
        // Pen type of the squiggle
    };

    std::vector<Squiggle> squiggles; // Vector of all squiggles

    // std::vector<std::vector<wxPoint2DDouble>> squiggles;
    // std::vector<std::pair<std::vector<wxPoint2DDouble>, wxColour>> squiggles; // Now a vector of pairs

    struct DrawRecord
    {
        enum Type
        {
            Squiggle,
            Shape
        } type;
        size_t index;
    };
    std::vector<DrawRecord> drawOrder;

    wxMenu contextMenu;
    void BuildContextMenu();
    void OnContextMenuEvent(wxContextMenuEvent &);

    friend class ShapeChecker;
};