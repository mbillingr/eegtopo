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

#ifndef COLORMAP_H
#define COLORMAP_H

#include <vector>

template <typename T>
struct Color
{
public:
    Color( T R=0, T G=0, T B=0) : r(R), g(G), b(B) { }

    Color( const std::vector<double> v )
    {
        r = v[0];
        g = v[1];
        b = v[2];
    }

    T r,g,b;
};

// =========================================================================
// =========================================================================

template <typename T>
class Colormap
{
public:
    Colormap( double min, double max ) : min_(min), max_(max)
    {
        if( max_ == min_ )
        {
            if( max_ == 0 )
            {
                min_ = -1;
                max_ = 1;
            }
            else
            {
                min_ = 0;
                max_ = 2 * max_;
            }
        }
    }

    virtual Color<T> operator()( double val ) = 0;

    double normalize( double val )
    {
        return (val-min_) / (max_-min_);
    }

protected:
    double min_, max_;
};

// =========================================================================
// =========================================================================

class Jet : public Colormap<unsigned char>
{
public:
    Jet( double min, double max ) : Colormap<unsigned char>( min, max ) { }
    virtual Color<unsigned char> operator()( double val );
};

// =========================================================================
// =========================================================================

class Sym : public Colormap<unsigned char>
{
public:
    Sym( double min, double max ) : Colormap<unsigned char>( min, max ) { }
    virtual Color<unsigned char> operator()( double val );
};

#endif // COLORMAP_H
