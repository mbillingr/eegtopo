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
