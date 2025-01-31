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
    // if (drawOrder.empty())
    //     return;
    // if (squiggles.empty())
    //     return;
    // wxPaintDC dc(this);

    // wxAutoBufferedPaintDC dc(this);

    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if (gc)
    {
        for (const auto &record : drawOrder)
        {

            if (!squiggles.empty() && record.type == DrawRecord::Squiggle)
            {

                const auto &squiggle = squiggles[record.index];
                wxPen pen = squiggle.penType;
                if (squiggle.isEraser == true)
                {
                    pen.SetColour(this->GetBackgroundColour());
                    pen.SetStyle(wxPENSTYLE_SOLID);
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
            if (record.type == DrawRecord::Shape)
            {

                const auto &shape = shapes[record.index];

                ShapeDrawer drawer(dc, shape.color, shape.size, shape.fillcolor, shape.penTyp, shape.fillshape);

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

        // this->SetBackgroundColour(newcolor);
        delete gc;
    }
    // this->SetBackgroundColour(newcolor);
}

void DrawingCanvas::OnMouseDown(wxMouseEvent &event)
{

    if (fillshape == true)
    {
        // wxLogMessage("inside fillshape");
        clickedpoint = event.GetPosition();

        // undoStack.push_back({copydrawOrder, currentBGColor, true});
        ShapeChecker::CheckClickedShape(clickedpoint, drawOrder, shapes, penColor, this, backcolor);

        // wxLogMessage("size:%zu", drawOrder.size());

        Refresh();
        fillshape = false;
        fillShape(false);
        // wxLogMessage("after refresh size:%zu", drawOrder.size());
    }

    // ShapeChecker::CheckClickedShape(clickedpoint, drawOrder, shapes);

    else if ((shapeactive == false))
    {
        // wxLogMessage("inside squiggle");

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

        // undoStack.push_back({drawOrder, this->GetBackgroundColour(), false});
        drawOrder.push_back({DrawRecord::Squiggle, squiggles.size() - 1});
    }

    else if (shapeactive == true)
    {
        // shapeStartPoint = event.GetPosition();
        // wxLogMessage("inside shape");

        squiggle = false;

        shapeCopy = shapes;
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
        shapes.back().fillshape = false;
        // shapes.push_back(newShape);

        undoStack.push_back({drawOrder, this->GetBackgroundColour(), false});

        drawOrder.push_back({DrawRecord::Shape, shapes.size() - 1});
        // shapestatus(false);
        // wxLogMessage("size:%zu", drawOrder.size());
    }
}

//}

void DrawingCanvas::OnMouseMove(wxMouseEvent &event)
{

    if (isDrawing && shapeactive == false)
    {
        // wxLogMessage("squiggle");

        if (squiggles.empty())
        {
            // No squiggle to update; return early.
            return;
        }
        auto pt = event.GetPosition();
        auto &currentSquiggle = squiggles.back().points;
        currentSquiggle.push_back(pt);

        // if (currentSquiggle.size() == 1)
        // {
        //     if (!squiggles.empty())
        //     {
        //         wxLogMessage("After pop_back: squiggles size = %zu", squiggles.size());
        //         squiggles.pop_back();
        //         // wxLogMessage("After pop_back: squiggles size = %zu", squiggles.size());
        //     }
        //     // squiggles.pop_back();
        //     if (!drawOrder.empty())
        //         drawOrder.pop_back();
        // }

        Refresh();
        //    enablesquiggle(false);
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

    // wxColour currentBGColor = this->GetBackgroundColour();

    // if (!undoStack.empty() && undoStack.back().backgroundColor != currentBGColor)
    // {
    //     // Only push the background color change if it differs from the previous one
    //     undoStack.push_back({drawOrder, currentBGColor, true});
    // }
    // else
    //     undoStack.push_back({drawOrder, this->GetBackgroundColour(), false});

    isDrawing = false;
    if (!shapeactive) // squiggle drawing mode
    {
        if (!squiggles.empty())
        {
            // Decide if this squiggle was accidental.
            // Option 1: Check if only one point was recorded.
            bool accidental = (squiggles.back().points.size() <= 1);

            // Option 2: Or check if the distance moved is below a threshold.
            // For example, if the distance between the first and last point is less than 5 pixels:
            // auto &pts = squiggles.back().points;
            // double dx = pts.back().x - pts.front().x;
            // double dy = pts.back().y - pts.front().y;
            // bool accidental = (sqrt(dx*dx + dy*dy) < 5);

            if (accidental)
            {
                // Remove the squiggle and its corresponding record.
                squiggles.pop_back();
                if (!drawOrder.empty() && drawOrder.back().type == DrawRecord::Squiggle)
                {
                    drawOrder.pop_back();
                }
            }
        }
        Refresh();
    }

    else if ((shapeactive == true) && (!shapes.empty()))
    {
        // wxAutoBufferedPaintDC dc(this);

        // wxAutoBufferedPaintDC dc(this);
        wxClientDC dc(this); // Create a device context to draw on the canvas
                             // ShapeDrawer drawer(dc, penColor, penSize, penColor); // here also i added pencolor due to fillcolor

        // Redraw the shape when mouse is up
        ShapeData &currentShape = shapes.back();
        ShapeDrawer drawer(dc, penColor, penSize, currentShape.fillcolor, currentShape.penTyp, currentShape.fillshape);

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
        shapestatus(false);

        // shapeactive = false;
    }
    //  wxLogMessage("size:%zu", drawOrder.size());
}

void DrawingCanvas::OnMouseLeave(wxMouseEvent &)
{
    // wxLogMessage("size:%zu", drawOrder.size());

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

void DrawingCanvas::Undo()
{

    // wxLogMessage("size:%zu", drawOrder.size());

    if (!drawOrder.empty())
    {
        const DrawRecord &lastRecord = drawOrder.back();

        redo.push_back(lastRecord);

        drawOrder.pop_back();
        if ((lastRecord.type == DrawRecord::Bgcolor) && !backcolor.empty())
        {

            redocolor.push_back(backcolor.back());
            backcolor.pop_back();
            if (backcolor.empty())
            {
                this->SetBackgroundColour(*wxBLACK);
            }
            else
                this->SetBackgroundColour(backcolor.back().bgcolor);
        }
    }
    Refresh();
}

void DrawingCanvas::Redo()
{

    // wxLogMessage("size:%zu", redocolor.size());

    if (!redo.empty())
    {
        const DrawRecord &lastRecord = redo.back();

        // redo.push(lastRecord);
        redo.pop_back();
        drawOrder.push_back(lastRecord);
        if ((lastRecord.type == DrawRecord::Bgcolor) && !redocolor.empty())
        {
            // redocolor.push(backcolor.back());
            backcolor.push_back(redocolor.back());
            // redocolor.pop_back();
            if (redocolor.empty())
            {
                this->SetBackgroundColour(*wxBLACK);
            }
            else
                this->SetBackgroundColour(redocolor.back().bgcolor);

            redocolor.pop_back();
        }
    }
    Refresh();
}

void DrawingCanvas::enablesquiggle(bool active)
{
    squiggle = active;
}