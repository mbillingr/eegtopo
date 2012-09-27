#include "guimain.h"

#include "previewwidget.h"

#include <gtkmm/application.h>
#include <gtkmm/window.h>


int gui_main( int argc, char *argv[] )
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create( argc, argv, "EEGtopo" );

    PreviewWidget preview;

    Gtk::Window window;
    window.add( preview );
    window.show_all_children( );

//    Cairo::RefPtr<Cairo::Context> context = draw.get_window()->create_cairo_context();

    return app->run( window );
}
