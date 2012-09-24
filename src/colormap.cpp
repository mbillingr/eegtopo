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

#include "colormap.h"

#include <iostream>

Color<unsigned char> Jet::operator()( double val )
{
    const double S = 6.0;
    const double T = 1.0 / 6.0;

    Color<double> c;

    val = normalize( val );

    if( val > 1 )
    {
        std::cout << "min: " << min_ << std::endl;
        std::cout << "max: " << max_ << std::endl;
        std::cout << "val: " << val << std::endl;
    }

    if( val <= 0 ) {
        return Color<unsigned char>( 0, 0, 128 );
    }

    if( val <= T ) {
        c.r = 0;
        c.g = 0;
        c.b = (val * S) * 0.5 + 0.5;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val <= 2*T ) {
        c.r = 0;
        c.g = (val-T) * S;
        c.b = 1;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val <= 3*T ) {
        c.r = 0;
        c.g = 1;
        c.b = 1 - (val-2*T) * S;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val <= 4*T ) {
        c.r = (val-3*T) * S;
        c.g = 1;
        c.b = 0;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val <= 5*T ) {
        c.r = 1;
        c.g = 1 - (val-4*T) * S;
        c.b = 0;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val <= 6*T ) {
        c.r = 1;
        c.g = (val-5*T) * S;
        c.b = (val-5*T) * S;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val > 6*T ) {
        return Color<unsigned char>( 255, 255, 255 );
    }

    return Color<unsigned char>( 0, 0, 0 );
}

// =========================================================================
// =========================================================================

Color<unsigned char> Sym::operator()( double val )
{
    const double S = 6.0;
    const double T = 1.0 / 6.0;

    Color<double> c;

    val = normalize( val );

    if( val > 1 )
    {
        std::cout << "min: " << min_ << std::endl;
        std::cout << "max: " << max_ << std::endl;
        std::cout << "val: " << val << std::endl;
    }

    if( val <= 0 ) {
        return Color<unsigned char>( 0, 0, 128 );
    }

    if( val <= T ) {
        c.r = 0;
        c.g = 0;
        c.b = (val * S) * 0.5 + 0.5;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val <= 2*T ) {
        c.r = 0;
        c.g = (val-T) * S;
        c.b = 1;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val <= 3*T ) {
        c.r = (val-2*T) * S;
        c.g = 1;
        c.b = 1;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val <= 4*T ) {
        c.r = 1;
        c.g = 1;
        c.b = 1 - (val-3*T) * S;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val <= 5*T ) {
        c.r = 1;
        c.g = 1 - (val-4*T) * S;
        c.b = 0;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val <= 6*T ) {
        c.r = 1 - (val-5*T) * S * 0.5;
        c.g = 0;
        c.b = 0;
        return Color<unsigned char>( c.r*255, c.g*255, c.b*255 );
    }

    if( val > 6*T ) {
        return Color<unsigned char>( 128, 0, 0 );
    }

    return Color<unsigned char>( 0, 0, 0 );
}
