/*
The contents of this file are subject to the Initial Developer's Public
License Version 1.0 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License here:
http://www.flamerobin.org/license.html.

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

The Original Code is FlameRobin (TM).

The Initial Developer of the Original Code is Milan Babuskov.

Portions created by the original developer
are Copyright (C) 2004 Milan Babuskov.

All Rights Reserved.

$Id$

Contributor(s): Michael Hieke
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "DatabaseRegistrationDialog.h"
#include "frutils.h"
#include "styleguide.h"
#include "ugly.h"

#if wxCHECK_VERSION(2, 5, 3)
#include "wx/gbsizer.h"
#endif
//-----------------------------------------------------------------------------
DatabaseRegistrationDialog::DatabaseRegistrationDialog(wxWindow* parent, int id, const wxString& title, bool createDB, const wxPoint& pos, const wxSize& size, long style):
    BaseDialog(parent, id, title, pos, size, style)
{
    createM = createDB;
    label_dbpath = new wxStaticText(this, -1, _("Database path:"));
    text_ctrl_dbpath = new wxTextCtrl(this, ID_textcontrol_dbpath, wxT(""));
    button_browse = new wxButton(this, ID_button_browse, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    label_username = new wxStaticText(this, -1, _("Username:"));
    text_ctrl_username = new wxTextCtrl(this, ID_textcontrol_username, wxT("SYSDBA"));
    label_password = new wxStaticText(this, -1, _("Password:"));
    text_ctrl_password = new wxTextCtrl(this, -1, wxT("masterkey"), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    text_ctrl_password->SetToolTip(_("Leave empty if you wish to be prompted for password every time"));
    label_charset = new wxStaticText(this, -1, _("Charset:"));
    const wxString charset_choices[] = {
        wxT("NONE"),
        wxT("ASCII"),
        wxT("BIG_5"),
        wxT("CYRL"),
        wxT("DOS437"),
        wxT("DOS850"),
        wxT("DOS852"),
        wxT("DOS857"),
        wxT("DOS860"),
        wxT("DOS861"),
        wxT("DOS863"),
        wxT("DOS865"),
        wxT("EUCJ_0208"),
        wxT("GB_2312"),
        wxT("ISO8859_1"),
        wxT("ISO8859_2"),
        wxT("KSC_5601"),
        wxT("NEXT"),
        wxT("OCTETS"),
        wxT("SJIS_0208"),
        wxT("UNICODE_FSS"),
        wxT("WIN1250"),
        wxT("WIN1251"),
        wxT("WIN1252"),
        wxT("WIN1253"),
        wxT("WIN1254")
    };
    combo_box_charset = new wxComboBox(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize,
        sizeof(charset_choices) / sizeof(wxString), charset_choices, wxCB_DROPDOWN|wxCB_READONLY);
    label_role = new wxStaticText(this, -1, _("Role:"));
    text_ctrl_role = new wxTextCtrl(this, -1, wxT(""));

    if (createM)
    {
        label_pagesize = new wxStaticText(this, -1, _("Page size:"));
        const wxString pagesize_choices[] = {
            wxT("1024"),
            wxT("2048"),
            wxT("4096"),
            wxT("8192"),
            wxT("16384")
        };
        combo_box_pagesize = new wxComboBox(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize,
            sizeof(pagesize_choices) / sizeof(wxString), pagesize_choices, wxCB_DROPDOWN|wxCB_READONLY);
        label_dialect = new wxStaticText(this, -1, _("SQL Dialect:"));
        const wxString dialect_choices[] = {
            wxT("1"),
            wxT("3")
        };
        combo_box_dialect = new wxComboBox(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize,
            sizeof(dialect_choices) / sizeof(wxString), dialect_choices, wxCB_DROPDOWN|wxCB_READONLY);
    }

    button_ok = new wxButton(this, ID_button_ok, (createM ? _("Create") : _("Save")));
    button_cancel = new wxButton(this, ID_button_cancel, _("Cancel"));

    set_properties();
    do_layout();
    updateButtons();
}
//-----------------------------------------------------------------------------
//! implementation details
void DatabaseRegistrationDialog::do_layout()
{
#if wxCHECK_VERSION(2, 5, 3)
    // create sizer for controls
    wxGridBagSizer* sizerControls = new wxGridBagSizer(styleguide().getRelatedControlMargin(wxVERTICAL),
        styleguide().getControlLabelMargin());

    sizerControls->Add(label_dbpath, wxGBPosition(0, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
    wxBoxSizer* sizer_r1c1_3 = new wxBoxSizer(wxHORIZONTAL);
    sizer_r1c1_3->Add(text_ctrl_dbpath, 1, wxALIGN_CENTER_VERTICAL);
    sizer_r1c1_3->Add(button_browse, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, styleguide().getBrowseButtonMargin());
    sizerControls->Add(sizer_r1c1_3, wxGBPosition(0, 1), wxGBSpan(1, 3), wxEXPAND);

    int dx = styleguide().getUnrelatedControlMargin(wxHORIZONTAL) - styleguide().getControlLabelMargin();
    if (dx < 0)
        dx = 0;

    sizerControls->Add(label_username, wxGBPosition(1, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
    sizerControls->Add(text_ctrl_username, wxGBPosition(1, 1), wxDefaultSpan, wxALIGN_CENTER_VERTICAL|wxEXPAND);
    sizerControls->Add(label_password, wxGBPosition(1, 2), wxDefaultSpan, wxLEFT|wxALIGN_CENTER_VERTICAL, dx);
    sizerControls->Add(text_ctrl_password, wxGBPosition(1, 3), wxDefaultSpan, wxALIGN_CENTER_VERTICAL|wxEXPAND);

    sizerControls->Add(label_charset, wxGBPosition(2, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
    sizerControls->Add(combo_box_charset, wxGBPosition(2, 1), wxDefaultSpan, wxALIGN_CENTER_VERTICAL|wxEXPAND);
    sizerControls->Add(label_role, wxGBPosition(2, 2), wxDefaultSpan, wxLEFT|wxALIGN_CENTER_VERTICAL, dx);
    sizerControls->Add(text_ctrl_role, wxGBPosition(2, 3), wxDefaultSpan, wxALIGN_CENTER_VERTICAL|wxEXPAND);

    if (createM)
    {
        sizerControls->Add(label_pagesize, wxGBPosition(3, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
        sizerControls->Add(combo_box_pagesize, wxGBPosition(3, 1), wxDefaultSpan, wxALIGN_CENTER_VERTICAL|wxEXPAND);
        sizerControls->Add(label_dialect, wxGBPosition(3, 2), wxDefaultSpan, wxLEFT|wxALIGN_CENTER_VERTICAL, dx);
        sizerControls->Add(combo_box_dialect, wxGBPosition(3, 3), wxDefaultSpan, wxALIGN_CENTER_VERTICAL|wxEXPAND);
    }

    sizerControls->AddGrowableCol(1);
    sizerControls->AddGrowableCol(3);
#else
    // make all labels and controls have the same width to simulate a grid
    std::list<wxWindow*> controls;
    controls.push_back(label_dbpath);
    controls.push_back(label_username);
    controls.push_back(label_charset);
    if (createM)
        controls.push_back(label_pagesize);
    adjustControlsMinWidth(controls);
    controls.clear();

    controls.push_back(label_password);
    controls.push_back(label_role);
    if (createM)
        controls.push_back(label_dialect);
    adjustControlsMinWidth(controls);
    controls.clear();

    controls.push_back(text_ctrl_username);
    controls.push_back(text_ctrl_password);
    controls.push_back(combo_box_charset);
    controls.push_back(text_ctrl_role);
    if (createM)
    {
        controls.push_back(combo_box_pagesize);
        controls.push_back(combo_box_dialect);
    }
    adjustControlsMinWidth(controls);
    controls.clear();

    // create sizers hierarchy for controls
    wxBoxSizer* sizerRow1 = new wxBoxSizer(wxHORIZONTAL);
    sizerRow1->Add(label_dbpath, 0, wxALIGN_CENTER_VERTICAL);
    sizerRow1->Add(styleguide().getControlLabelMargin(), 0);
    sizerRow1->Add(text_ctrl_dbpath, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
    sizerRow1->Add(styleguide().getBrowseButtonMargin(), 0);
    sizerRow1->Add(button_browse, 0, wxALIGN_CENTER_VERTICAL);

    wxBoxSizer* sizerRow2 = new wxBoxSizer(wxHORIZONTAL);
    sizerRow2->Add(label_username, 0, wxALIGN_CENTER_VERTICAL);
    sizerRow2->Add(styleguide().getControlLabelMargin(), 0);
    sizerRow2->Add(text_ctrl_username, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
    sizerRow2->Add(styleguide().getUnrelatedControlMargin(wxHORIZONTAL), 0);
    sizerRow2->Add(label_password, 0, wxALIGN_CENTER_VERTICAL);
    sizerRow2->Add(styleguide().getControlLabelMargin(), 0);
    sizerRow2->Add(text_ctrl_password, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);

    wxBoxSizer* sizerRow3 = new wxBoxSizer(wxHORIZONTAL);
    sizerRow3->Add(label_charset, 0, wxALIGN_CENTER_VERTICAL);
    sizerRow3->Add(styleguide().getControlLabelMargin(), 0);
    sizerRow3->Add(combo_box_charset, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
    sizerRow3->Add(styleguide().getUnrelatedControlMargin(wxHORIZONTAL), 0);
    sizerRow3->Add(label_role, 0, wxALIGN_CENTER_VERTICAL);
    sizerRow3->Add(styleguide().getControlLabelMargin(), 0);
    sizerRow3->Add(text_ctrl_role, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);

    wxBoxSizer* sizerRow4 = 0;
    if (createM)
    {
        sizerRow4 = new wxBoxSizer(wxHORIZONTAL);
        sizerRow4->Add(label_pagesize, 0, wxALIGN_CENTER_VERTICAL);
        sizerRow4->Add(styleguide().getControlLabelMargin(), 0);
        sizerRow4->Add(combo_box_pagesize, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
        sizerRow4->Add(styleguide().getUnrelatedControlMargin(wxHORIZONTAL), 0);
        sizerRow4->Add(label_dialect, 0, wxALIGN_CENTER_VERTICAL);
        sizerRow4->Add(styleguide().getControlLabelMargin(), 0);
        sizerRow4->Add(combo_box_dialect, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL);
    }

    wxBoxSizer* sizerControls = new wxBoxSizer(wxVERTICAL);
    sizerControls->Add(sizerRow1, 0, wxEXPAND);
    sizerControls->Add(0, styleguide().getRelatedControlMargin(wxVERTICAL));
    sizerControls->Add(sizerRow2, 0, wxEXPAND);
    sizerControls->Add(0, styleguide().getRelatedControlMargin(wxVERTICAL));
    sizerControls->Add(sizerRow3, 0, wxEXPAND);
    if (createM)
    {
        sizerControls->Add(0, styleguide().getRelatedControlMargin(wxVERTICAL));
        sizerControls->Add(sizerRow4, 0, wxEXPAND);
    }
#endif

    // create sizer for buttons -> styleguide class will align it correctly
    wxSizer* sizerButtons = styleguide().createButtonSizer(button_ok, button_cancel);

    // use method in base class to set everything up
    layoutSizers(sizerControls, sizerButtons);
}
//-----------------------------------------------------------------------------
const std::string DatabaseRegistrationDialog::getName() const
{
    return "DatabaseRegistrationInfoFrame";
}
//-----------------------------------------------------------------------------
void DatabaseRegistrationDialog::set_properties()
{
    int wh = text_ctrl_dbpath->GetMinHeight();
    button_browse->SetSize(wh, wh);
    if (createM)
    {
        combo_box_pagesize->SetSelection(2);
        combo_box_dialect->SetSelection(1);
    }
    button_ok->SetDefault();
}
//-----------------------------------------------------------------------------
void DatabaseRegistrationDialog::setDatabase(YDatabase *db)
{
    databaseM = db;
    /* this could be reactivated if there is a dialog with "Don't show me again"
    if (databaseM->isConnected())
    ::wxMessageBox(_("Properties of connected database cannot be changed."), _("Warning"), wxOK |wxICON_INFORMATION );
    */
    text_ctrl_dbpath->SetValue(std2wx(databaseM->getPath()));
    text_ctrl_username->SetValue(std2wx(databaseM->getUsername()));
    text_ctrl_password->SetValue(std2wx(databaseM->getPassword()));
    text_ctrl_role->SetValue(std2wx(databaseM->getRole()));
    combo_box_charset->SetSelection(combo_box_charset->FindString(std2wx(databaseM->getCharset())));
    if (combo_box_charset->GetSelection() < 0)
        combo_box_charset->SetSelection(combo_box_charset->FindString(wxT("NONE")));

    // enable controls depending on operation and database connection status
    // use SetEditable() for edit controls to allow copying text to clipboard
    bool isConnected = databaseM->isConnected();
    text_ctrl_dbpath->SetEditable(!isConnected);
    button_browse->Enable(!isConnected);
    text_ctrl_username->SetEditable(!isConnected);
    text_ctrl_password->SetEditable(!isConnected);
    combo_box_charset->Enable(!isConnected);
    text_ctrl_role->SetEditable(!isConnected);
    button_ok->Enable(!isConnected);
    if (isConnected)
    {
        button_cancel->SetLabel(_("Close"));
        button_cancel->SetDefault();
    };
    updateButtons();
}
//-----------------------------------------------------------------------------
void DatabaseRegistrationDialog::setServer(YServer *s)
{
    serverM = s;
}
//-----------------------------------------------------------------------------
void DatabaseRegistrationDialog::updateButtons()
{
    if (button_ok->IsShown())
    {
        button_ok->Enable(text_ctrl_dbpath->IsEditable()
            && !text_ctrl_dbpath->GetValue().IsEmpty()
            && !text_ctrl_username->GetValue().IsEmpty());
    }
}
//-----------------------------------------------------------------------------
//! event handling
BEGIN_EVENT_TABLE(DatabaseRegistrationDialog, BaseDialog)
    EVT_BUTTON(DatabaseRegistrationDialog::ID_button_browse, DatabaseRegistrationDialog::OnBrowseButtonClick)
    EVT_BUTTON(DatabaseRegistrationDialog::ID_button_ok, DatabaseRegistrationDialog::OnOkButtonClick)
    EVT_TEXT(DatabaseRegistrationDialog::ID_textcontrol_dbpath, DatabaseRegistrationDialog::OnSettingsChange)
    EVT_TEXT(DatabaseRegistrationDialog::ID_textcontrol_username, DatabaseRegistrationDialog::OnSettingsChange)
END_EVENT_TABLE()
//-----------------------------------------------------------------------------
void DatabaseRegistrationDialog::OnBrowseButtonClick(wxCommandEvent& WXUNUSED(event))
{
    wxString path = ::wxFileSelector(_("Select database file"), wxT(""), wxT(""), wxT(""),
        _("Firebird database files (*.fdb, *.gdb)|*.fdb;*.gdb|All files (*.*)|*.*"), 0, this);
    if (!path.empty())
        text_ctrl_dbpath->SetValue(path);
}
//-----------------------------------------------------------------------------
void DatabaseRegistrationDialog::OnOkButtonClick(wxCommandEvent& WXUNUSED(event))
{
    wxBusyCursor wait;
    databaseM->setPath(wx2std(text_ctrl_dbpath->GetValue()));
    databaseM->setUsername(wx2std(text_ctrl_username->GetValue()));
    databaseM->setPassword(wx2std(text_ctrl_password->GetValue()));
    databaseM->setCharset(wx2std(combo_box_charset->GetValue()));
    databaseM->setRole(wx2std(text_ctrl_role->GetValue()));

    try
    {
        if (createM)	// create new database
        {
            int dialect = 1 + (2 * combo_box_dialect->GetSelection());	// 0/1 => 1/3
            wxString ps(wxT("PAGE_SIZE "));
            ps += combo_box_pagesize->GetStringSelection();
            serverM->createDatabase(databaseM, wx2std(ps), dialect);
        }
        EndModal(wxID_OK);
    }
    catch (IBPP::Exception &e)
    {
        wxMessageBox(std2wx(e.ErrorMessage()), _("Error"), wxOK|wxICON_ERROR);
    }
    catch (...)
    {
        wxMessageBox(_("SYSTEM ERROR!\n"), _("Error"), wxOK|wxICON_ERROR);
    }
}
//-----------------------------------------------------------------------------
void DatabaseRegistrationDialog::OnSettingsChange(wxCommandEvent& WXUNUSED(event))
{
    if (IsShown())
        updateButtons();
}
//-----------------------------------------------------------------------------
