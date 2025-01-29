#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include "drawingcanvas.h"
#include <wx/regex.h> // For wxRegEx
#include <stack>
#include "ShapeChecker.h"

DrawingCanvas::DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxWindow(parent, id, pos, size), penSize(5), eraserTool(nullptr), eraserToolActive(false), shapeactive(true)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->SetBackgroundColour(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE : *wxBLACK);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
    this->Bind(wxEVT_LEFT_DOWN, &DrawingCanvas::OnMouseDown, this);
    this->Bind(wxEVT_MOTION, &DrawingCanvas::OnMouseMove, this);
    this->Bind(wxEVT_LEFT_UP, &DrawingCanvas::OnMouseUp, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &DrawingCanvas::OnMouseLeave, this);

    BuildContextMenu();

    this->Bind(wxEVT_CONTEXT_MENU, &DrawingCanvas::OnContextMenuEvent, this);
}

void DrawingCanvas::OnPaint(wxPaintEvent &)
{
    wxAutoBufferedPaintDC dc(this);

    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if (gc)
    {
        for (const auto &record : drawOrder)
        {
            if (record.type == DrawRecord::Squiggle)
            {
                const auto &squiggle = squiggles[record.index];
                wxPen pen = squiggle.penType;
                if (squiggle.isEraser == true)
                {
                    pen.SetColour(this->GetBackgroundColour());
                }
                else
                {
                    pen.SetColour(squiggle.color);
                }
                pen.SetWidth(squiggle.Size);
                gc->SetPen(pen);

                if (squiggle.points.size() > 1)
                {
                    gc->StrokeLines(squiggle.points.size(), squiggle.points.data());
                }
            }
            else if (record.type == DrawRecord::Shape)
            {
                const auto &shape = shapes[record.index];
                ShapeDrawer drawer(dc, shape.color, shape.size, shape.fillcolor, shape.penTyp);

                if (shape.shapeType == "Square")
                {
                    drawer.DrawSquare(shape.startPoint, shape.endPoint, shape.fillcolor);
                }
                else if (shape.shapeType == "Circle")
                {
                    drawer.DrawCircle(shape.startPoint, shape.endPoint);
                }
                else if (shape.shapeType == "Line")
                {
                    drawer.DrawLine(shape.startPoint, shape.endPoint);
                }
                else if (shape.shapeType == "Triangle")
                {
                    drawer.DrawTriangle(shape.startPoint, shape.endPoint);
                }
                else if (shape.shapeType == "Ellipse")
                {
                    drawer.DrawEllipse(shape.startPoint, shape.endPoint);
                }
                else if (shape.shapeType == "Star")
                {
                    drawer.DrawStar(shape.startPoint, shape.endPoint);
                }
            }
        }

        // for (const auto &shape : shapes)
        // {
        //     // Create a ShapeDrawer to draw each shape in the container
        //     ShapeDrawer drawer(dc, shape.color, shape.size);

        //     if (shape.shapeType == "Square")
        //     {
        //         drawer.DrawSquare(shape.startPoint, shape.endPoint);
        //     }
        //     else if (shape.shapeType == "Circle")
        //     {
        //         drawer.DrawCircle(shape.startPoint, shape.endPoint);
        //     }
        //     else if (shape.shapeType == "Line")
        //     {
        //         drawer.DrawLine(shape.startPoint, shape.endPoint);
        //     }
        //     else if (shape.shapeType == "Triangle")
        //     {
        //         drawer.DrawTriangle(shape.startPoint, shape.endPoint);
        //     }
        // }
        // for (const auto &pair : squiggles)
        // {
        //     // const auto &pointsVector = pair.first;
        //     // const auto &color = pair.second;

        //     const auto &points = pair.points;
        //     const auto &color = pair.color;
        //     const auto &penType = pair.penType;
        //     int size = pair.Size;

        //     wxPen pen = pair.penType; // Use the currentPen
        //                               // wxPen pen = PenManager::GetPen(penType);

        //     pen.SetColour(color);
        //     pen.SetWidth(size);
        //     // Set the color of the pen
        //     // pen.SetStyle(pair.penType);
        //     gc->SetPen(pen);

        //     // gc->SetPen(wxPen(color, 8));
        //     if (points.size() > 1)
        //     {
        //         gc->StrokeLines(points.size(), points.data());
        //     }
        // }

        delete gc;
    }
}

void DrawingCanvas::OnMouseDown(wxMouseEvent &event)
{

    // if (eraserTool->activestatus() == false)
    // this->SetCursor(wxCursor(wxCURSOR_DEFAULT)); // Revert to default cursor
    if (fillshape == true)
    {
        clickedpoint = event.GetPosition();
        ShapeChecker::CheckClickedShape(clickedpoint, drawOrder, shapes, penColor, this);
        Refresh();
    }

    // ShapeChecker::CheckClickedShape(clickedpoint, drawOrder, shapes);

    if (shapeactive == false)
    {
        squiggles.push_back(Squiggle{{}, penColor, currentPen, penSize});
        if (eraserstatus == true)
        {
            squiggles.back().isEraser = true;
            squiggles.back().color = this->GetBackgroundColour();
        }
        else

        {
            squiggles.back().color = penColor; // Set the current pen color for this squiggle
        }
        isDrawing = true;

        drawOrder.push_back({DrawRecord::Squiggle, squiggles.size() - 1});
    }

    if (shapeactive == true)
    {
        // shapeStartPoint = event.GetPosition();

        ShapeData newShape;

        shapes.push_back(newShape);
        shapes.back().shapeType = selectedShape;
        // newShape.startPoint = shapeStartPoint;
        shapes.back().startPoint = event.GetPosition();

        shapes.back().endPoint = event.GetPosition();
        // newShape.endPoint.x += 100;
        // newShape.endPoint.y += 0;
        shapes.back().color = penColor;
        shapes.back().size = penSize;

        shapes.back().penTyp = currentPen;
        // shapes.push_back(newShape);
        drawOrder.push_back({DrawRecord::Shape, shapes.size() - 1});
    }
}

bool DrawingCanvas::IsPointInSquare(const wxPoint &point, const wxPoint &start, const wxPoint &end)
{
    int xMin = std::min(start.x, end.x);
    int xMax = std::max(start.x, end.x);
    int yMin = std::min(start.y, end.y);
    int yMax = std::max(start.y, end.y);
    return point.x >= xMin && point.x <= xMax && point.y >= yMin && point.y <= yMax;
}

//}

void DrawingCanvas::OnMouseMove(wxMouseEvent &event)
{
    if (isDrawing && shapeactive == false)
    {
        // wxLogMessage("squiggle");
        auto pt = event.GetPosition();
        auto &currentSquiggle = squiggles.back().points;

        currentSquiggle.push_back(pt);
        Refresh();
    }

    else if (shapeactive == true && event.Dragging())
    {
        // wxLogMessage("shape");
        // shapeEndPoint = event.GetPosition();

        // Update the last shape's endPoint in the container
        if (!shapes.empty())
        {
            shapeEndPoint = event.GetPosition();

            shapes.back().endPoint = shapeEndPoint;
        }
        // wxLogMessage("shape");
    }
}

void DrawingCanvas::OnMouseUp(wxMouseEvent &)
{
    isDrawing = false;
    if ((shapeactive == true) && (!shapes.empty()))
    {
        // wxAutoBufferedPaintDC dc(this);

        // wxAutoBufferedPaintDC dc(this);
        wxClientDC dc(this); // Create a device context to draw on the canvas
                             // ShapeDrawer drawer(dc, penColor, penSize, penColor); // here also i added pencolor due to fillcolor

        // Redraw the shape when mouse is up
        ShapeData &currentShape = shapes.back();
        ShapeDrawer drawer(dc, penColor, penSize, currentShape.fillcolor, currentShape.penTyp);

        if (currentShape.shapeType == "Square")
        {

            drawer.DrawSquare(currentShape.startPoint, currentShape.endPoint, currentShape.fillcolor);
        }
        else if (currentShape.shapeType == "Circle")
        {
            drawer.DrawCircle(currentShape.startPoint, currentShape.endPoint);
        }
        else if (currentShape.shapeType == "Line")
        {
            drawer.DrawLine(currentShape.startPoint, currentShape.endPoint);
        }
        else if (currentShape.shapeType == "Triangle")
        {
            drawer.DrawTriangle(currentShape.startPoint, currentShape.endPoint);
        }

        else if (currentShape.shapeType == "Ellipse")
        {
            drawer.DrawEllipse(currentShape.startPoint, currentShape.endPoint);
        }

        else if (currentShape.shapeType == "Star")
        {
            drawer.DrawStar(currentShape.startPoint, currentShape.endPoint);
        }
        // shapeactive = false;
    }
}

void DrawingCanvas::OnMouseLeave(wxMouseEvent &)
{
    isDrawing = false;
    // shapeactive = false;
}

void DrawingCanvas::BuildContextMenu()
{
    auto clear = contextMenu.Append(wxID_ANY, "&Clear");
    contextMenu.Append(wxID_ANY, "Save &As...");

    this->Bind(
        wxEVT_MENU,
        [this](wxCommandEvent &)
        {
            this->drawOrder.clear();
            this->squiggles.clear();
            this->shapes.clear();
            this->SetBackgroundColour(*wxBLACK);
            this->Refresh();
        },
        clear->GetId());
}

void DrawingCanvas::OnContextMenuEvent(wxContextMenuEvent &e)
{
    auto clientPos = e.GetPosition() == wxDefaultPosition
                         ? wxPoint(this->GetSize().GetWidth() / 2, this->GetSize().GetHeight() / 2)
                         : this->ScreenToClient(e.GetPosition());
    PopupMenu(&this->contextMenu, clientPos);
}

void DrawingCanvas::SetPenColor(const wxColour &color)
{
    // wxLogMessage("Erasercolor is active, handling mouse move.");
    penColor = color; // Update the pen color
                      // Refresh();
}

void DrawingCanvas::SetPenType(PenType type)
{
    currentPen = PenManager::GetPen(type);
    Refresh(); // Refresh the canvas to reflect changes
}

void DrawingCanvas::SetPenSize(int size)
{
    penSize = size;

    // Update the pen drawing settings with the new size
    Refresh(); // Repaint the canvas if needed
}
int DrawingCanvas::GetPenSize()
{
    // shapeactive = true;
    return penSize;
}

bool DrawingCanvas::shapestatus(bool status)
{
    shapeactive = status;
    return shapeactive;
}

void DrawingCanvas::SelectedShape(wxString shape)
{
    selectedShape = shape;
}

void DrawingCanvas::fillShape(bool active)
{
    fillshape = active;
}

void DrawingCanvas::eraser(bool active)
{
    eraserstatus = active;
}