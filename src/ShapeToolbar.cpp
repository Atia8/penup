#include "ShapeToolbar.h"
#include "drawingcanvas.h"
ShapeToolbar::ShapeToolbar(wxWindow *parent, DrawingCanvas *canvas) : wxPanel(parent),
                                                                      canvas(canvas), selectedButton(nullptr)
{

    wxScrolledWindow *scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(600, 90), wxVSCROLL | wxHSCROLL);
    scrolledWindow->SetScrollRate(5, 5); // Set scroll rate for smoother scrolling
                                         // auto mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create a sizer for the scrolled window
    auto sizer = new wxWrapSizer(wxHORIZONTAL);
    // mainSizer->Add(scrolledWindow, 0, wxALL | wxEXPAND, FromDIP(5));
    // wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxImage buttonImage(wxT("image\\square.png"), wxBITMAP_TYPE_PNG);
    buttonImage.Rescale(FromDIP(30), FromDIP(30)); // Resize the image if needed
    wxBitmap buttonBitmap(buttonImage);

    wxImage circleimage(wxT("image\\circle.png"), wxBITMAP_TYPE_PNG);
    circleimage.Rescale(FromDIP(30), FromDIP(30)); // Resize the image if needed
    wxBitmap circle(circleimage);

    wxImage lineimage(wxT("image\\line.png"), wxBITMAP_TYPE_PNG);
    lineimage.Rescale(FromDIP(30), FromDIP(30)); // Resize the image if needed
    wxBitmap line(lineimage);

    wxImage triangleimage(wxT("image\\triangle.png"), wxBITMAP_TYPE_PNG);
    triangleimage.Rescale(FromDIP(30), FromDIP(30)); // Resize the image if needed
    wxBitmap triangle(triangleimage);

    // for ellipse
    wxImage ellipseimage(wxT("image\\ovalshape.png"), wxBITMAP_TYPE_PNG);
    ellipseimage.Rescale(FromDIP(30), FromDIP(30)); // Resize the image if needed
    wxBitmap ellipse(ellipseimage);

    wxImage starimage(wxT("image\\star.png"), wxBITMAP_TYPE_PNG);
    starimage.Rescale(FromDIP(30), FromDIP(30)); // Resize the image if needed
    wxBitmap star(starimage);

    // // Add buttons for different shapes
    squareButton = new wxBitmapButton(scrolledWindow, wxID_ANY, buttonBitmap, wxDefaultPosition);
    circleButton = new wxBitmapButton(scrolledWindow, wxID_ANY, circle, wxDefaultPosition);
    lineButton = new wxBitmapButton(scrolledWindow, wxID_ANY, line, wxDefaultPosition);
    triangleButton = new wxBitmapButton(scrolledWindow, wxID_ANY, triangle, wxDefaultPosition);

    // ellipse
    ellipseButton = new wxBitmapButton(scrolledWindow, wxID_ANY, ellipse, wxDefaultPosition);

    starButton = new wxBitmapButton(scrolledWindow, wxID_ANY, star, wxDefaultPosition);

    sizer->Add(squareButton, 0, wxALL, 3);
    sizer->Add(circleButton, 0, wxALL, 3);
    sizer->Add(lineButton, 0, wxALL, 3);
    sizer->Add(triangleButton, 0, wxALL, 3);

    sizer->Add(ellipseButton, 0, wxALL, 3);

    sizer->Add(starButton, 0, wxALL, 3);
    // Set the sizer for the scrolled window
    scrolledWindow->SetSizer(sizer);
    scrolledWindow->FitInside();
    // scrolledWindow->SetScrollbars(10, 10, 50, 50); // Enable scrollbars
    scrolledWindow->SetVirtualSize(scrolledWindow->GetMinSize());
    //   Bind button events directly
    squareButton->Bind(wxEVT_BUTTON, &ShapeToolbar::OnShapeSelected, this);
    circleButton->Bind(wxEVT_BUTTON, &ShapeToolbar::OnShapeSelected, this);
    lineButton->Bind(wxEVT_BUTTON, &ShapeToolbar::OnShapeSelected, this);
    triangleButton->Bind(wxEVT_BUTTON, &ShapeToolbar::OnShapeSelected, this);

    ellipseButton->Bind(wxEVT_BUTTON, &ShapeToolbar::OnShapeSelected, this);
    starButton->Bind(wxEVT_BUTTON, &ShapeToolbar::OnShapeSelected, this);

    // Bind(wxEVT_BUTTON, &ShapeToolbar::OnShapeSelected, this);
    // scrolledWindow->SetSizer(sizer);
    // scrolledWindow->FitInside(); // Adjusts virtual size to match content
    // scrolledWindow->SetVirtualSize(wxSize(600, 300)); // Larger than the visible size

    // Add the scrolled window to the main toolbar sizer
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(scrolledWindow, 1, wxEXPAND | wxALL, 5);
    this->SetSizer(mainSizer);
}

void ShapeToolbar::OnShapeSelected(wxCommandEvent &event)
{
    // Logic to handle shape selection
    wxBitmapButton *button = dynamic_cast<wxBitmapButton *>(event.GetEventObject());
    // wxButton *button = dynamic_cast<wxButton *>(event.GetEventObject());

    if (button)
    {
        // selectedShape = button->GetLabel();
        if (button == squareButton)
        {
            selectedShape = "Square";
        }
        else if (button == circleButton)
        {
            selectedShape = "Circle";
        }
        else if (button == lineButton)
        {
            selectedShape = "Line";
        }
        else if (button == triangleButton)
        {
            selectedShape = "Triangle";
        }

        else if (button == ellipseButton)
        {
            selectedShape = "Ellipse";
        }

        else if (button == starButton)
        {
            selectedShape = "Star";
        }
        bool status = true;
        button->SetBackgroundColour(wxColour(173, 216, 230)); // Light blue for highlight
        button->Refresh();
        // Reset the previously selected button (if any)
        if (selectedButton && selectedButton != button)
        {
            selectedButton->SetBackgroundColour(wxNullColour); // Reset to default
            selectedButton->Refresh();

            // Remove focus from the previously selected button
            // selectedButton->SetFocusIgnoringChildren();
        }

        // Update the currently selected button
        selectedButton = button;

        // if (canvas == nullptr)
        // {
        //     wxLogMessage("null: ");
        // }
        canvas->shapestatus(status);
        canvas->SelectedShape(selectedShape);

        canvas->fillShape(false);
        // wxLogMessage("Selected shape: " + selectedShape);
        // Add your shape handling logic here
    }
}
void ShapeToolbar::setcanvas(DrawingCanvas *newcanvas)
{
    canvas = newcanvas;
}

void ShapeToolbar::disableButtonHighlight()
{
    if (selectedButton)
    {
        selectedButton->SetBackgroundColour(wxNullColour); // Reset to default
        selectedButton->Refresh();
    }
    selectedButton = nullptr;
}