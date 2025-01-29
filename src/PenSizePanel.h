#ifndef PEN_SIZE_PANEL_H
#define PEN_SIZE_PANEL_H

#include <wx/wx.h>
#include <wx/slider.h>

class PenSizePanel : public wxPanel
{
public:
    PenSizePanel(wxWindow *parent, std::function<void(int)> onPenSizeChange);

private:
    wxSlider *sizeSlider, *m_slider;

    std::function<void(int)> onPenSizeChangeCallback;

    // void OnSliderUpdate(wxCommandEvent &event);
    std::function<void(int)> m_penSizeCallback;

    void OnSliderChange(wxCommandEvent &event);
};

#endif // PEN_SIZE_PANEL_H
