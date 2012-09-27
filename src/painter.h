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

#ifndef PAINTER_H
#define PAINTER_H

#include "electrodegraphics.h"
#include "electrodes.h"

#include <cairomm/cairomm.h>
#include <boost/foreach.hpp>

class Painter : public MemberMap
{
public:
    Painter( const Cairo::RefPtr<Cairo::Context>& cairo_context );

    void draw_first( );

    void draw_head( );
    void draw_grid( int const& level );

    void draw_layout( );
    void draw_layout(const std::vector< std::string >& el , double radius=-1);

    void draw_topo( );

    template <typename T>
    void set_electrode( const std::string& el, const std::string& var, const T& val )
    {
        find_electrode( el ).set( var, val );
    }

    template <typename T>
    void set_group( const std::string& gr, const std::string& var, const T& val )
    {
        BOOST_FOREACH( std::string el, find_group( gr ) )
                find_electrode( el ).set( var, val );
    }

    void define_group( const std::string &group, const std::vector<std::string> &electrodes );

protected:
    void arc3p( const Electrodes::point_t &a, const Electrodes::point_t &b, const Electrodes::point_t &c );

    ElectrodeGraphics &find_electrode( const std::string &el );
    std::vector<std::string> &find_group( const std::string &gr );

private:
    Cairo::RefPtr<Cairo::Context> cr;

    Electrodes el0;
    std::map< std::string, ElectrodeGraphics > electrodes;
    std::map< std::string, std::vector<std::string> > groups;

    bool first_draw;

    /*double head_linewidth;  // linewidth for head and nose
    double head_radius;     // radius of the head cirle
    double nose_angle;      // opening angle of the nose (degrees)
    double nose_length;     // length of the nose (relative to head)

    int ear_level;          // ear drawing style
    double ear_radius;
    double ear_angle;*/
};

#endif // PAINTER_H
