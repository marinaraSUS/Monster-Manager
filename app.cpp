#include "app.h"



wxIMPLEMENT_APP(app);

app::app() {


}

app::~app() {

}


bool app::OnInit() {

	m_frame1 = new main();
	m_frame1->Show();

	return true;
}