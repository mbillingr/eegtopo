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

#include "mainwindow.h"

#include <gtkmm/button.h>
#include <gtkmm/radiobutton.h>
#include <boost/assign/std/vector.hpp>

MainWindow::MainWindow()
    : elselbox( Gtk::ORIENTATION_VERTICAL )
    , leftright( Gtk::ORIENTATION_HORIZONTAL )
{
    using boost::assign::operator +=;

    elgroups["none"];

    Electrodes::const_iterator it = elsel.get_painter().get_el().begin();
    for( ; it != elsel.get_painter().get_el().end(); it++ )
    {
        elgroups["all"].push_back( it->first );
    }

    elgroups["10-20"] += "Fp1", "Fp2", "F7", "F3", "Fz", "F4", "F8", "T7", "C3", "Cz",
                         "C4", "T8", "P7", "P3", "Pz", "P4", "P8", "O1", "O2";

    elgroups["extended"] += "AF7", "AFz", "AF8", "F3", "F1", "Fz", "F2", "F4", "FT7",
                            "FC5", "FC3", "FC1", "FCz", "FC2", "FC4", "FC6", "FT8",
                            "C5", "C3", "C1", "Cz", "C2", "C4", "C6", "CP5", "CP3",
                            "CP1", "CPz", "CP2", "CP4", "CP6", "P7", "P3", "Pz", "P4",
                            "P8", "PO3", "POz", "PO4", "O1", "Oz", "O2", "O9", "Iz", "O10";

    //elgroups["additional"] += "Fp1", "Fp2", "F7", "F3", "Fz", "F4", "F8", "T7", "C3", "Cz", "C4", "T8", "P7", "P3", "Pz", "P4", "P8", "O1", "O2";

    elsel.show();

    std::map< std::string, std::vector<std::string> >::iterator git = elgroups.begin();
    for( ; git != elgroups.end(); git++ )
    {
        elselcombo.append( git->first );
    }

    elselcombo.set_active( 1 );
    elselcombo.show();
    elselcombo.signal_changed().connect( sigc::mem_fun(*this, &MainWindow::on_elselcombo_changed) );

    elselbox.pack_start( elselcombo, false, false );
    elselbox.pack_start( elsel, false, false );
    elselbox.show();

    elselframe.add( elselbox );
    elselframe.set_border_width( 10 );
    elselframe.show();

    preview.show();

    leftright.pack_start( elselframe, false, true );
    leftright.pack_start( preview, true, true );
    leftright.show();

    add( leftright );
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_elselcombo_changed( )
{
    elsel.set_visible( elgroups[elselcombo.get_active_text()] );
}
