#pragma once

#include <wx/wx.h>
#include <vector>
#include "PenManager.h"
#include "Eraser.h"
#include "ShapeToolbar.h"
#include <string>
#include "ShapeDrawer.h"
#include <queue>
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

    // void eraserActive(bool eraserToolActive);
    //  void setdefaultcursor();

    void fillShape(bool active);

    void eraser(bool active);
    void Undo();
    void Redo();

    void enablesquiggle(bool active);

private:
    // wxColour newcolor = *wxBlack;
    // wxColour newcolor = wxColour(*wxBLACK);

    bool undo = false, squiggle = false;

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
        // wxColour fillcolor;

        wxPen penTyp;

        bool fillshape = false;
    };
    // for eraser to work inside of shape or erase fillcolor usually make shape={} not shapes
    std::vector<ShapeData> shapes = {}, shapeCopy;

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

    std::vector<Squiggle> squiggles, squiggleCopy; // Vector of all squiggles

    // std::vector<std::vector<wxPoint2DDouble>> squiggles;
    // std::vector<std::pair<std::vector<wxPoint2DDouble>, wxColour>> squiggles; // Now a vector of pairs

    struct bgcolorstruct
    {
        wxColour bgcolor;
    };

    std::vector<bgcolorstruct> backcolor = {}, redocolor = {};

    // backcolor.push_back(bgcolorstruct{*wxBLACK});
    // backcolor.push_back(*wxBlack);
    struct DrawRecord
    {
        enum Type
        {
            Squiggle,
            Shape,
            Bgcolor
        } type;
        size_t index;
    };

    std::vector<DrawRecord> drawOrder, redo;
    std::vector<DrawRecord> copydrawOrder, undoorder;
    // queue<DrawRecord> redo;

    // queue<bgcolorstruct> redocolor;

    wxMenu contextMenu;
    void BuildContextMenu();
    void OnContextMenuEvent(wxContextMenuEvent &);

    friend class ShapeChecker;

    // undo/redo:

    struct CanvasState
    {
        std::vector<DrawRecord> drawOrderundo;
        wxColour backgroundColor;
        bool color = false;

        // std::vector<ShapeData> shapeCopy; // Copy shapes separately
    };

    std::vector<CanvasState> undoStack;
    std::vector<CanvasState> redoStack;
};