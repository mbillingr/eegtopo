/*
 *  This file is part of EEGtopo.
 *
 *  Foobar is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  Copyright 2012 Martin Billinger
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "previewwidget.h"
#include "elselwidget.h"

#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/frame.h>
#include <gtkmm/window.h>

class MainWindow : public Gtk::Window
{
public:
    MainWindow();
    virtual ~MainWindow();

    void on_elselcombo_changed( );

private:
    std::map< std::string, std::vector<std::string> > elgroups;

    ElselWidget elsel;
    Gtk::ComboBoxText elselcombo;
    Gtk::Frame elselframe;
    Gtk::Box elselbox;

    PreviewWidget preview;

    Gtk::Box leftright;
};

#endif // MAINWINDOW_H
