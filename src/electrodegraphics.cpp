#include "electrodegraphics.h"

#include <limits>

ElectrodeGraphics::ElectrodeGraphics( )
{
    set( "label", "" );
    set( "x", 0.0 );
    set( "y", 0.0 );

    set_defaults( );
}

ElectrodeGraphics::ElectrodeGraphics( const std::string &label )
{
    static const Electrodes el;

    Electrodes::point_t p = el.get( label );

    set( "label", label );
    set( "x", p.x );
    set( "y", p.y );

    set_defaults( );
}

ElectrodeGraphics::ElectrodeGraphics( const std::string &label, double x, double y )
{
    set( "label", label );
    set( "x", x );
    set( "y", y );

    set_defaults( );
}

void ElectrodeGraphics::set_defaults( )
{
    set( "radius",  0.05 );

    set( "visible",  false );

    set( "line_width",  0.015 );

    set( "line",  Color<double>( 0, 0, 0 ) );
    set( "font",  Color<double>( 0, 0, 0 ) );
    set( "fill",  Color<double>( 1, 1, 1 ) );


    set( "show_label",  true );
    set( "font_scale",  1.0 );
    set<std::string>( "font_family",  "helvetica" );
    set( "font_slant",  Cairo::FONT_SLANT_NORMAL );
    set( "font_weight",  Cairo::FONT_WEIGHT_BOLD );

    set( "value",  std::numeric_limits<double>::quiet_NaN() );
}
