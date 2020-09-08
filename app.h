#pragma once

#include "wx/wx.h"
#include "main.h"


class app : public wxApp
{

public:

	app();
	~app();
	virtual bool OnInit();

private:

	main* m_frame1 = nullptr;
};
