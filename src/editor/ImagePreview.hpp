#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>

class ImagePreview : public wxPanel{
public:
	ImagePreview(wxWindow *parent, wxWindowID id=wxID_ANY);

	void SetImage(const wxString &path);
	void DeleteImage();
private:
	void OnMouseMove(wxMouseEvent&);
	void OnPaint(wxPaintEvent&);
	void OnSize(wxSizeEvent&);
	void ResizeImage();

	wxImage image;
	wxBitmap resized;
	wxString path;
	bool loaded;

	wxDECLARE_EVENT_TABLE();
};
