
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

/*void print_elweights( mglGraph &gr, const Electrodes &el, const vector<double> &weights )
{
    if( el.size() != weights.size() )
        throw invaslantlid_argument( "Same number of electrodes and weights required." );

    for( size_t i=0; i<weights.size(); i++ )
    {
        Electrodes::point_t p = el[i];
        gr.Circle( mglPoint( p.x, p.y, 1), weights[i] , "k" );
    }
}

double entropy( const vector<double> &weights )
{
    double H = 0;
    for( size_t i=0; i<weights.size(); i++ )
        if( weights[i] > 0 )
        {
            H -= weights[i] * log2(weights[i]);
        }
    return H;
}*/

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

    /*list< ElectrodeGraphics > electrodes;

    Electrodes el;

    Electrodes::const_iterator it = el.begin();
    for( ; it != el.end(); it++ )
        electrodes.push_back( it->first );

    electrodes.front().line_width = 0.001;
    electrodes.front().line_green = 1.0;
    electrodes.front().fill_blue = 0.0;
    electrodes.front().font_red = 1.0;
    electrodes.front().font_family = "sans";
    electrodes.front().font_weight = Cairo::FONT_WEIGHT_NORMAL;
    electrodes.front().font_slant = Cairo::FONT_SLANT_OBLIQUE;*/

    /*vector< pair<std::string,size_t> > selection;
    selection.push_back( pair<std::string,size_t>("AF7",0) );
    selection.push_back( pair<std::string,size_t>("AFz",1) );
    selection.push_back( pair<std::string,size_t>("AF8",2) );
    selection.push_back( pair<std::string,size_t>("F3",3) );
    selection.push_back( pair<std::string,size_t>("F1",4) );
    selection.push_back( pair<std::string,size_t>("Fz",5) );
    selection.push_back( pair<std::string,size_t>("F2",6) );
    selection.push_back( pair<std::string,size_t>("F4",7) );
    selection.push_back( pair<std::string,size_t>("FT7",8) );
    selection.push_back( pair<std::string,size_t>("FC5",9) );
    selection.push_back( pair<std::string,size_t>("FC3",10) );
    selection.push_back( pair<std::string,size_t>("FC1",11) );
    selection.push_back( pair<std::string,size_t>("FCz",12) );
    selection.push_back( pair<std::string,size_t>("FC2",13) );
    selection.push_back( pair<std::string,size_t>("FC4",14) );
    selection.push_back( pair<std::string,size_t>("FC6",15) );
    selection.push_back( pair<std::string,size_t>("FT8",16) );
    selection.push_back( pair<std::string,size_t>("C5",17) );
    selection.push_back( pair<std::string,size_t>("C3",18) );
    selection.push_back( pair<std::string,size_t>("C1",19) );
    selection.push_back( pair<std::string,size_t>("Cz",20) );
    selection.push_back( pair<std::string,size_t>("C2",21) );
    selection.push_back( pair<std::string,size_t>("C4",22) );
    selection.push_back( pair<std::string,size_t>("C6",23) );
    selection.push_back( pair<std::string,size_t>("CP5",24) );
    selection.push_back( pair<std::string,size_t>("CP3",25) );
    selection.push_back( pair<std::string,size_t>("CP1",26) );
    selection.push_back( pair<std::string,size_t>("CPz",27) );
    selection.push_back( pair<std::string,size_t>("CP2",28) );
    selection.push_back( pair<std::string,size_t>("CP4",29) );
    selection.push_back( pair<std::string,size_t>("CP6",30) );
    selection.push_back( pair<std::string,size_t>("P7",31) );
    selection.push_back( pair<std::string,size_t>("P3",32) );
    selection.push_back( pair<std::string,size_t>("Pz",33) );
    selection.push_back( pair<std::string,size_t>("P4",34) );
    selection.push_back( pair<std::string,size_t>("P8",35) );
    selection.push_back( pair<std::string,size_t>("PO3",36) );
    selection.push_back( pair<std::string,size_t>("POz",37) );
    selection.push_back( pair<std::string,size_t>("PO4",38) );
    selection.push_back( pair<std::string,size_t>("O1",39) );
    selection.push_back( pair<std::string,size_t>("Oz",40) );
    selection.push_back( pair<std::string,size_t>("O2",41) );
    selection.push_back( pair<std::string,size_t>("O9",42) );
    selection.push_back( pair<std::string,size_t>("Iz",43) );
    selection.push_back( pair<std::string,size_t>("O10",44) );*/

    //Electrodes el = Electrodes( selection );
    //Electrodes el;

    /*Cairo::RefPtr<Cairo::PdfSurface> surface =
            Cairo::PdfSurface::create( "hello.pdf", 100, 100);

    Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

    cr->scale( 50, 50 );
    cr->translate( 1, 1 );
    cr->scale( 0.8, -0.8 );

    print_head( cr );
    print_grid( cr );
    print_layout( cr, electrodes );

    surface->write_to_png( "hello.png" );

    mglGraph gr;
    gr.SetSize( 1000, 1000 );
    gr.SetAlphaDef( 1.0 );
    gr.SubPlot(1,1,0,"");
    print_head( gr );
    print_grid( gr );
    print_layout( gr, el, 0.07, 1.25 );
    gr.WriteEPS( "layout.eps" );
    gr.WritePNG( "layout.png" );*/

    /*vector<std::string> subjects;
    subjects.push_back( "BU9" );
    subjects.push_back( "BV3" );
    subjects.push_back( "BW2" );
    subjects.push_back( "BW3" );
    subjects.push_back( "BW4" );
    subjects.push_back( "BW6" );
    subjects.push_back( "BX5" );
    subjects.push_back( "BY3" );
    subjects.push_back( "BZ9" );
    subjects.push_back( "CA2" );
    subjects.push_back( "CA3" );
    subjects.push_back( "CA4" );
    subjects.push_back( "CA5" );
    subjects.push_back( "CA6" );

    vector<std::string> methods;
    methods.push_back( "BP" );
    methods.push_back( "S" );
    methods.push_back( "COH" );
    methods.push_back( "iCOH" );
    methods.push_back( "pCOH" );
    methods.push_back( "DTF" );
    methods.push_back( "PDC" );


    ifstream in;
    in.open( "selprop.dat" );

    for( size_t s=0; s<subjects.size(); s++ )
        for( size_t t=1; t<=2; t++ )
            for( size_t m=0; m<methods.size(); m++ )
            {
                cout << subjects[s] << "-" << t << "-" << methods[m] << ": ";
                cout << in.tellg( ) << " ... ";

                vector<double> p;
                p.resize( 45 );
                in.readsome( (char*)&p[0], sizeof(double)*45 );

                vector<double> weights = p;
                for( size_t i=0; i<45; i++ )
                {
                    weights[i] = weights[i] * 6.0;
                    cout << weights[i] << ", ";
                }
                cout << endl;

                stringstream ss;
                ss << "E = " << entropy( p );

                for( size_t i=0; i<45; i++ )
                {
                    weights[i] = 0.1 * sqrt( (pow( 1.5, weights[i] )-1) / pow(1.5, 6) );
                }

                mglGraph gr;
                gr.SetSize( 1000, 1000 );
                gr.SetAlphaDef( 1.0 );
                gr.SubPlot(1,1,0,"");
                print_head( gr );
                print_grid( gr );
                print_elweights( gr, el, weights );
                gr.Puts( -1.15, 1.05, ss.str().c_str(), ":L" );

                stringstream f;
                f << subjects[s] << "-" << t << "-" << methods[m] << ".eps";
                gr.WriteFrame( f.str().c_str() );

                system( ("epstopdf " +f.str()).c_str() );
                system( ("rm " +f.str()).c_str() );*/
/*            }

    in.close();*/

    return 0;
}

