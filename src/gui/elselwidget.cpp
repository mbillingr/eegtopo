#include "elselwidget.h"

#include <iostream>

ElselWidget::ElselWidget()
    : Gtk::Fixed( ),
      painter( Cairo::RefPtr<Cairo::Context>() )
{
    scale = 0.8;
}

ElselWidget::~ElselWidget()
{
    map_label_button::iterator it = buttons.begin( );
    for( ; it!=buttons.end(); it++ )
    {
        delete it->second;
    }
}

void ElselWidget::init_widgets( )
{
    Electrodes::const_iterator it = el.begin();
    for( ; it!=el.end(); it++ )
    {
        ButtonWidget* b = new ButtonWidget();
        buttons[it->first] = b;
        labels[b] = it->first;

        b->set_border_width( 0 );

        put( *b, it->second.x, it->second.y );
    }

    show_all_children( );
}

void ElselWidget::on_size_allocate( Gtk::Allocation& allocation )
{
    map_label_button::iterator it = buttons.begin( );

    int w = allocation.get_width();
    int h = allocation.get_height();

    int m = std::min( w, h );

    for( ; it!=buttons.end(); it++ )
    {
        Electrodes::point_t p = el.get( it->first );

        int x = ( scale * p.x * m + w - it->second->get_width() ) * 0.5;
        int y = ( scale * p.y * m + h - it->second->get_height() ) * 0.5;

        move( *it->second, x, y );
    }

    Gtk::Fixed::on_size_allocate( allocation );
}

bool ElselWidget::on_draw( const Cairo::RefPtr<Cairo::Context>& context )
{
    painter.set_context( context );

    int w = get_width( );
    int h = get_height( );

    context->save();

    if( h < w )
    {
        context->translate( (w-h)/2, 0 );
        context->scale( h, h );
    }
    else
    {
        context->translate( 0, (h-w)/2 );
        context->scale( w, w );
    }

    context->scale( 0.5, -0.5 );
    context->translate( 1, -1 );
    context->scale( scale, scale );

    painter.draw_head( );
    painter.draw_grid( 2 );

    context->restore();

    Gtk::Fixed::on_draw( context );
}

Gtk::SizeRequestMode ElselWidget::get_request_mode_vfunc() const
{
    return Gtk::SIZE_REQUEST_HEIGHT_FOR_WIDTH;
}

void ElselWidget::get_preferred_width_vfunc(int& minimum_width, int& natural_width) const
{
    int button_minimum_width, button_natural_width;
    int button_minimum_height, button_natural_height;

    buttons.begin()->second->get_preferred_width( button_minimum_width, button_natural_width );
    buttons.begin()->second->get_preferred_height( button_minimum_height, button_natural_height );

    double minimum_distance = sqrt( button_minimum_width*button_minimum_width +
                                    button_minimum_height*button_minimum_height );
    double natural_distance = sqrt( button_natural_width*button_natural_width +
                                      button_natural_height*button_natural_height );

    double minimum_scale = minimum_distance / ( el.get_mindist() * scale );
    double natural_scale = natural_distance / ( el.get_mindist() * scale );

    minimum_width = ceil( minimum_scale );
    natural_width = ceil( natural_scale );
}

void ElselWidget::get_preferred_height_vfunc(int& minimum_height, int& natural_height) const
{
    // we prefer to be square :)
    get_preferred_width_vfunc( minimum_height, natural_height );
}

void  ElselWidget::get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const
{
    get_preferred_width_vfunc( minimum_width, natural_width );

    minimum_width = std::max( height, minimum_width );
    natural_width = std::max( height, natural_width );
}

void ElselWidget::get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const
{
    get_preferred_width_vfunc( minimum_height, natural_height );

    minimum_height = std::max( width, minimum_height );
    natural_height = std::max( width, natural_height );
}
