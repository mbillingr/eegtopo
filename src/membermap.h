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

#ifndef MEMBERMAP_H
#define MEMBERMAP_H

#include "colormap.h"

#include <boost/variant.hpp>
#include <map>
#include <string>
#include <stdexcept>

class MemberMap
{
public:
    typedef std::map< std::string, boost::variant<bool, int, double, Color<double>, std::string> > container_type;
    typedef container_type::value_type value_type;

    MemberMap();

    template <typename T>
    T& operator[]( const std::string& var )
    {
        return members[var];
    }

    template <typename T>
    T get( const std::string& var ) const
    {
        container_type::const_iterator it = members.find( var );
        if( it == members.end() )
            throw std::invalid_argument( "Unknown member: " + var );
        return boost::get<T>( it->second );
    }

    template <typename T>
    void set( const std::string& var, const T& val )
    {
        members[var] = val;
    }

    container_type::const_iterator members_begin( ) const { return members.begin(); }
    container_type::const_iterator members_end( ) const { return members.end(); }

private:
    container_type members;
};

#endif // MEMBERMAP_H
