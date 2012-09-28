#include "guimain.h"

//#include "previewwidget.h"
#include "elselwidget.h"

#include <gtkmm/application.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/window.h>


int gui_main( int argc, char *argv[] )
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create( argc, argv, "EEGtopo" );

    ElselWidget elsel;

    elsel.init_widgets( );

    Gtk::Window window;
    window.add( elsel );
    window.show_all_children( );

//    Cairo::RefPtr<Cairo::Context> context = draw.get_window()->create_cairo_context();

    return app->run( window );
}
