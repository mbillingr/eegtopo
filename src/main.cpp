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

#include "parser.h"
#include "electrodegraphics.h"
#include "electrodes.h"

#include <cairomm/cairomm.h>

#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>


using namespace std;

int main( int argc, char *argv[] )
{

    using boost::program_options::positional_options_description;
    using boost::program_options::options_description;
    using boost::program_options::variables_map;
    using boost::program_options::command_line_parser;
    using boost::program_options::store;
    using boost::program_options::notify;
    using boost::program_options::value;

    vector<string> opt_elist;
    string opt_infile, opt_outfile;
    double opt_radius;

    options_description desc( "Allowed options" );
    desc.add_options( )
            ( "help,h", "produce help message" )
            ( "file,f", value<string>( &opt_infile ), "definition file" )
            ( "output,o", value<string>( &opt_outfile ), "output file" )
            ( "electrodes,e", value< vector<string> >( &opt_elist )->multitoken(), "list of electrodes" )
            ( "radius,r", value<double>( &opt_radius )->default_value(0.075,"0.075"), "electrode radius" );

    positional_options_description pod;
    pod.add( "file", 1 );

    variables_map vm;
    store( command_line_parser( argc, argv ).options(desc).positional(pod).run(), vm );
    notify( vm );

    if( argc == 1 )
    {
        cout << "GUI not yet implemented." << endl;
        cout << "Command Line Usage: " + string(argv[0]) << " [options] [file] [options]" << endl;
        cout << desc << endl;
        return 0;
    }

    if( vm.count("help") )
    {
        cout << "Usage: " + string(argv[0]) << " [options] [file] [options]" << endl;
        cout << desc << endl;
        return 1;
    }

    if( !(vm.count("file") ^ vm.count("electrodes")) )
    {
        cout << "Error: either file or electrode list required." << endl;
        return 1;
    }

    if( !vm.count("output") )
    {
        if( vm.count("file") )
        {
            size_t p1 = opt_infile.find_last_of( '.' );
            size_t p0 = opt_infile.find_last_of( '\\' );
            if( p0 == std::numeric_limits<size_t>::max() ) p0 = opt_infile.find_last_of( '/' );
            p0 += 1;
            opt_outfile = opt_infile.substr( p0, p1-p0 ) + ".pdf";
        }
        else
        {
            opt_outfile = "hello.pdf";
        }
    }

    size_t p = opt_outfile.find_last_of( '.' );
    string ext = opt_outfile.substr( p );
    std::transform( ext.begin(), ext.end(), ext.begin(), ::tolower );

    Cairo::RefPtr<Cairo::Surface> surface;

    if( ext == ".pdf")
        surface = Cairo::PdfSurface::create( opt_outfile, 100, 100);
    else if( ext == ".eps" || ext == ".ps")
        surface = Cairo::PsSurface::create( opt_outfile, 100, 100);
    else
    {
        cout << "Error: unrecognized output extension: " << ext << endl;
        return 1;
    }

    Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

    cr->scale( 50, 50 );
    cr->translate( 1, 1 );

    Painter painter( cr );

    if( vm.count("file") )
    {
        Parser p;
        p.parse_file( opt_infile, painter );

        return 0;
    }

    if( vm.count("electrodes") )
    {
        painter.draw_head( );
        painter.draw_grid( 2 );
        painter.draw_layout( opt_elist, opt_radius );
    }

    return 0;
}

