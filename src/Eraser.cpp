#include "Eraser.h"
#include "DrawingCanvas.h"

Eraser::Eraser(DrawingCanvas *canvas)
    : canvas(canvas), isActive(false)
{
}

void Eraser::ToggleEraser()
{
    isActive = true;
    if (isActive)
    {

        // wxImage eraserImage("C:\\penup\\wx_cmake_template\\src\\eraser.png", wxBITMAP_TYPE_PNG);
        wxImage eraserImage("image\\erasercursor.png", wxBITMAP_TYPE_PNG);

        int size = canvas->GetPenSize();
        size = CheckDefaultSize(size);
        eraserImage = eraserImage.Scale(size, size);
        wxCursor eraserCursor(eraserImage);
        canvas->SetCursor(eraserCursor); // Change cursor to cross or custom eraser
        canvas->SetPenColor(canvas->GetBackgroundColour());

        canvas->eraser(true);
    }
}

// bool Eraser::activestatus()
// {
//     return isActive;
// }

// void Eraser::OnMouseMove(wxMouseEvent &event)
// {
//     // wxLogMessage("Eraser class is active, handling mouse move.");

//     if (isActive)
//     {
//         wxPoint pt = event.GetPosition();

//         // Remove the portion of the squiggle where the mouse is located
//         // Implement logic to erase points from the squiggle
//         canvas->SetPenColor(*wxBLACK);
//         // canvas->EraseAtPoint(pt);
//     }

int Eraser::CheckDefaultSize(int size)
{
    int defaultsize = 18;
    if (size < 18)
        return defaultsize;
    else
        return size;
}