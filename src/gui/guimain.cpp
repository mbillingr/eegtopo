#include "guimain.h"

#include <gtkmm/application.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/fixed.h>
#include <gtkmm/stock.h>
#include <gtkmm/window.h>


int gui_main( int argc, char *argv[] )
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create( argc, argv, "EEGtopo" );

    Gtk::CheckButton button1, button2;
    //button1.set_border_width( 100 );
    //button2.set_border_width( 200 );

    Gtk::Fixed fixed;
    fixed.put( button1, 1, 1 );
    fixed.put( button2, 100, 100 );

    Gtk::Window window;
    window.add( fixed );
    window.show_all_children( );

    return app->run( window );
}
