#include "ImagePreview.hpp"
#include <wx/dcclient.h>
#include <wx/dnd.h>

BEGIN_EVENT_TABLE(ImagePreview,wxPanel)
	EVT_MOTION(ImagePreview::OnMouseMove)
	EVT_PAINT(ImagePreview::OnPaint)
	EVT_SIZE(ImagePreview::OnSize)
END_EVENT_TABLE()

ImagePreview::ImagePreview(wxWindow *parent, wxWindowID id)
	:wxPanel(parent, id, wxDefaultPosition, wxSize(-1, 100)), loaded(false) {Hide();}

void ImagePreview::OnPaint(wxPaintEvent&){
	wxPaintDC dc(this);
	if(!loaded)
		dc.Clear();
	else{
		wxSize panelSize = GetSize();
		wxSize imgSize = resized.GetSize();
		wxSize drawPos = panelSize/2-imgSize/2;
		dc.DrawBitmap(resized, drawPos.GetWidth(), drawPos.GetHeight(), true);
	}
}
void ImagePreview::ResizeImage(){
	wxSize panelSize = GetSize();
	wxSize imgSize = image.GetSize();

	if(panelSize.GetHeight() == 0 || panelSize.GetWidth() == 0)
		return;

	if(panelSize.GetWidth() < imgSize.GetWidth() ||
			panelSize.GetHeight() < imgSize.GetHeight()){
		float width = imgSize.GetWidth();
		float height = imgSize.GetHeight();
		float ratio = std::min(panelSize.GetWidth()/width,
							panelSize.GetHeight()/height);
		resized = wxBitmap(image.Scale(width*ratio, height*ratio));
	}else
		resized = image;
}
void ImagePreview::OnSize(wxSizeEvent&){
	if(loaded)
		ResizeImage();
	Refresh();
}
void ImagePreview::SetImage(const wxString &_path){
	path = _path;
	loaded = image.LoadFile(_path);
	if(loaded){
		ResizeImage();
		Show();
	}
}
void ImagePreview::DeleteImage(){
	loaded = false;
	Hide();
}
void ImagePreview::OnMouseMove(wxMouseEvent &event){
	if(loaded && event.LeftIsDown()){
		wxBitmapDataObject data(image);
		wxDropSource dragSource(this);
		wxFileDataObject files;
		files.AddFile(path);
		dragSource.SetData(files);
		wxDragResult result = dragSource.DoDragDrop(wxDrag_CopyOnly);
	}
}
