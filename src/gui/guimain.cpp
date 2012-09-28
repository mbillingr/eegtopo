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

#include "guimain.h"

#include "previewwidget.h"
#include "elselwidget.h"

#include <gtkmm/application.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/window.h>


int gui_main( int argc, char *argv[] )
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create( argc, argv, "EEGtopo" );

    ElselWidget elsel;

    PreviewWidget preview;

    Gtk::Box leftright( Gtk::ORIENTATION_HORIZONTAL );
    leftright.pack_start( elsel, false, true );
    elsel.show();

    leftright.pack_start( preview, true, true );
    preview.show();

    Gtk::Window window;
    window.add( leftright );
    window.show_all_children( );

    return app->run( window );
}
