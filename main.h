#pragma once
#include "wx/wx.h"




class main :public wxMDIParentFrame
{
public:

	main();
	~main();

public:

	wxButton* btn = nullptr;
	wxButton* btn1 = nullptr;
	
	wxDECLARE_EVENT_TABLE();

};
