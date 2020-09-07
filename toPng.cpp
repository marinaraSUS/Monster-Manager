// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
        #include "wx/wx.h"
#endif


class B2CFrame : public wxFrame
{
public:
       
        B2CFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = "frame");

        void onFileOpen(wxCommandEvent& arg_event);
        void onFileSave(wxCommandEvent& arg_event);
        void onFileExit(wxCommandEvent& arg_event);
       
        DECLARE_EVENT_TABLE()
};

class Bin2CGUIApp : public wxApp
{

public:

    virtual bool OnInit();

        void bin2c();

        wxString m_filename;

        B2CFrame* m_frame;
        wxTextCtrl* m_textctrl;
        wxString *validFilename;
        wxMenu* filemenu;

};


IMPLEMENT_APP(Bin2CGUIApp)

enum
{
        B2C_FILE_OPEN = 1000,
        B2C_FILE_SAVE,
        B2C_FILE_EXIT
};


/* this is executed upon startup, like 'main()' in non-wxWindows programs */
bool Bin2CGUIApp::OnInit()
{
        m_filename = _T("");

        m_frame = new B2CFrame((wxFrame*) NULL, -1, _T("Binary to C"), wxDefaultPosition, wxSize(800,600));
       
        m_textctrl = new wxTextCtrl(m_frame, -1, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH2);
        wxFont textfont(10, wxDEFAULT, wxNORMAL, wxNORMAL, FALSE, _T("Courier New"));
        wxTextAttr textattr(*wxBLACK, *wxWHITE, textfont);
        m_textctrl->SetDefaultStyle(textattr);

        wxMenuBar* menubar = new wxMenuBar();
        filemenu = new wxMenu();
        filemenu->Append(B2C_FILE_OPEN, _T("&Open..."));
        filemenu->Append(B2C_FILE_SAVE, _T("&Save..."));
        filemenu->AppendSeparator();
        filemenu->Append(B2C_FILE_EXIT, _T("&Exit"));
       
        filemenu->Enable(B2C_FILE_SAVE, false);

        menubar->Append(filemenu, _T("&File"));

        m_frame->SetMenuBar(menubar);

    m_frame->Show(TRUE);
    SetTopWindow(m_frame);


    return true;
}



void Bin2CGUIApp::bin2c()
{
        /* This "algorithm" is based on Bart Trzynadlowski bin2c program written on June 11, 2001 */
        m_textctrl->Clear();
        FILE            *fp;
        long            size, i;
        unsigned char   data;

        if((fp = fopen(m_filename, "rb")) == NULL)
        {
                wxMessageBox(wxString(_T("Unable to open file")));
                return;
        }

        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        rewind(fp);

        validFilename = new wxString(m_filename.AfterLast('\\').Lower());
        validFilename->Replace(_T(">"), _T("_"));
        validFilename->Replace(_T("<"), _T("_"));
        validFilename->Replace(_T("="), _T("_"));
        validFilename->Replace(_T("!"), _T("_"));
        validFilename->Replace(_T("&"), _T("_"));
        validFilename->Replace(_T("|"), _T("_"));
        validFilename->Replace(_T("-"), _T("_"));
        validFilename->Replace(_T("["), _T("_"));
        validFilename->Replace(_T("]"), _T("_"));
        validFilename->Replace(_T("^"), _T("_"));
        validFilename->Replace(_T(":"), _T("_"));
        validFilename->Replace(_T(","), _T("_"));
        validFilename->Replace(_T("{"), _T("_"));
        validFilename->Replace(_T("}"), _T("_"));
        validFilename->Replace(_T("."), _T("_"));
        validFilename->Replace(_T("*"), _T("_"));
        validFilename->Replace(_T("("), _T("_"));
        validFilename->Replace(_T(")"), _T("_"));
        validFilename->Replace(_T("+"), _T("_"));
        validFilename->Replace(_T("%"), _T("_"));
        validFilename->Replace(_T("#"), _T("_"));
        validFilename->Replace(_T("?"), _T("_"));
        validFilename->Replace(_T("/"), _T("_"));
        validFilename->Replace(_T("*"), _T("_"));
        validFilename->Replace(_T("~"), _T("_"));
        validFilename->Replace(_T("\\"), _T("_"));
        validFilename->Replace(_T("."), _T("_"));
        validFilename->Replace(_T(" "), _T("_"));
        m_textctrl->AppendText(_T("unsigned char "));
        m_textctrl->AppendText(_T(*validFilename));
        m_textctrl->AppendText(_T("[] ="));
        m_textctrl->AppendText(_T("{"));
        m_textctrl->AppendText(_T("\n"));
                       
        i = 0;
        while (size--)
        {
                fread(&data, sizeof(unsigned char), 1, fp);

                if (!i)
                        m_textctrl->AppendText("\t");
                if (size == 0)
                        m_textctrl->AppendText(wxString::Format("0x%02X\n};\n", data));
                else
                        m_textctrl->AppendText(wxString::Format("0x%02X,", data));
                i++;
                if (i == 14)
                {
                        m_textctrl->AppendText("\n");
                        i = 0;
                }
        }

        fclose(fp);
}


BEGIN_EVENT_TABLE(B2CFrame, wxFrame)
        EVT_MENU(B2C_FILE_OPEN, B2CFrame::onFileOpen)
        EVT_MENU(B2C_FILE_SAVE, B2CFrame::onFileSave)
        EVT_MENU(B2C_FILE_EXIT, B2CFrame::onFileExit)
END_EVENT_TABLE()

B2CFrame::B2CFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxFrame(parent, id, title, pos, size, style, name)
{

}

void B2CFrame::onFileOpen(wxCommandEvent& WXUNUSED(event)t)
{
        wxFileDialog fileDialog(this, _T("Select a file"), _T(""), _T(""), _T("*.*"), wxOPEN);

        if(fileDialog.ShowModal() == wxID_OK)
        {
                wxGetApp().filemenu->Enable(B2C_FILE_SAVE, true);
                wxGetApp().m_filename = fileDialog.GetPath();
                wxGetApp().bin2c();
        }
}

void B2CFrame::onFileSave(wxCommandEvent& WXUNUSED(event))
{
        wxFileDialog fileDialog(this, _T("Save in a file"), _T(""), (*wxGetApp().validFilename)+".h" ,  "header files (*.h)|*.h|c code files (*.c)|*.c|c++ code files (*.cpp)|*.cpp", wxSAVE);

        if(fileDialog.ShowModal() == wxID_OK)
        {
            wxGetApp().m_textctrl->SaveFile(fileDialog.GetPath()); 
        }
}
               
void B2CFrame::onFileExit(wxCommandEvent& WXUNUSED(event))
{
     Destroy();
}