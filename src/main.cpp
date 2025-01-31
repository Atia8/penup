#include <wx/wx.h>
#include <wx/wrapsizer.h>
#include <wx/splitter.h>
#include <wx/colordlg.h>

#include <string>
#include <vector>

#include "colorpane.h"
#include "PenManager.h"
#include "PenSizePanel.h"

// #include "pensizepane.h"

#include "drawingcanvas.h"
#include "Eraser.h"
#include "ShapeToolbar.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void OnSelectColor(wxCommandEvent &event);

    wxPanel *BuildControlsPanel(wxWindow *parent);

    void SetupColorPanes(wxWindow *parent, wxSizer *sizer);
    // void SetupPenPanes(wxWindow *parent, wxSizer *sizer);

    void SelectColorPane(ColorPane *pane);
    // void SelectPenPane(PenSizePane *pane);

    std::vector<ColorPane *> colorPanes{};
    // std::vector<PenSizePane *> penPanes{};

    DrawingCanvas *canvas = nullptr;

    PenManager *penManager;

    const std::vector<std::string> niceColors = {"#000000", "#ffffff", "#fd7f6f",
                                                 "#7eb0d5", "#b2e061", "#bd7ebe",
                                                 "#ffb55a", "#ffee65", "#beb9db",
                                                 "#fdcce5", "#8bd3c7"};

    // const std::vector<std::string> niceColors = {"#000000", "#ffffff", "#fd7f6f"};

    const int penCount = 6;

    const std::string lightBackground = "#f4f3f3";
    const std::string darkBackground = "#2c2828";

    PenSizePanel *penSizePanel = nullptr;
    wxSlider *penSizeSlider = nullptr;

    // Eraser
    Eraser *eraser = nullptr;
    ShapeToolbar *shapeToolbar = nullptr;

    void OnEraserButton(wxCommandEvent &event);
    void cursorselect();
    void disableEraser();
    void disableshape();
    void disablecolorfiller();
};

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    MyFrame *frame = new MyFrame("Hello World", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
}

void MyFrame::SetupColorPanes(wxWindow *parent, wxSizer *sizer)
{

    // canvas->SetCursor(wxCursor(wxCURSOR_DEFAULT)); // Revert to default cursor

    for (const auto &color : niceColors)
    {
        auto colorPane = new ColorPane(parent, wxID_ANY, wxColour(color));

        colorPane->Bind(wxEVT_LEFT_DOWN, [this, colorPane](wxMouseEvent &event)
                        { SelectColorPane(colorPane);
                          canvas->SetPenColor(colorPane->GetColor()); });

        colorPanes.push_back(colorPane);
        sizer->Add(colorPane, 0, wxRIGHT | wxBOTTOM, FromDIP(5));
    }
}

wxPanel *MyFrame::BuildControlsPanel(wxWindow *parent)
{

    auto controlsPanel = new wxScrolled<wxPanel>(parent, wxID_ANY);
    controlsPanel->SetScrollRate(0, FromDIP(10));

    bool isDark = wxSystemSettings::GetAppearance().IsDark();

    controlsPanel->SetBackgroundColour(wxColour(isDark ? darkBackground : lightBackground));

    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    auto colorPaneSizer = new wxWrapSizer(wxHORIZONTAL);
    SetupColorPanes(controlsPanel, colorPaneSizer);

    mainSizer->Add(colorPaneSizer, 0, wxALL | wxEXPAND, FromDIP(5));
    auto button = new wxButton(controlsPanel, wxID_ANY, "Save As...");

    // Add the Select Color button
    wxImage rainbowImage(wxT("image//rainbow.png"), wxBITMAP_TYPE_PNG);
    rainbowImage.Rescale(FromDIP(45), FromDIP(45));
    wxBitmap rainbowBitmap(rainbowImage);

    // wxBitmap rainbowBitmap(wxT("C:\\Users\\atiaz\\OneDrive\\Pictures\\Screenshots\\Screenshot 2024-12-06 010320.png"), wxBITMAP_TYPE_PNG);

    auto rainbowButton = new wxBitmapButton(controlsPanel, wxID_ANY, rainbowBitmap);

    // auto colorButton = new wxButton(controlsPanel, wxID_ANY, "Select Color");
    colorPaneSizer->Add(rainbowButton, 0, wxALL, FromDIP(5));

    // mainSizer->Add(rainbowButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, FromDIP(5));

    // Bind the button to open the color dialog
    rainbowButton->Bind(wxEVT_BUTTON, &MyFrame::OnSelectColor, this);

    // mainSizer->AddStretchSpacer();
    // mainSizer->Add(button, 0, wxALL, FromDIP(5));
    mainSizer->AddSpacer(FromDIP(8));

    controlsPanel->SetSizer(mainSizer);

    wxBoxSizer *penTypeSizer = new wxWrapSizer(wxHORIZONTAL);

    // Load images for each pen type
    wxImage solidImage(wxT("image\\brush.png"), wxBITMAP_TYPE_PNG);
    wxImage dashedImage(wxT("image\\artpen2.png"), wxBITMAP_TYPE_PNG);
    wxImage dottedImage(wxT("image\\pen3.png"), wxBITMAP_TYPE_PNG);
    wxImage brushImage(wxT("image\\pen4.png"), wxBITMAP_TYPE_PNG);

    // new
    wxImage customdashImage(wxT("image\\pen5.png"), wxBITMAP_TYPE_PNG);
    wxImage redlineImage(wxT("image\\pen6.png"), wxBITMAP_TYPE_PNG);

    if (!brushImage.HasAlpha())
    {

        wxLogError("The image does not have an alpha channel.");
    }

    wxBitmap transparentBitmap(brushImage);

    // Rescale the images if needed
    solidImage.Rescale(FromDIP(30), FromDIP(30));
    dashedImage.Rescale(FromDIP(30), FromDIP(30));
    dottedImage.Rescale(FromDIP(30), FromDIP(30));
    brushImage.Rescale(FromDIP(30), FromDIP(30));

    // new
    customdashImage.Rescale(FromDIP(30), FromDIP(30));
    redlineImage.Rescale(FromDIP(30), FromDIP(30));

    // Create bitmap buttons for each pen type
    auto solidButton = new wxBitmapButton(controlsPanel, wxID_ANY, wxBitmap(solidImage));
    auto dashedButton = new wxBitmapButton(controlsPanel, wxID_ANY, wxBitmap(dashedImage));
    auto dottedButton = new wxBitmapButton(controlsPanel, wxID_ANY, wxBitmap(dottedImage));
    auto brushButton = new wxBitmapButton(controlsPanel, wxID_ANY, wxBitmap(brushImage));

    // new
    auto customdashButton = new wxBitmapButton(controlsPanel, wxID_ANY, wxBitmap(customdashImage));
    auto redlineButton = new wxBitmapButton(controlsPanel, wxID_ANY, wxBitmap(redlineImage));

    dashedButton->SetSize(dashedImage.GetWidth(), dashedImage.GetHeight());
    // Bind events to change pen type
    solidButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                      { canvas->SetPenType(PenType::SOLID);
                      disablecolorfiller();
                      canvas->enablesquiggle(true); });
    dashedButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                       { canvas->SetPenType(PenType::DASHED); 
                         disablecolorfiller();
                         canvas->enablesquiggle(true); });
    dottedButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                       { canvas->SetPenType(PenType::DOTTED); 
                         disablecolorfiller(); });
    brushButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                      { canvas->SetPenType(PenType::BRUSH);
                        disablecolorfiller(); });

    // new
    customdashButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                           { canvas->SetPenType(PenType::CUSTOM_DASH); 
                             disablecolorfiller(); });
    redlineButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                        { canvas->SetPenType(PenType::THICK_RED_LINE);
                          disablecolorfiller(); });

    // Add buttons to the sizer
    penTypeSizer->Add(solidButton, 0, wxALL | wxEXPAND, FromDIP(5));
    penTypeSizer->Add(dashedButton, 0, wxALL | wxEXPAND, FromDIP(5));
    penTypeSizer->Add(dottedButton, 0, wxALL | wxEXPAND, FromDIP(5));
    penTypeSizer->Add(brushButton, 0, wxALL | wxEXPAND, FromDIP(5));

    // new
    penTypeSizer->Add(customdashButton, 0, wxALL | wxEXPAND, FromDIP(5));
    penTypeSizer->Add(redlineButton, 0, wxALL | wxEXPAND, FromDIP(5));

    // Add the pen type sizer to the main sizer
    mainSizer->Add(penTypeSizer, 0, wxRIGHT | wxBOTTOM, FromDIP(5));

    mainSizer->AddSpacer(FromDIP(3));
    mainSizer->Add(button, 0, wxALL, FromDIP(5));

    // auto penSizeButton = new wxButton(controlsPanel, wxID_ANY, "");
    wxImage img(wxT("image\\pensize.jpg"), wxBITMAP_TYPE_JPEG);
    img.Rescale(FromDIP(40), FromDIP(40)); // Resize image to fit your button size
    wxBitmap resizedBitmap(img);           // Convert the resized image to a bitmap
    auto penSizeButton = new wxBitmapButton(controlsPanel, wxID_ANY, wxBitmap(img));

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(buttonSizer, 0, wxALL | wxEXPAND, FromDIP(5));

    // controlsPanel->SetSizer(buttonSizer);

    buttonSizer->Add(penSizeButton, 0, wxALL, FromDIP(5));

    // mainSizer->Add(penSizeButton, 0, wxALL, FromDIP(5));

    penSizeButton->Bind(wxEVT_BUTTON, [this, controlsPanel, mainSizer, buttonSizer](wxCommandEvent &event)
                        {
                            if (penSizeSlider == nullptr)
                            {
                                penSizeSlider = new wxSlider(controlsPanel, wxID_ANY, 5, 1, 50, // Default pen size is 5, range from 1 to 50
                                                             wxDefaultPosition, wxDefaultSize,
                                                             wxSL_HORIZONTAL | wxSL_LABELS);

                                // Add the slider to the sizer
                                // mainSizer->Add(penSizeSlider, 0, wxALL | wxEXPAND, FromDIP(5));
                               // wxSizer *buttonSizer = controlsPanel->GetSizer(); // Get the sizer managing controlsPanel
                                if (buttonSizer)
                                {
                                    buttonSizer->Add(penSizeSlider, 0, wxALL | wxEXPAND, FromDIP(5)); // Add slider with padding
                                }

                                penSizeSlider->Bind(wxEVT_SLIDER, [this](wxCommandEvent &event)
                                                    {
                                                        int penSize = event.GetInt(); // Get the current value from the slider
                                                        canvas->SetPenSize(penSize);  // Set the pen size on the canvas
                                                        if(eraser!=nullptr)
                                                        eraser->ToggleEraser(); });
                                controlsPanel->Layout();
                            }

                            else
                            {
                                // Toggle visibility of the slider
                                penSizeSlider->Show(!penSizeSlider->IsShown());
                                controlsPanel->Layout(); // Re-layout the panel
                            } });

    // mainSizer->AddSpacer(FromDIP(3));
    wxImage eraserImage(wxT("image\\eraser.png"), wxBITMAP_TYPE_PNG);
    eraserImage.Rescale(FromDIP(30), FromDIP(30));
    wxBitmap eraserBitmap(eraserImage);

    auto eraserButton = new wxBitmapButton(controlsPanel, wxID_ANY, eraserBitmap);
    penTypeSizer->Add(eraserButton, 0, wxALL | wxEXPAND, FromDIP(5));
    eraserButton->Bind(wxEVT_BUTTON, &MyFrame::OnEraserButton, this);
    // eraserButton->Bind(wxEVT_BUTTON, &MyFrame::cursorselect, this);
    mainSizer->AddSpacer(FromDIP(3));

    //  Create a smaller scrollable panel
    auto nestedScrollPanel = new wxPanel(controlsPanel, wxID_ANY);
    nestedScrollPanel->SetBackgroundColour(*wxWHITE); // Optional: Highlight the nested panel
    wxBoxSizer *nestedSizer = new wxBoxSizer(wxVERTICAL);
    nestedScrollPanel->SetSizer(nestedSizer);
    nestedScrollPanel->FitInside();                  // Ensure it fits its content
    nestedScrollPanel->SetMinSize(wxSize(200, 100)); // Set minimum size for visibility

    // Add the nested scroll panel to the main window
    mainSizer->Add(nestedScrollPanel, 0, wxALL, 5);

    shapeToolbar = new ShapeToolbar(nestedScrollPanel, canvas);

    nestedSizer->Add(shapeToolbar, 0, wxALL, 5);

    // Inside the BuildControlsPanel method

    // Add the Color Fill button
    wxImage fillImage(wxT("image\\fillshape.png"), wxBITMAP_TYPE_PNG); // Replace with your fill icon
    fillImage.Rescale(FromDIP(30), FromDIP(30));
    wxBitmap fillBitmap(fillImage);
    auto fillButton = new wxBitmapButton(controlsPanel, wxID_ANY, fillBitmap);

    // Bind the button to trigger color fill
    fillButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                     {
    if (canvas) {
        canvas->fillShape(true); // Add this method in your DrawingCanvas class
    } });

    // Add the button to the sizer
    // mainSizer->Add(fillButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, FromDIP(5));

    penTypeSizer->Add(fillButton, 0, wxALL | wxEXPAND, FromDIP(5));

    // undo

    wxBoxSizer *undoSizer = new wxWrapSizer(wxHORIZONTAL);

    wxImage undoImage(wxT("image\\undo.png"), wxBITMAP_TYPE_PNG); // Replace with your fill icon
    undoImage.Rescale(FromDIP(30), FromDIP(30));
    wxBitmap undoBitmap(undoImage);
    auto undoButton = new wxBitmapButton(controlsPanel, wxID_ANY, undoBitmap);

    // Bind the button to trigger color fill
    undoButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                     {
    if (canvas) {
        canvas->Undo(); // Add this method in your DrawingCanvas class
    } });

    undoSizer->Add(undoButton, 0, wxALL | wxEXPAND, FromDIP(5));

    // mainSizer->Add(undoButton, 0, wxALL | wxEXPAND, FromDIP(5));

    // redo
    wxImage redoImage(wxT("image\\redo.png"), wxBITMAP_TYPE_PNG); // Replace with your fill icon
    redoImage.Rescale(FromDIP(30), FromDIP(30));
    wxBitmap redoBitmap(redoImage);
    auto redoButton = new wxBitmapButton(controlsPanel, wxID_ANY, redoBitmap);

    // Bind the button to trigger color fill
    redoButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                     {
    if (canvas) {
        canvas->Redo(); // Add this method in your DrawingCanvas class
    } });

    undoSizer->Add(redoButton, 0, wxALL | wxEXPAND, FromDIP(5));
    mainSizer->Add(undoSizer, 0, wxRIGHT | wxBOTTOM, FromDIP(5));

    // mainSizer->Add(redoButton, 0, wxALL | wxEXPAND, FromDIP(5));

    controlsPanel->SetSizer(mainSizer);
    controlsPanel->FitInside();
    return controlsPanel;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);

    splitter->SetMinimumPaneSize(FromDIP(150)); // sets a min pane size->cannot be less than 150

    auto controlsPanel = BuildControlsPanel(splitter);
    canvas = new DrawingCanvas(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    shapeToolbar->setcanvas(canvas);
    // eraser
    // Eraser *erase = nullptr;
    // eraser = new Eraser(20, canvas->GetBackgroundColour());

    // wxButton *eraserButton = new wxButton(controlsPanel, wxID_ANY, "Eraser");
    // // eraserButton->Bind(wxEVT_BUTTON, &MyFrame::OnEraserButton, this);

    // eraserButton->Bind(wxEVT_BUTTON, [this, controlsPanel, eraserButton](wxCommandEvent &event)
    //                    { MyFrame::OnEraserButton(event, eraserButton); });

    // penManager = new PenManager(canvas);

    splitter->SplitVertically(controlsPanel, canvas);
    splitter->SetSashPosition(FromDIP(220)); // initial position of divider. leaves 220 pixels for the left

    // Create a sizer for the frame and add the splitter to it.
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(splitter, 1, wxEXPAND); // proportion 1 and expand flag make splitter resize with frame

    // Set the sizer on the frame.
    this->SetSizer(sizer);
    this->Layout();

    this->SetSize(FromDIP(800), FromDIP(500));      // sets the size of the window
    this->SetMinSize({FromDIP(400), FromDIP(200)}); // min size of window

    SelectColorPane(colorPanes[0]);

    wxPanel *panel = new wxPanel(this, wxID_ANY);
}

void MyFrame::SelectColorPane(ColorPane *pane)
{
    disableEraser();
    disableshape();
    this->SetFocus(); // Set focus to the panel
    // canvas->SetCursor(wxCursor(wxCURSOR_DEFAULT)); // Revert to default cursor

    for (auto colorPane : colorPanes)
    {
        colorPane->selected = (colorPane == pane);

        colorPane->Refresh();
    }
}

void MyFrame::OnSelectColor(wxCommandEvent &event)
{

    disableEraser();
    disableshape();
    this->SetFocus(); // Set focus to the panel
    wxColourData data;
    wxColourDialog dialog(this, &data);

    // Show the color dialog
    if (dialog.ShowModal() == wxID_OK)
    {
        // Get the color selected by the user
        wxColour selectedColor = dialog.GetColourData().GetColour();
        canvas->SetPenColor(selectedColor);

        // Example: Change the background color of the frame to the selected color
        // this->SetBackgroundColour(selectedColor);
        // this->Refresh(); // Refresh to apply the new background color
    }
}
void MyFrame::OnEraserButton(wxCommandEvent &event)
{

    // Toggle the eraser tool
    if (!eraser)
    {
        eraser = new Eraser(canvas); // Assuming you have an Eraser class
    }

    // Activate the eraser tool

    eraser->ToggleEraser();
    disableshape();
    // eraser = nullptr;
    // canvas->SetCursor(wxCursor(wxCURSOR_DEFAULT)); // Revert to default cursor
}

// void MyFrame::cursorselect()
// {
//     wxImage eraserImage("C:\\penup\\wx_cmake_template\\src\\eraser.png", wxBITMAP_TYPE_PNG);
//     eraserImage = eraserImage.Scale(28, 28);
//     wxCursor eraserCursor(eraserImage);
//     canvas->SetCursor(eraserCursor); // Change cursor to cross or custom eraser
// }

void MyFrame::disableEraser()
{
    eraser = nullptr;
    canvas->SetCursor(wxCursor(wxCURSOR_DEFAULT));
    canvas->eraser(false);
}

void MyFrame::disableshape()
{
    canvas->shapestatus(false);
    shapeToolbar->disableButtonHighlight();
}

void MyFrame::disablecolorfiller()
{

    canvas->fillShape(false);
    // shapeToolbar->disableButtonHighlight();
}

// #include <wx/wx.h>
// #include <wx/scrolwin.h>
// #include "ShapeToolbar.h"
// class MyFrame : public wxFrame
// {
// public:
//     MyFrame()
//         : wxFrame(nullptr, wxID_ANY, "Window with Nested Scroll Panel", wxDefaultPosition, wxSize(800, 600))
//     {

//         // Entire window is a scrollable panel
//         auto *mainScrollWindow = new wxScrolled<wxPanel>(this, wxID_ANY);
//         mainScrollWindow->SetScrollRate(10, 10);

//         // Sizer for the main scrollable window
//         wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

//         // Add some static content to the main window
//         for (int i = 0; i < 50; ++i)
//         {
//             mainSizer->Add(new wxStaticText(mainScrollWindow, wxID_ANY, wxString::Format("Main Content %d", i + 1)), 0, wxALL, 5);
//         }
//         // ShapeToolbar *shapeToolbar = new ShapeToolbar(mainScrollWindow);

//         // Create a smaller scrollable panel
//         wxImage buttonImage(wxT("image\\stop.png"), wxBITMAP_TYPE_PNG);
//         buttonImage.Rescale(FromDIP(30), FromDIP(30)); // Resize the image if needed
//         wxBitmap buttonBitmap(buttonImage);

//         wxBitmapButton *squareButton = new wxBitmapButton(mainScrollWindow, wxID_ANY, buttonBitmap, wxDefaultPosition);

//         mainSizer->Add(squareButton, 0, wxALL, 5);

//         auto nestedScrollPanel = new wxPanel(mainScrollWindow, wxID_ANY);

//         // wxScrolledWindow *nestedScrollPanel = new wxScrolledWindow(mainScrollWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
//         // nestedScrollPanel->SetScrollRate(5, 5);
//         nestedScrollPanel->SetBackgroundColour(*wxLIGHT_GREY); // Optional: Highlight the nested panel

//         // ShapeToolbar *shapeToolbar = new ShapeToolbar(nestedScrollPanel);
//         // Sizer for the nested scroll panel
//         wxBoxSizer *nestedSizer = new wxBoxSizer(wxVERTICAL);

//         // ShapeToolbar *shapeToolbar = new ShapeToolbar(nestedScrollPanel);

//         // nestedSizer->Add(shapeToolbar, 0, wxALL, 10);

//         // ShapeToolbar *s;
//         // Add content to the nested panel
//         for (int i = 0; i < 10; ++i)
//         {
//             nestedSizer->Add(new wxButton(nestedScrollPanel, wxID_ANY, wxString::Format("Button %d", i + 1)), 0, wxALL, 5);
//         }

//         nestedScrollPanel->SetSizer(nestedSizer);
//         nestedScrollPanel->FitInside();                  // Ensure it fits its content
//         nestedScrollPanel->SetMinSize(wxSize(500, 100)); // Set minimum size for visibility

//         // Add the nested scroll panel to the main window
//         mainSizer->Add(nestedScrollPanel, 0, wxALL, 5);

//         // ShapeToolbar *shapeToolbar = new ShapeToolbar(nestedScrollPanel);

//         // nestedSizer->Add(shapeToolbar, 0, wxALL, 10);

//         // Add more static content to the main window
//         for (int i = 0; i < 5; ++i)
//         {
//             mainSizer->Add(new wxStaticText(mainScrollWindow, wxID_ANY, wxString::Format("More Main Content %d", i + 1)), 0, wxALL, 5);
//         }

//         mainScrollWindow->SetSizer(mainSizer);
//         mainScrollWindow->FitInside(); // Ensure it fits its content
//     }
// };

// class MyApp : public wxApp
// {
// public:
//     virtual bool OnInit()
//     {
//         wxInitAllImageHandlers(); // Initialize all handlers for image formats
//         MyFrame *frame = new MyFrame();
//         frame->Show();
//         return true;
//     }
// };

// wxIMPLEMENT_APP(MyApp);
