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

#include "electrodes.h"

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

#include <stdexcept>

BOOST_FUSION_ADAPT_STRUCT(
        Color<double>,
        (double, r)
        (double, g)
        (double, b)
)

namespace client
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    template <typename T>
    void set_electrode( Painter *painter, const boost::fusion::vector<std::string, std::pair<std::string, T> > &v )
    {
        using boost::fusion::at_c;
        painter->set_electrode( at_c<0>(v),  at_c<1>(v).first, at_c<1>(v).second );
    }

    template <typename T>
    void set_group( Painter *painter, const boost::fusion::vector<std::string, std::pair<std::string, T> > &v )
    {
        using boost::fusion::at_c;
        painter->set_group( at_c<0>(v),  at_c<1>(v).first, at_c<1>(v).second );
    }

    void set_painter( Painter *painter, const boost::fusion::vector2<std::string, double> &v )
    {
        painter->set( v.m0, v.m1 );
    }

    void define( Painter *painter, const boost::fusion::vector<std::string,std::vector<std::string> > &v )
    {
        using boost::fusion::at_c;
        painter->define_group( at_c<0>(v), at_c<1>(v) );
    }

    struct electrodes_ : qi::symbols<char, std::string>
    {
        electrodes_()
        {
            Electrodes el0;
            Electrodes::const_iterator it = el0.begin();
            for( ; it!=el0.end(); it++ )
            {
                std::string lo = it->first;
                for( size_t i=0; i<lo.size(); i++ )
                    lo[i] = std::tolower( lo[i] );
                add( lo, it->first );
            }
            add( "n", "N" );
            add( "el", "El" );
            add( "er", "Er" );
        }
    } electrodes;

    struct pvars_ : qi::symbols<char, std::string>
    {
        pvars_()
        {
            // create dummy context
            Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create( Cairo::ImageSurface::create( Cairo::FORMAT_A1, 0, 0 ) );

            Painter p( cr );
            Painter::container_type::const_iterator it = p.members_begin();
            for( ; it != p.members_end(); it++ )
            {
                add( it->first, it->first );
            }
        }
    } pvars;

    struct intvars_ : qi::symbols<char, std::string>
    {
        intvars_()
        {
            add( "font_slant", "font_slant" );
            add( "font_weight", "font_weight" );
        }
    } intvars;

    struct colvars_ : qi::symbols<char, std::string>
    {
        colvars_()
        {
            add( "line_color", "line" );
            add( "fill_color", "fill" );
            add( "font_color", "font" );
        }
    } colvars;

    struct dblvars_ : qi::symbols<char, std::string>
    {
        dblvars_()
        {
            add( "radius", "radius" );
            add( "pos_x", "x" );
            add( "pos_y", "y" );
            add( "line_width", "line_width" );
            add( "value", "value" );
        }
    } dblvars;

    struct strvars_ : qi::symbols<char, std::string>
    {
        strvars_()
        {
            add( "label", "label" );
            add( "font_family", "font_family" );
        }
    } strvars;

    struct boolvars_ : qi::symbols<char, std::string>
    {
        boolvars_()
        {
            add( "visible", "visible" );
            add( "show_label", "show_label" );
        }
    } boolvars;

    template <typename Iterator>
    struct eegtopo : qi::grammar<Iterator, ascii::space_type>
    {
        eegtopo( Painter &painter ) : eegtopo::base_type(start)
        {
            using qi::_val;
            using qi::lit;
            //using qi::_1;
            //using qi::_2;
            using qi::lexeme;
            using ascii::char_;
            using ascii::no_case;
            using qi::double_;
            using qi::bool_;
            using qi::int_;
            using phoenix::push_back;

            start = *(cmd_draw_head | cmd_draw_grid | cmd_draw_chan | cmd_draw_topo | cmd_define | cmd_set_painter | cmd_setcol | cmd_setint | cmd_setdbl | cmd_setstr | cmd_setbool);

            cmd_draw_head = no_case["draw_head"] [boost::bind(&Painter::draw_head,&painter)] >> ';';
            cmd_draw_grid = ( no_case["draw_grid"] >> ';' ) [boost::bind(&Painter::draw_grid,&painter,2)] |
                            ( no_case["draw_grid_2"] >> ';' ) [boost::bind(&Painter::draw_grid,&painter,2)] |
                            ( no_case["draw_grid_1"] >> ';' ) [boost::bind(&Painter::draw_grid,&painter,1)] |
                            ( no_case["draw_grid_0"] >> ';' ) [boost::bind(&Painter::draw_grid,&painter,0)];
            cmd_draw_chan = no_case["draw_electrodes"] [boost::bind(&Painter::draw_layout,&painter)] >> ';';
            cmd_draw_topo = no_case["draw_topo"] [boost::bind(&Painter::draw_topo,&painter)] >> ';';

            cmd_set_painter = ( no_case["set"] >> no_case[pvars] >> '=' >> double_ >> ';' ) [boost::bind(&set_painter, &painter, _1)];

            cmd_setint = ( no_case["set"] >> (electrode) >> '.' >> intvar >> ';' ) [boost::bind(&set_electrode<int>, &painter, _1)]
                       | ( no_case["set"] >> (groupname) >> '.' >> intvar >> ';' ) [boost::bind(&set_group<int>, &painter, _1)];

            cmd_setdbl = ( no_case["set"] >> (electrode) >> '.' >> dblvar >> ';' ) [boost::bind(&set_electrode<double>, &painter, _1)]
                       | ( no_case["set"] >> (groupname) >> '.' >> dblvar >> ';' ) [boost::bind(&set_group<double>, &painter, _1)];

            cmd_setbool = ( no_case["set"] >> (electrode) >> '.' >> boolvar >> ';' ) [boost::bind(&set_electrode<bool>, &painter, _1)]
                        | ( no_case["set"] >> (groupname) >> '.' >> boolvar >> ';' ) [boost::bind(&set_group<bool>, &painter, _1)];

            cmd_setstr = ( no_case["set"] >> (electrode) >> '.' >> strvar >> ';' ) [boost::bind(&set_electrode<std::string>, &painter, _1)]
                       | ( no_case["set"] >> (groupname) >> '.' >> strvar >> ';' ) [boost::bind(&set_group<std::string>, &painter, _1)];

            cmd_setcol = ( no_case["set"] >> (electrode) >> '.' >> colvar >> ';' ) [boost::bind(&set_electrode<Color<double> >, &painter, _1)]
                       | ( no_case["set"] >> (groupname) >> '.' >> colvar >> ';' ) [boost::bind(&set_group<Color<double> >, &painter, _1)];

            cmd_define = (no_case["define"] >> groupname >> ':' >> el_list  >> ';') [boost::bind(&define, &painter, _1)];

            intvar = no_case[intvars] >> '=' >> int_;
            dblvar = no_case[dblvars] >> '=' >> double_;
            strvar = no_case[strvars] >> '=' >> lexeme['"' >> +(char_ - '"') >> '"'];
            boolvar = no_case[boolvars] >> '=' >> bool_;
            //colvar = no_case[colvars] >> '=' >> lit('[') >> double_ >> ',' >> double_ >> ',' >> double_ >> lit(']');
            colvar = no_case[colvars] >> '=' >> lit('[') >> (double_ % ',') >> lit(']');

            groupname = lexeme['"' >> +(char_ - '"') >> '"'];

            el_list = electrode [push_back(_val, qi::_1)] % ',';
            electrode = no_case[electrodes];

        }
        qi::rule<Iterator, ascii::space_type> start;

        qi::rule<Iterator, ascii::space_type> cmd_draw_head;
        qi::rule<Iterator, ascii::space_type> cmd_draw_grid;
        qi::rule<Iterator, ascii::space_type> cmd_draw_chan;
        qi::rule<Iterator, ascii::space_type> cmd_draw_topo;

        qi::rule<Iterator, ascii::space_type> cmd_set_painter;

        qi::rule<Iterator, ascii::space_type> cmd_setint;
        qi::rule<Iterator, ascii::space_type> cmd_setdbl;
        qi::rule<Iterator, ascii::space_type> cmd_setstr;
        qi::rule<Iterator, ascii::space_type> cmd_setcol;
        qi::rule<Iterator, ascii::space_type> cmd_setbool;
        qi::rule<Iterator, ascii::space_type> cmd_define;

        qi::rule<Iterator, std::pair<std::string, int>(), ascii::space_type> intvar;
        qi::rule<Iterator, std::pair<std::string, double>(), ascii::space_type> dblvar;
        qi::rule<Iterator, std::pair<std::string, bool>(), ascii::space_type> boolvar;
        qi::rule<Iterator, std::pair<std::string, std::vector<double> >(), ascii::space_type> colvar;
        qi::rule<Iterator, std::pair<std::string, std::string>(), ascii::space_type> strvar;
        qi::rule<Iterator, std::vector<std::string>(), ascii::space_type> el_list;
        qi::rule<Iterator, std::string(), ascii::space_type> electrode, groupname;

    };

}

Parser::Parser()
{
}

void Parser::parse(const std::string &line, Painter &painter )
{
    client::eegtopo<std::string::const_iterator> grammar( painter );

    using boost::spirit::ascii::space;

    std::string::const_iterator iter = line.begin();
    std::string::const_iterator end = line.end();

    bool r = phrase_parse(iter, end, grammar, space );

    if (r && iter == end)
    {
        // parsing succeeded...
    }
    else
    {
        std::string rest(iter, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \": " << rest << "\"\n";
        std::cout << "-------------------------\n";
        throw std::runtime_error( "Parser Error." );
    }
}

void Parser::parse_file(const std::string &filename, Painter &painter )
{
    std::ifstream fin( filename.c_str() );

    std::string str((std::istreambuf_iterator<char>(fin)),
                     std::istreambuf_iterator<char>());

    parse( str, painter );

    fin.close( );
}
