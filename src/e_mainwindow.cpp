/*
 *  Enyo Launcher - a launcher for Doom games 
 *  Copyright 2005-2021 Stephen D. Cofer
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <https://www.gnu.org/licenses/>
*/
#include "e_mainwindow.h"
#include "ui_e_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <cstdlib>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QCursor>
#include <QDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QStringList>
#include <QDir>
#include <QListWidget>
#include <QListWidgetItem>

#ifndef WIN32
#include <libgen.h>
#else
#include <stdlib.h>
#endif
// GLOBAL VALUES (globals are bad?  pfft!)

struct game_settings
{
    QString game_name;
    QString game_wad;
    QStringList game_pwad;
    int engine_id;
    QString game_advanced_settings;
};

game_settings enyo_games[MAX_GAMES];

struct game_engines
{
    QString engine_name;
    QString engine_binary_path;
};

game_engines enyo_engines [MAX_ENGINES];

//these point to the NEXT engine to be created, thus no engines = 0, 1 = 1, etc
int game_pointer = 0;
int engine_pointer = 0;

int last_game_selected = 0;
int last_engine_selected = 0;

bool show_output = true;
bool tabs_top = false;
bool exit_after_run = false;
bool show_game_numbers = true;

QString last_path;


// About window
class about_window : public QDialog
{
	public:
		about_window(QWidget *parent=0);
	private:
		QLabel *enyo_logo;
		QVBoxLayout *about_vlayout;
		QPlainTextEdit *ab_text;
};
		
about_window::about_window(QWidget *parent)
{
	setFocusPolicy(Qt::StrongFocus);
	setFocus();

	about_vlayout = new QVBoxLayout;

	enyo_logo = new QLabel();
    enyo_logo->setPixmap( QPixmap (":/share/enyo.png"));
    enyo_logo->setScaledContents(true);
	about_vlayout->addWidget (enyo_logo);
	ab_text = new QPlainTextEdit;
	ab_text->clear();
	ab_text->setReadOnly (true);
	QString about_text = "Enyo Launcher\nVersion ";
	about_text.append(ENYO_VERSION);
	about_text.append (" (\"");
	about_text.append(ENYO_TAGLINE);
	about_text.append("\")\n\n");
	about_text.append(ENYO_COPYRIGHT);
	about_text.append("\n\n");
	about_text.append(GPL_TEXT); // QString's overloaded + doesn't support DEFINEd string constants, apparently
	ab_text->appendPlainText(about_text);
	ab_text->moveCursor(QTextCursor::Start);
	about_vlayout->addWidget (ab_text);
	setLayout (about_vlayout);
	setWindowTitle ("About Enyo Launcher");
	setFixedSize (640, 480);
	setParent (parent);
}

			

// SETTINGS

void e_mainwindow::load_settings ()
{
    QSettings enyo_settings (QSettings::NativeFormat, QSettings::UserScope, "enyo-launcher", "enyo-launcher");
    if (enyo_settings.allKeys().size() == 0)
    {
        load_legacy_settings();
        return;
    }
    int g_size = enyo_settings.beginReadArray("Games");
    if (g_size == 0)
    {
	    for (int i=0; i<4; i++) enyo_games[i].game_name = default_games.at(i);
        game_pointer = 4;
    }
    else
    {
        for (int j=0; j<g_size; j++)
        {
            enyo_settings.setArrayIndex(j);
            enyo_games[game_pointer].game_name = enyo_settings.value ("name").toString();
            enyo_games[game_pointer].game_wad = enyo_settings.value ("iwad", "").toString();
            enyo_games[game_pointer].game_pwad = enyo_settings.value ("pwads", "").toStringList();
            enyo_games[game_pointer].engine_id = enyo_settings.value ("engine", 0).toInt();
            enyo_games[game_pointer].game_advanced_settings = enyo_settings.value ("advanced", "").toString();
            game_pointer++;
        }
    }
    enyo_settings.endArray();
    int e_size = enyo_settings.beginReadArray ("Engines");
    if (e_size == 0)
    {
    for (int i=0; i<3; i++)
        {
            enyo_engines[i].engine_name = engine_names.at(i);
            enyo_engines[i].engine_binary_path = engine_paths.at(i);
        }
        engine_pointer = 3;
    }
    else
    {
        for (int j=0; j< e_size ; j++)
        {
            enyo_settings.setArrayIndex(j);
            enyo_engines[engine_pointer].engine_name = enyo_settings.value ("name").toString();
            enyo_engines[engine_pointer].engine_binary_path = enyo_settings.value ("binarypath", "").toString();
            engine_pointer++;
        }
    }
    enyo_settings.endArray();
    show_output = enyo_settings.value ("showoutput", "true").toBool();
    tabs_top = enyo_settings.value ("tabsontop", "false").toBool();
    exit_after_run = enyo_settings.value ("exitafterrun", "false").toBool();
    show_game_numbers = enyo_settings.value ("gamenumbers", "true").toBool();
    last_game_selected = enyo_settings.value("lastgame", "0").toInt();
    last_engine_selected = enyo_settings.value("lastengine", "0").toInt();
    last_path = enyo_settings.value("lastpath", QDir::homePath()).toString();
    restoreGeometry(enyo_settings.value("geometry").toByteArray());
    restoreState(enyo_settings.value("windowState").toByteArray());

}
void e_mainwindow::load_legacy_settings ()
{
    QSettings enyo_settings (QSettings::NativeFormat, QSettings::UserScope, "enyo-doom", "enyo-doom");

    int g_size = enyo_settings.beginReadArray("Games");
    if (g_size == 0)
    {
	    for (int i=0; i<4; i++) enyo_games[i].game_name = default_games.at(i);
        game_pointer = 4;
    }
    else
    {
        for (int j=0; j<g_size; j++)
        {
            enyo_settings.setArrayIndex(j);
            enyo_games[game_pointer].game_name = enyo_settings.value ("name").toString();
            enyo_games[game_pointer].game_wad = enyo_settings.value ("iwad", "").toString();
            enyo_games[game_pointer].game_pwad = enyo_settings.value ("pwads", "").toStringList();
            enyo_games[game_pointer].engine_id = enyo_settings.value ("engine", 0).toInt();
            enyo_games[game_pointer].game_advanced_settings = enyo_settings.value ("advanced", "").toString();
            game_pointer++;
        }
    }
    enyo_settings.endArray();
    int e_size = enyo_settings.beginReadArray ("Engines");
    if (e_size == 0)
    {
    for (int i=0; i<3; i++)
        {
            enyo_engines[i].engine_name = engine_names.at(i);
            enyo_engines[i].engine_binary_path = engine_paths.at(i);
        }
        engine_pointer = 3;
    }
    else
    {
        for (int j=0; j< e_size ; j++)
        {
            enyo_settings.setArrayIndex(j);
            enyo_engines[engine_pointer].engine_name = enyo_settings.value ("name").toString();
            enyo_engines[engine_pointer].engine_binary_path = enyo_settings.value ("binarypath", "").toString();
            engine_pointer++;
        }
    }
    enyo_settings.endArray();
    show_output = enyo_settings.value ("showoutput", "true").toBool();
    tabs_top = enyo_settings.value ("tabsontop", "false").toBool();
    exit_after_run = enyo_settings.value ("exitafterrun", "false").toBool();
    show_game_numbers = enyo_settings.value ("gamenumbers", "true").toBool();
    last_game_selected = enyo_settings.value("lastgame", "0").toInt();
    last_engine_selected = enyo_settings.value("lastengine", "0").toInt();
    last_path = enyo_settings.value("lastpath", QDir::homePath()).toString();
    restoreGeometry(enyo_settings.value("geometry").toByteArray());
    restoreState(enyo_settings.value("windowState").toByteArray());

}


void e_mainwindow::save_settings ()
{
    QSettings enyo_settings (QSettings::NativeFormat, QSettings::UserScope, "enyo-launcher", "enyo-launcher");
    enyo_settings.beginWriteArray ("Games");
    for (int i=0; i<game_pointer; i++)
    {
        enyo_settings.setArrayIndex(i);
        enyo_settings.setValue("name", enyo_games[i].game_name);
        enyo_settings.setValue ("iwad", enyo_games[i].game_wad);
        enyo_settings.setValue ("pwads", enyo_games[i].game_pwad);
        enyo_settings.setValue ("engine", enyo_games[i].engine_id);
        enyo_settings.setValue ("advanced", enyo_games[i].game_advanced_settings);
    }
    enyo_settings.endArray();
    enyo_settings.beginWriteArray("Engines");
    for (int i=0; i<engine_pointer; i++)
    {
        enyo_settings.setArrayIndex(i);
        enyo_settings.setValue("name", enyo_engines[i].engine_name);
        enyo_settings.setValue("binarypath", enyo_engines[i].engine_binary_path);
    }
    enyo_settings.endArray();
    enyo_settings.setValue("showoutput", show_output);
    enyo_settings.setValue("tabsontop", tabs_top);
    enyo_settings.setValue("exitafterrun", exit_after_run);
    enyo_settings.setValue("gamenumbers", show_game_numbers);
    enyo_settings.setValue("lastgame", last_game_selected);
    enyo_settings.setValue("lastengine", last_engine_selected);
    enyo_settings.setValue ("lastpath", last_path);
    enyo_settings.setValue("geometry", saveGeometry());
    enyo_settings.setValue("windowState", saveState());
}

// MAIN WINDOW ONLOAD SETUP
e_mainwindow::e_mainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::e_mainwindow)
{
    QString u_title;
    ui->setupUi(this);
    setWindowIcon(QPixmap(":/share/enyo_icon.png"));
    u_title = "Enyo Launcher ";
    u_title.append (ENYO_VERSION);
    setWindowTitle(u_title);
    this->load_settings();
    ui->line_main_wad->setText (enyo_games[last_game_selected].game_wad);
    for (int i=0; i<enyo_games[last_game_selected].game_pwad.count(); i++)
    {
        ui->list_pwads->addItem(enyo_games[last_game_selected].game_pwad.at(i));
    }
    // on load, no pwad list item is selected so disable buttons onload
    ui->button_pwad_up->setEnabled (false);
    ui->button_pwad_down->setEnabled (false);
    ui->line_advanced_settings->setText (enyo_games[last_game_selected].game_advanced_settings);
    for (int i=0; i<engine_pointer; i++)
    {
        ui->cb_engines->addItem(enyo_engines[i].engine_name, Qt::DisplayRole);
        ui->cb_elock->addItem(enyo_engines[i].engine_name, Qt::DisplayRole);
    }
    ui->cb_engines->update();
    ui->cb_elock->update();
    ui->check_elock->setChecked (enyo_games[last_game_selected].engine_id != 0);
    if (enyo_games[last_game_selected].engine_id == 0)
    {
	    ui->cb_elock->setEnabled(false);
    }
    else
    {
	    ui->cb_elock->setEnabled(true);	
	    ui->cb_elock->setCurrentIndex(enyo_games[last_game_selected].engine_id - 1);
    }	   
    repopulate_game_list();
    ui->cb_engines->setCurrentIndex(last_engine_selected);
    ui->line_engine_path->setText (enyo_engines[last_engine_selected].engine_binary_path);
    ui->cb_show_output->setChecked(show_output);	
    if (!show_output)
    {
        ui->tabWidget->removeTab (2);
    }
    ui->cb_tabs_top->setChecked (tabs_top);
    if (tabs_top)
    {
        ui->tabWidget->setTabPosition (QTabWidget::North);
    }
    else
    {
        ui->tabWidget->setTabPosition (QTabWidget::West);
    }
    ui->tabWidget->setCurrentIndex (0);
    ui->cb_exit->setChecked(exit_after_run);
    ui->cb_show_numbers->setChecked(show_game_numbers);
    ui->text_output->setPlainText("Engine output will be shown here after running.");
    move_enabler();
}

e_mainwindow::~e_mainwindow()
{
    this->save_settings();
    delete ui;
}
void e_mainwindow::move_enabler()
{
    ui->btn_move_game_up->setEnabled ((last_game_selected > 0) && (game_pointer > 1));
    ui->btn_move_game_down->setEnabled ((last_game_selected != (game_pointer - 1)) && (game_pointer > 1));
}

// CALLBACKS

void e_mainwindow::on_cb_games_activated(int index)
{
    last_game_selected = index;
    ui->line_main_wad->setText (enyo_games[last_game_selected].game_wad);
    ui->list_pwads->clear();
    for (int i=0; i<enyo_games[last_game_selected].game_pwad.count(); i++)
    {
	ui->list_pwads->addItem(enyo_games[last_game_selected].game_pwad.at(i));
    }
    ui->line_advanced_settings->setText (enyo_games[last_game_selected].game_advanced_settings);
    ui->check_elock->setChecked(enyo_games[last_game_selected].engine_id != 0);
    ui->cb_elock->setEnabled(enyo_games[last_game_selected].engine_id != 0);
    if (enyo_games[last_game_selected].engine_id > 0)
    {
	ui->cb_elock->setCurrentIndex(enyo_games[last_game_selected].engine_id - 1);
    }
    move_enabler();
}

void e_mainwindow::on_line_main_wad_editingFinished()
{
    enyo_games[last_game_selected].game_wad = ui->line_main_wad->text();
}

void e_mainwindow::on_run_engine_clicked()
{
    int engine_index;
    if (enyo_games[last_game_selected].game_wad.isEmpty())
    {
        QMessageBox mb;
        mb.setText ("You have not set a main WAD for this game.");
        mb.exec();
        return;
    }
    QFile fwad (enyo_games[last_game_selected].game_wad);
    if (!fwad.exists())
    {
        QMessageBox mb;
        mb.setText("The path to the game WAD is invalid.  Please check its path.");
        mb.exec();
        return;
    }
    ui->text_output->setPlainText("");
    if (enyo_games[last_game_selected].engine_id == 0)
    {
        engine_index = last_engine_selected;
    }
    else
    {
        engine_index = enyo_games[last_game_selected].engine_id - 1;
    }
    QString doomengine;
    QString full_doomengine = enyo_engines[engine_index].engine_binary_path; 
    if (full_doomengine.isEmpty())
    {
	    QMessageBox mb;
	    mb.setText ("The engine executable has not been set.");
	    mb.exec();
	    return;
    }
    QStringList eng_arguments;
    QStringList engine_parts;
    QProcess *doom_process = new QProcess (this);

    // For executables with multiple parts, such as flatpak executable calls
    
    engine_parts = full_doomengine.split(" ");
    if (engine_parts.size()>0)
    {
	doomengine = engine_parts.at(0).toLocal8Bit().constData();
	for (int e_it = 1; e_it < engine_parts.size(); e_it++)
	{
		eng_arguments << engine_parts.at(e_it).toLocal8Bit().constData();
	}
    }
    else
    {
	    doomengine = full_doomengine;
    }

    eng_arguments << "-iwad" << enyo_games[last_game_selected].game_wad;
    if (enyo_games[last_game_selected].game_pwad.length() > 0)
    {
        eng_arguments << "-file";
        for (int i=0; i<enyo_games[last_game_selected].game_pwad.count(); i++)
        {
            eng_arguments << enyo_games[last_game_selected].game_pwad.at(i);
        }
    }
    if (!enyo_games[last_game_selected].game_advanced_settings.isEmpty())
    {
        QStringList advanced_settings;
        advanced_settings = enyo_games[last_game_selected].game_advanced_settings.split(" ");
        eng_arguments << advanced_settings;
    }
    this->setCursor (Qt::WaitCursor);
    doom_process->start(doomengine, eng_arguments);
    doom_process->waitForFinished();
    this->setCursor (Qt::ArrowCursor);
    if (show_output)
    {
    	QByteArray result = doom_process->readAll();
        QString cmdline;
        ui->text_output->appendPlainText ("# Running:");
    	cmdline += doomengine + " ";
    	for (int i=0;i<eng_arguments.count(); i++)
    	{
        	cmdline += eng_arguments.at(i) + " ";

    	}
    	ui->text_output->appendPlainText(cmdline);
        ui->text_output->appendPlainText("");
        ui->text_output->appendPlainText("# Output:");
    	ui->text_output->appendPlainText(result);
    }
    if (exit_after_run)
    {
	    this->save_settings();
	    delete ui;
	    exit (0);
    }
}

void e_mainwindow::closeEvent(QCloseEvent *event)
{
    save_settings();
    event->accept();
}

void e_mainwindow::on_button_find_wad_clicked()
{
    QFileDialog wad_selector(this);
    QStringList filters;
    #ifdef WIN32
    	char tmp[FILENAME_MAX];
    #endif
    filters << "WAD file (*.wad)"
	   << "Any file (*)";
    wad_selector.setFileMode(QFileDialog::ExistingFile);
    wad_selector.setNameFilters(filters);
    wad_selector.setDirectory(last_path);
    QStringList wad_filename;
    if (wad_selector.exec())
    {
        wad_filename = wad_selector.selectedFiles();
        enyo_games[last_game_selected].game_wad = wad_filename.at(0);
        ui->line_main_wad->setText( wad_filename.at(0));
        QByteArray ba_str = wad_filename.at(0).toLocal8Bit();
        const char* cp_str = ba_str.data();
#ifndef WIN32	
        last_path = dirname((char*)cp_str);
#else
        _splitpath_s (cp_str, NULL, 0, tmp, FILENAME_MAX, NULL, 0, NULL, 0);
        last_path = tmp;
#endif	
    }
}

void e_mainwindow::on_button_pwad_add_clicked()
{
    QFileDialog wad_selector(this);
#ifdef WIN32
    char tmp [FILENAME_MAX];
#endif    
    wad_selector.setFileMode(QFileDialog::ExistingFile);
    wad_selector.setDirectory(last_path);
    QStringList wad_filename;
    if (wad_selector.exec())
    {
        wad_filename = wad_selector.selectedFiles();
        enyo_games[last_game_selected].game_pwad.append(wad_filename.at(0));
        ui->list_pwads->addItem(wad_filename.at(0));
        QByteArray ba_str = wad_filename.at(0).toLocal8Bit();
        const char* cp_str = ba_str.data();
#ifndef WIN32	
        last_path = dirname ((char*)cp_str);
#else
        _splitpath_s (cp_str, NULL, 0, tmp, FILENAME_MAX, NULL, 0, NULL, 0);
        last_path = tmp;
#endif
    }
    ui->list_pwads->setCurrentRow (ui->list_pwads->count() - 1);
    ui->button_pwad_up->setEnabled (ui->list_pwads->count() == 1?false:true);
    ui->button_pwad_down->setEnabled (false);
}

void e_mainwindow::on_button_pwad_remove_clicked()
{
    if (ui->list_pwads->count() == 0)
    {
        return;
    }
    int cur_sel = ui->list_pwads->currentRow();
    QStringList temp_list;
    ui->list_pwads->clear();
    for (int i=0; i<enyo_games[last_game_selected].game_pwad.count();i++)
    {
        if (i != cur_sel)
        {
            ui->list_pwads->addItem(enyo_games[last_game_selected].game_pwad.at(i));
            temp_list.append (enyo_games[last_game_selected].game_pwad.at(i));
        }
    }
    enyo_games[last_game_selected].game_pwad = temp_list;
    ui->button_pwad_up->setEnabled (false);
    ui->button_pwad_down->setEnabled (false);
}

void e_mainwindow::on_button_pwad_clear_clicked()
{
    if (ui->list_pwads->count() == 0) return;
    QMessageBox mbConfirm;
    mbConfirm.setText ("Are you sure you want to clear the add-on list?");
    mbConfirm.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
    mbConfirm.setDefaultButton (QMessageBox::No);
    int rConfirm = mbConfirm.exec();
    if (rConfirm == QMessageBox::No) return;
    enyo_games[last_game_selected].game_pwad.clear();
    ui->list_pwads->clear();
    ui->button_pwad_up->setEnabled (false);
    ui->button_pwad_down->setEnabled (false);
}

void e_mainwindow::on_button_pwad_up_clicked()
{
	QString tmp;
	int cur_sel = ui->list_pwads->currentRow();
	tmp = enyo_games[last_game_selected].game_pwad.at(cur_sel);
	enyo_games[last_game_selected].game_pwad[cur_sel] = enyo_games[last_game_selected].game_pwad.at(cur_sel - 1);
	enyo_games[last_game_selected].game_pwad[cur_sel - 1]= tmp;
	ui->list_pwads->clear();
	for (int i=0; i<enyo_games[last_game_selected].game_pwad.count();i++)
    {
		ui->list_pwads->addItem(enyo_games[last_game_selected].game_pwad.at(i));
    }
	ui->list_pwads->setCurrentRow (cur_sel - 1);
	ui->button_pwad_up->setEnabled ((cur_sel - 1) > 0 ? true:false);
	ui->button_pwad_down->setEnabled (true);
	return;
}

void e_mainwindow::on_button_pwad_down_clicked()
{
	QString tmp;
	int cur_sel = ui->list_pwads->currentRow();
	tmp = enyo_games[last_game_selected].game_pwad.at(cur_sel);
	enyo_games[last_game_selected].game_pwad[cur_sel] = enyo_games[last_game_selected].game_pwad.at(cur_sel + 1);
	enyo_games[last_game_selected].game_pwad[cur_sel + 1] = tmp;
	ui->list_pwads->clear();
	for (int i=0; i<enyo_games[last_game_selected].game_pwad.count();i++)
    {
		ui->list_pwads->addItem(enyo_games[last_game_selected].game_pwad.at(i));
    }
	ui->list_pwads->setCurrentRow (cur_sel + 1);
	ui->button_pwad_up->setEnabled (true);
	ui->button_pwad_down->setEnabled ((cur_sel + 1) < ui->list_pwads->count() - 1 ? true:false);
	return;
}

void e_mainwindow::on_cb_engines_activated(int index)
{
    last_engine_selected = index;
    ui->line_engine_path->setText (enyo_engines[last_engine_selected].engine_binary_path);
}

void e_mainwindow::on_line_engine_path_editingFinished()
{
    enyo_engines[last_engine_selected].engine_binary_path = ui->line_engine_path->text();
}

void e_mainwindow::on_btn_select_engine_path_clicked()
{
    QFileDialog wad_selector(this);
    wad_selector.setFileMode(QFileDialog::ExistingFile);
    wad_selector.setDirectory(DEFAULT_ENGINEDIR);
    QStringList wad_filename;
    if (wad_selector.exec())
    {
        wad_filename = wad_selector.selectedFiles();
        enyo_engines[last_engine_selected].engine_binary_path = wad_filename.at(0);
        ui->line_engine_path->setText (wad_filename.at(0));
    }
}

void e_mainwindow::on_cb_show_output_toggled(bool checked)
{
    	show_output = checked;
    	if (show_output)
    	{
    		ui->tabWidget->insertTab (2, ui->tab_3, "Output");
    	}
    	else
    	{
        	ui->tabWidget->removeTab (2);
    	}
}

void e_mainwindow::on_cb_tabs_top_stateChanged(int state)
{
	tabs_top = (state == Qt::Checked);
	if (tabs_top) 
    	{
		ui->tabWidget->setTabPosition (QTabWidget::North);
    	}
	else
    	{
        	ui->tabWidget->setTabPosition (QTabWidget::West);
    	}
}

void e_mainwindow::on_cb_show_numbers_clicked (bool checked)
{
	show_game_numbers = checked;
	repopulate_game_list();
}

void e_mainwindow::on_btn_add_game_clicked()
{
    if (game_pointer >= MAX_GAMES)
    {
        QMessageBox mbox;
        mbox.setText ("The maximum number of games has been reached.  Please remove unused games to add more.");
        mbox.exec();
        return;
    }
    bool dialog_ok;
    QString new_game;
    QString list_new_game;
    new_game = QInputDialog::getText (this, "Enyo Launcher", "New game name:", QLineEdit::Normal, "", &dialog_ok);
    if (dialog_ok && !new_game.isEmpty())
    {
        for (int i=0; i<game_pointer; i++)
        {
            if (new_game == enyo_games[i].game_name)
            {
                QMessageBox mbox2;
                mbox2.setText("There is already a game with that name.  Choose another.");
                mbox2.exec();
                return;
            }
        }
        QMessageBox mCurrent;
        mCurrent.setText ("Copy current game settings to new game type?");
        mCurrent.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        mCurrent.setDefaultButton(QMessageBox::No);
        int rCurrent = mCurrent.exec();
        if (rCurrent == QMessageBox::Cancel)
        {
            return;
        }
        enyo_games[game_pointer].game_name = new_game;
        if (rCurrent == QMessageBox::Yes)
        {
            enyo_games[game_pointer].engine_id = enyo_games[last_game_selected].engine_id;
            enyo_games[game_pointer].game_advanced_settings = enyo_games[last_game_selected].game_advanced_settings;
            enyo_games[game_pointer].game_pwad = enyo_games[last_game_selected].game_pwad;
            enyo_games[game_pointer].game_wad = enyo_games[last_game_selected].game_wad;
        }
        last_game_selected = game_pointer;
        game_pointer++;
	repopulate_game_list();
        move_enabler();
    }
}

void e_mainwindow::on_btn_remove_game_clicked()
{
	if (game_pointer == 0)
	{
		QMessageBox mbox;
		mbox.setText ("There are no games to remove.");
		mbox.exec();
		return;
	}
    QMessageBox mbConfirm;
    mbConfirm.setText ("Are you sure you want to remove this game?");
    mbConfirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mbConfirm.setDefaultButton(QMessageBox::No);
    int rConfirm = mbConfirm.exec();
    if (rConfirm == QMessageBox::No)
    {
        return;
    }
	for (int i=last_game_selected; i<game_pointer-1; i++)
	{
		enyo_games[i].engine_id = enyo_games[i+1].engine_id;
		enyo_games[i].game_wad = enyo_games[i+1].game_wad;
		enyo_games[i].game_pwad = enyo_games[i+1].game_pwad;
		enyo_games[i].game_advanced_settings = enyo_games[i+1].game_advanced_settings;
		enyo_games[i].game_name = enyo_games[i+1].game_name;
	}
	game_pointer--;
	last_game_selected = 0;
	repopulate_game_list();
	ui->cb_games->setCurrentIndex (last_game_selected);
	on_cb_games_activated (last_game_selected);
	move_enabler();
}

void e_mainwindow::repopulate_game_list()
{
	ui->cb_games->clear();
	for (int i=0; i < game_pointer; i++)
	{
		QString list_new_game;
		if (show_game_numbers)
		{
			list_new_game = QString::number (i+1) + ": ";
		}
		else
		{
			list_new_game = "";
		}
		list_new_game += enyo_games[i].game_name;
		ui->cb_games->addItem (list_new_game, Qt::DisplayRole);
	}
	ui->cb_games->update();
	ui->cb_games->setCurrentIndex (last_game_selected);
	on_cb_games_activated (last_game_selected);
}


void e_mainwindow::on_line_advanced_settings_editingFinished()
{
    enyo_games[last_game_selected].game_advanced_settings = ui->line_advanced_settings->text();
}

void e_mainwindow::on_btn_add_engine_clicked()
{
    if (engine_pointer >= MAX_ENGINES)
    {
        QMessageBox mbox;
        mbox.setText ("The maximum number of engines has been reached.  Please remove unused games to add more.");
        mbox.exec();
        return;
    }
    bool dialog_ok;
    QString new_engine;
    new_engine = QInputDialog::getText (this, "Enyo Launcher", "New engine name:", QLineEdit::Normal, "", &dialog_ok);
    if (dialog_ok && !new_engine.isEmpty())
    {
        for (int i=0; i<engine_pointer; i++)
        {
            if (new_engine == enyo_engines[i].engine_name)
            {
                QMessageBox mbox2;
                mbox2.setText("There is already an engine with that name.  Choose another.");
                mbox2.exec();
                return;
            }
        }
        enyo_engines[engine_pointer].engine_name = new_engine;
        enyo_engines[engine_pointer].engine_binary_path = "";
        last_engine_selected = engine_pointer;
        ui->cb_engines->addItem (new_engine);
        ui->cb_elock->addItem(new_engine);
        ui->cb_engines->setCurrentIndex(last_engine_selected);
	ui->line_engine_path->setText(enyo_engines[engine_pointer].engine_binary_path);
        engine_pointer++;
    }
}

void e_mainwindow::on_btn_remove_engine_clicked()
{
	if (engine_pointer == 0)
	{
		QMessageBox mbox;
		mbox.setText ("There are no engines to remove.");
		mbox.exec();
		return;
	}
    QMessageBox mbConfirm;
    mbConfirm.setText ("Are you sure you want to remove this engine?");
    mbConfirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mbConfirm.setDefaultButton(QMessageBox::No);
    int rConfirm = mbConfirm.exec();
    if (rConfirm == QMessageBox::No)
    {
        return;
    }
	for (int i=last_engine_selected; i<engine_pointer-1; i++)
	{
		enyo_engines[i].engine_name = enyo_engines[i+1].engine_name;
		enyo_engines[i].engine_binary_path = enyo_engines[i+1].engine_binary_path;
	}
	engine_pointer--;
	ui->cb_engines->clear();
	for (int i=0; i<engine_pointer;i++)
	{
		ui->cb_engines->addItem(enyo_engines[i].engine_name, Qt::DisplayRole);
	}
	ui->cb_engines->update();
    ui->cb_elock->clear();
    ui->cb_elock->addItem ("(none)", Qt::DisplayRole);
    for (int i=0; i<engine_pointer; i++)
    {
        ui->cb_elock->addItem(enyo_engines[i].engine_name, Qt::DisplayRole);
    }
    if (enyo_games[last_game_selected].engine_id == last_engine_selected+1)
    {
        enyo_games[last_game_selected].engine_id = 0;
    }
    ui->cb_elock->setCurrentIndex(enyo_games[last_game_selected].engine_id);
	last_engine_selected = 0;
	ui->cb_engines->setCurrentIndex (last_engine_selected);
	on_cb_engines_activated (last_engine_selected);
}

void e_mainwindow::on_cb_elock_activated(int index)
{
    	enyo_games[last_game_selected].engine_id = index+1;
}

void e_mainwindow::on_action_About_triggered()
{
	about_window abt ;
	abt.exec();
}

void e_mainwindow::on_action_Run_triggered()
{
	on_run_engine_clicked();
}

void e_mainwindow::on_actionE_xit_triggered()
{
	this->save_settings();
	delete ui;
	exit(0);
}

void e_mainwindow::on_action_Webpage_triggered()
{
	QDesktopServices::openUrl (QUrl(ENYO_HOMEPAGE));
}

void e_mainwindow::on_cb_exit_toggled(bool checked)
{
	exit_after_run = checked;
}

void e_mainwindow::on_btn_move_game_up_clicked()
{
	game_settings tmp;
	QString txttmp;
	tmp = enyo_games[last_game_selected];
	enyo_games[last_game_selected] = enyo_games[last_game_selected - 1];
	enyo_games[--last_game_selected] = tmp;
	repopulate_game_list();
	move_enabler();
}

void e_mainwindow::on_btn_move_game_down_clicked()
{
	game_settings tmp;
	QString txttmp;
	tmp = enyo_games[last_game_selected];
	txttmp = ui->cb_games->itemText(last_game_selected);
	enyo_games[last_game_selected] = enyo_games[last_game_selected + 1];
	enyo_games[++last_game_selected] = tmp;
	repopulate_game_list();
	move_enabler();
}

void e_mainwindow::on_check_elock_clicked (bool checked)
{
	ui->cb_elock->setEnabled (checked);
	if (checked)
	{
	    ui->cb_elock->setCurrentIndex(0);
	    enyo_games[last_game_selected].engine_id = 1;
	}
	else
	{
		ui->cb_elock->setCurrentIndex(0);
		enyo_games[last_game_selected].engine_id = 0;
	}
	ui->cb_elock->update();
}

void e_mainwindow::on_btn_edit_label_clicked()
{
	QString game_change;
	QString current_name;
	bool dialog_ok;
	current_name = enyo_games[last_game_selected].game_name;
	game_change = QInputDialog::getText (this, "Enyo Launcher", "New game name:", QLineEdit::Normal, current_name, &dialog_ok);
	if (dialog_ok && !game_change.isEmpty() && !(game_change == current_name))
	{
		enyo_games[last_game_selected].game_name = game_change;
		repopulate_game_list();
	}
}

void e_mainwindow::on_list_pwads_itemChanged (QListWidgetItem *item)
{
	int cur_sel = ui->list_pwads->currentRow();
	ui->button_pwad_up->setEnabled (cur_sel > 0 ? true:false);
	ui->button_pwad_down->setEnabled (cur_sel < (ui->list_pwads->count() - 1) ? true:false);
}

void e_mainwindow::on_list_pwads_itemClicked (QListWidgetItem *item)
{
	int cur_sel = ui->list_pwads->currentRow();
	ui->button_pwad_up->setEnabled (cur_sel > 0 ? true:false);
	ui->button_pwad_down->setEnabled (cur_sel < (ui->list_pwads->count() - 1) ? true:false);

}

