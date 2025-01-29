#include "PenSizePanel.h"

PenSizePanel::PenSizePanel(wxWindow *parent, std::function<void(int)> onPenSizeChange)
    : wxPanel(parent, wxID_ANY), onPenSizeChangeCallback(onPenSizeChange)
{
    auto sizer = new wxBoxSizer(wxVERTICAL);

    // Create the slider (min: 1, max: 100)
    m_slider = new wxSlider(this, wxID_ANY, 1, 1, 100, wxDefaultPosition, wxSize(200, -1), wxSL_HORIZONTAL);

    // Bind the slider event to a method
    m_slider->Bind(wxEVT_SCROLL_THUMBTRACK, &PenSizePanel::OnSliderChange, this);

    // Add the slider to the sizer
    sizer->Add(m_slider, 1, wxALL | wxEXPAND, 10);

    // auto label = new wxStaticText(this, wxID_ANY, "Adjust Pen Size:");
    // sizeSlider = new wxSlider(this, wxID_ANY, 5, 1, 50, wxDefaultPosition, wxSize(200, -1));
    // sizeSlider = new wxSlider(this, wxID_ANY, 5, 1, 50, wxDefaultPosition, wxSize(-1, 30));
    // sizeSlider->SetMinSize(wxSize(200, -1)); // Adjust size to fit better.

    // sizer->Add(label, 0, wxALL, 5);
    // sizer->Add(sizeSlider, 0, wxALL | wxEXPAND, 5);

    // Bind(wxEVT_SLIDER, &PenSizePanel::OnSliderUpdate, this);

    SetSizerAndFit(sizer);
}

void PenSizePanel::OnSliderChange(wxCommandEvent &event)
{
    int size = m_slider->GetValue();
    // onPenSizeChangeCallback(size);
    //  m_penSizeCallback(size);

    // int penSize = sizeSlider->GetValue();
    if (onPenSizeChangeCallback)
    {
        onPenSizeChangeCallback(size);
    }

    // m_slider->SetValue(size + 1); // Example: increment slider value
    // this->Layout();
}
