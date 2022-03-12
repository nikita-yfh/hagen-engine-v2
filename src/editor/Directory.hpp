#pragma once
#include <wx/string.h>

class Directory{
public:
	const wxString &GetPath() const{
		return str;
	}
	wxString ConvertToLocalPath(const wxString &apath) const{
		if(apath.StartsWith(str))
			return apath.Mid(str.Length()+1);
		return wxEmptyString;
	}
	wxString ConvertToAbsolutePath(const wxString &lpath) const{
		return str+"/"+lpath;
	}
	void operator=(const wxString &_str){
		str = _str;
	}
	void operator=(const Directory &other){
		str = other.str;
	}
private:
	wxString str;
};

