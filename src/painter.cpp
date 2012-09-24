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

#include "painter.h"

#include "colormap.h"

#include <Eigen/Dense>

#include <boost/foreach.hpp>
#include <boost/assign/std/vector.hpp>
#include <iostream>
#include <cmath>

Painter::Painter( Cairo::RefPtr<Cairo::Context> &cairo_context )
{
    using boost::assign::operator +=;
    cr = cairo_context;

    first_draw = true;

    set<double>( "grid_line_width", 0.005 );
    set<double>( "head_line_width", 0.03 );
    set<double>( "head_radius", 1.0 );
    set<double>( "nose_angle", 16 );
    set<double>( "nose_length", 0.18 );
    set<double>( "ear_level", 1 );
    set<double>( "ear_radius", 1.1 );
    set<double>( "ear_angle", 30 );

    set<double>( "draw_range_x", -1 );
    set<double>( "draw_range_y", -1 );

    set<double>( "topo_resolution", 256 );
    set<double>( "topo_radius", 1.0 );

    el0.add_position( "N", 0, get<double>("head_radius") + get<double>("nose_length") );
    el0.add_position( "El", -get<double>("ear_radius"), 0 );
    el0.add_position( "Er", get<double>("ear_radius"), 0 );

    el0.calc_mindist( );

    Electrodes::const_iterator it = el0.begin();
    for( ; it != el0.end(); it++ )
    {
        electrodes[it->first] = ElectrodeGraphics( it->first, it->second.x, it->second.y );
        groups["all"].push_back( it->first );
    }
    groups["10-20"] += "Fp1", "Fp2", "F7", "F3", "Fz", "F4", "F8", "T7", "C3", "Cz", "C4", "T8", "P7", "P3", "Pz", "P4", "P8", "O1", "O2";
}

void Painter::draw_first( )
{
    double x_extent = get<double>("draw_range_x");
    double y_extent = get<double>("draw_range_y");

    bool auto_x = x_extent == -1;
    bool auto_y = y_extent == -1;

    if( auto_x && auto_y )
    {
        // calculate extents
        std::map< std::string, ElectrodeGraphics >::iterator it = electrodes.begin();
        for( ; it!=electrodes.end(); it++ )
        {
            if( it->second.get<bool>("visible") )
            {
                double x = it->second.get<double>("x");
                double y = it->second.get<double>("y");
                double r = it->second.get<double>("radius") + it->second.get<double>("line_width") / 2.0;

                if( auto_x )
                {
                    x_extent = std::max( x_extent, std::abs(x+r) );
                    x_extent = std::max( x_extent, std::abs(x-r) );
                }

                if( auto_y )
                {
                    y_extent = std::max( y_extent, std::abs(y+r) );
                    y_extent = std::max( y_extent, std::abs(y-r) );
                }
            }
        }

        if( auto_x )
        {
            x_extent = std::max( x_extent, get<double>("ear_radius") + get<double>("head_line_width") / 2.0 );
            x_extent = std::max( x_extent, get<double>("topo_radius") );
        }

        if( auto_y )
        {
            y_extent = std::max( y_extent, get<double>("head_radius") + get<double>("nose_length") + get<double>("head_line_width") / 2.0 );
            y_extent = std::max( y_extent, get<double>("topo_radius") );
        }
    }

    cr->scale( 1.0/x_extent, -1.0/y_extent );

    first_draw = false;
}

void Painter::draw_head( )
{
    if( first_draw )
        draw_first( );

    // The Nose and head (and ears)

    cr->set_line_width( get<double>("head_line_width") );
    cr->set_line_join( Cairo::LINE_JOIN_ROUND );

    cr->arc( 0, 0, 1, 0, 2 * M_PI );

    double head_radius = get<double>("head_radius");
    double nose_length = get<double>("nose_length");
    double nose_angle = get<double>("nose_angle");
    double ear_radius = get<double>("ear_radius");
    double ear_angle = get<double>("ear_angle");

    double x = head_radius * cos((90.0-nose_angle/2)*M_PI/180.0);
    double y = head_radius * sin((90.0-nose_angle/2)*M_PI/180.0);
    cr->move_to( x, y );
    cr->line_to( 0, head_radius * (1+nose_length) );
    cr->line_to( -x, y );

    switch( (int)floor( get<double>("ear_level") ) )
    {
    default: break;
    case 1:

        double x = head_radius * cos((0.0-ear_angle/2)*M_PI/180.0);
        double y = head_radius * sin((0.0-ear_angle/2)*M_PI/180.0);

        double x1 = ear_radius * cos((0.0-ear_angle/2)*M_PI/180.0);
        double y1 = ear_radius * sin((0.0-ear_angle/2)*M_PI/180.0);

        cr->move_to( x, y );
        cr->curve_to( x1, y1, ear_radius, y, ear_radius, 0 );
        cr->curve_to( ear_radius, 0, ear_radius, -y, x, -y );

        cr->move_to( -x, y );
        cr->curve_to( -x1, y1, -ear_radius, y, -ear_radius, 0 );
        cr->curve_to( -ear_radius, 0, -ear_radius, -y, -x, -y );

        break;
    }

    cr->stroke();
}

void Painter::draw_grid(const int &level )
{
    if( first_draw )
        draw_first( );

    cr->set_line_width( get<double>("grid_line_width") );
    cr->arc( 0, 0, 0.8, 0, 2*M_PI );

    cr->move_to( 0, 1 );
    cr->line_to( 0, -1 );
    cr->move_to( -1, 0 );
    cr->line_to( 1, 0 );

    if( level >= 1 )
    {
        arc3p( el0.get("P4"), el0.get("C4"), el0.get("F4") );
        arc3p( el0.get("F3"), el0.get("C3"), el0.get("P3") );

        arc3p( el0.get("F9"), el0.get("Fz"), el0.get("F10") );
        arc3p( el0.get("P10"), el0.get("Pz"), el0.get("P9") );
    }

    if( level >= 2 )
    {
        arc3p( el0.get("P2"), el0.get("C2"), el0.get("F2") );
        arc3p( el0.get("P6"), el0.get("C6"), el0.get("F6") );
        arc3p( el0.get("F1"), el0.get("C1"), el0.get("P1") );
        arc3p( el0.get("F5"), el0.get("C5"), el0.get("P5") );

        arc3p( el0.get("AF7"), el0.get("AFz"), el0.get("AF8") );
        arc3p( el0.get("FT9"), el0.get("FCz"), el0.get("FT10") );
        arc3p( el0.get("TP10"), el0.get("CPz"), el0.get("TP9") );
        arc3p( el0.get("PO8"), el0.get("POz"), el0.get("PO7") );
    }

    cr->stroke();
}

void Painter::draw_layout( )
{
    if( first_draw )
        draw_first( );

    double fontsize = 1.0;
    cr->set_font_size( fontsize );

    std::map< std::string, ElectrodeGraphics >::const_iterator it = electrodes.begin();
    for( ; it!=electrodes.end(); it++ )
    {
        ElectrodeGraphics el = it->second;
        if( !el.get<bool>("visible") )
            continue;

        double x = el.get<double>("x");
        double y = el.get<double>("y");
        double radius = el.get<double>("radius");
        double line_width = el.get<double>("line_width");
        Color<double> line = el.get<Color<double> >("line");
        Color<double> fill = el.get<Color<double> >("fill");

        cr->move_to( x + radius, y );
        cr->arc( x, y, radius, 0, 2*M_PI );

        cr->set_source_rgb( fill.r, fill.g, fill.b );
        cr->fill_preserve( );
        cr->set_source_rgb( line.r, line.g, line.b );
        cr->set_line_width( line_width );
        cr->stroke();

        if( el.get<bool>("show_label") )
        {
            cr->select_font_face( el.get<std::string>("font_family"), (Cairo::FontSlant)el.get<int>("font_slant"), (Cairo::FontWeight)el.get<int>("font_weight") );

            Cairo::TextExtents ext;
            cr->get_text_extents( el.get<std::string>("label"), ext );

            double w = ext.width/2.0 + ext.x_bearing;
            double h = ext.height/2.0 + ext.y_bearing;

            // adjust font size so that the corners of the biggest string just touch
            // the inner border of the surrounding circle
            fontsize = std::min( fontsize, (radius - line_width/2.0) / sqrt(w*w + h*h) );
        }
    }

    // set correct text orientation (y points down)
    cr->scale( 1, -1 );

    for( it = electrodes.begin(); it!=electrodes.end(); it++ )
    {
        ElectrodeGraphics el = it->second;
        if( !el.get<bool>("visible") )
            continue;

        if( el.get<bool>("show_label") )
        {
            cr->select_font_face( el.get<std::string>("font_family"), (Cairo::FontSlant)el.get<int>("font_slant"), (Cairo::FontWeight)el.get<int>("font_weight") );
            cr->set_font_size( fontsize * el.get<double>("font_scale") );

            std::string label = el.get<std::string>("label");
            Color<double> fontc = el.get<Color<double> >( "font" );

            Cairo::TextExtents ext;
            cr->get_text_extents( label, ext );

            cr->move_to( el.get<double>("x") - ext.width/2.0 - ext.x_bearing, -el.get<double>("y") - ext.height/2.0 - ext.y_bearing );

            cr->set_source_rgb( fontc.r, fontc.g, fontc.b );

            cr->show_text( label );
        }
    }

    // make y point up again
    cr->scale( 1, -1 );
}

void Painter::draw_layout( const std::vector< std::string >& el, double radius )
{
    std::vector<bool> vis;
    std::vector<double> rad;

    // save visibility state
    std::map< std::string, ElectrodeGraphics >::iterator it = electrodes.begin();
    for( ; it!=electrodes.end(); it++ )
    {
        vis.push_back( it->second.get<bool>("visible") );
        rad.push_back( it->second.get<double>("radius") );
        it->second.set( "visible", false );
    }

    // temporarily set custom visibility state
    for( size_t i=0; i<el.size(); i++ )
    {
        electrodes[el[i]].set( "visible", true );
        if( radius > 0 ) electrodes[el[i]].set( "radius", radius );
    }

    // draw layout
    draw_layout( );

    // restore visibility state
    it = electrodes.begin();
    for( size_t i=0; it!=electrodes.end(); i++, it++ )
    {
        it->second.set( "visible", vis[i] );
        it->second.set( "radius", rad[i] );
    }
}

void Painter::draw_topo()
{
    if( first_draw )
        draw_first( );

    // TODO: don't hardcode values
    size_t N = get<double>("topo_resolution");
    double radius = get<double>("topo_radius");

    std::vector<ElectrodeGraphics> els;
    std::map< std::string, ElectrodeGraphics >::const_iterator it = electrodes.begin();
    for( ; it!=electrodes.end(); it++ )
    {
        double v = it->second.get<double>( "value" );

        //std::cout << v << std::endl;

        if( v == v )    // evaluates to false if v == nan
        {
            els.push_back( it->second );
        }
    }

    size_t M = els.size();
    Eigen::MatrixXd distances( M, M );
    Eigen::MatrixXd greenm( M, M );
    Eigen::VectorXd v( M );

    for( size_t i=0; i<M; i++ )
    {
        v(i) = els[i].get<double>( "value" );

        distances(i,i) = 0;
        greenm(i,i) = 0;

        double ax = els[i].get<double>( "x" );
        double ay = els[i].get<double>( "y" );

        for( size_t j=i+1; j<M; j++ )
        {
            double bx = els[j].get<double>( "x" );
            double by = els[j].get<double>( "y" );

            double d = sqrt( (ax-bx)*(ax-bx) + (ay-by)*(ay-by) );
            distances(i,j) = d;
            distances(j,i) = d;

            d = d * d * ( log(d) - 1 );
            greenm(i,j) = d;
            greenm(j,i) = d;
        }
    }

    Eigen::VectorXd weights = greenm.householderQr().solve(v);

    //std::cout << "distances: " << std::endl << distances << std::endl;
    //std::cout << "greenm: " << std::endl << greenm << std::endl;
    //std::cout << "weights: " << std::endl << weights << std::endl;

    Eigen::VectorXd greenv( M );
    Eigen::MatrixXd data( N, N );

    double range_min = 9e300;
    double range_max = -range_min;

    for( size_t i=0; i<N; i++ )
    {
        double y = ( i/(N-1.0) - 0.5 ) * 2 * radius;
        for( size_t j=0; j<N; j++ )
        {
            double x = ( j/(N-1.0) - 0.5 ) * 2 * radius;

            for( size_t k=0; k<M; k++ )
            {
                double ax = els[k].get<double>( "x" );
                double ay = els[k].get<double>( "y" );

                double d = sqrt( (x-ax)*(x-ax) + (y-ay)*(y-ay) );
                if( d == 0 )
                    greenv(k) = 0;
                else
                    greenv(k) = d * d * ( log(d) - 1 );
            }
            double d = greenv.dot( weights );
            data(i,j) = d;

            range_min = std::min( range_min, d );
            range_max = std::max( range_max, d );
        }
    }

    //std::cout << "range: " << range_min << " - " << range_max << std::endl;

    Jet colormap( range_min, range_max );

    cr->save();

    Cairo::RefPtr<Cairo::ImageSurface> image = Cairo::ImageSurface::create( Cairo::FORMAT_RGB24, N, N );
    unsigned char* rgb = image->get_data();

    for( size_t i=0; i<N; i++ )
    {
        for( size_t j=0; j<N; j++ )
        {
            Color<unsigned char> c = colormap( data(i,j) );
            rgb[(i*N+j)*4+0] = c.b;
            rgb[(i*N+j)*4+1] = c.g;
            rgb[(i*N+j)*4+2] = c.r;
        }
    }

    cr->arc( 0, 0, radius, 0, 2*M_PI );
    cr->clip( );

    cr->translate( -1*radius, -1*radius );
    cr->scale( 2.0*radius/N, 2.0*radius/N );

    cr->set_source( image, 0, 0 );
    cr->paint( );

    cr->restore();
}

void Painter::arc3p( const Electrodes::point_t &a, const Electrodes::point_t &b, const Electrodes::point_t &c )
{
    Electrodes::circle_t circle( a, b, c );

    double phi_a = atan2( a.y-circle.y, a.x-circle.x );
    double phi_b = atan2( c.y-circle.y, c.x-circle.x );

    cr->move_to( a.x, a.y );
    cr->arc( circle.x, circle.y, circle.r, phi_a, phi_b );
}

ElectrodeGraphics &Painter::find_electrode( const std::string &el )
{
    std::map< std::string, ElectrodeGraphics >::iterator it = electrodes.find( el );
    if( it == electrodes.end() )
        throw std::invalid_argument( "Unknown electrode: " + el );
    return it->second;
}
std::vector<std::string> &Painter::find_group( const std::string &gr )
{
    std::map< std::string, std::vector<std::string> >::iterator it = groups.find( gr );
    if( it == groups.end() )
        throw std::invalid_argument( "Unknown group: " + gr );
    return it->second;
}

void Painter::define_group( const std::string &group, const std::vector<std::string> &electrodes )
{
    groups[group] = electrodes;
}
