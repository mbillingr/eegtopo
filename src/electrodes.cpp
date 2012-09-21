#include "electrodes.h"

#include <cmath>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <iostream>

Electrodes::Electrodes( std::vector<std::string> labels )
{
    calc_positions( );
    container subset;

    std::vector<std::string>::iterator it = labels.begin();
    for( size_t idx=0 ; it!=labels.end(); it++, idx++ )
    {
        subset[*it] = get( *it );
        m_idx_label[idx] = *it;
        m_idx_pos[idx] = get( *it );
    }

    m_elcache = subset;

    calc_mindist( );
}

Electrodes::Electrodes( std::vector< std::pair<std::string,size_t> > labels )
{
    calc_positions( );
    container subset;

    std::vector< std::pair<std::string,size_t> >::iterator it = labels.begin();
    for( ; it!=labels.end(); it++ )
    {
        subset[it->first] = get( it->first );
        m_idx_label[it->second] = it->first;
        m_idx_pos[it->second] = get(it->first );
    }

    m_elcache = subset;

    calc_mindist( );
}

Electrodes::Electrodes()
{
    calc_positions( );    
    calc_mindist( );
}

Electrodes::~Electrodes()
{
}

bool Electrodes::get( const std::string &label , point_t *info) const
{
    const_iterator it = m_elcache.find( label );
    if( it == m_elcache.end() )
        return false;
    *info = it->second;
    return true;
}

Electrodes::point_t  Electrodes::get(const std::string &label ) const
{
    const_iterator it = m_elcache.find( label );
    if( it == m_elcache.end() )
        throw std::runtime_error( label+" not defined." );
    return it->second;
}

double Electrodes::get_distance( const std::string &a, const std::string &b )
{
    const_iterator it1 = m_elcache.find( a );
    const_iterator it2 = m_elcache.find( b );

    if( it1 == m_elcache.end() )
        throw std::runtime_error( a+" not defined." );

    if( it2 == m_elcache.end() )
        throw std::runtime_error( b+" not defined." );

    double dx = (it1->second.x - it2->second.x);
    double dy = (it1->second.y - it2->second.y);

    return sqrt( dx*dx + dy*dy );
}

double Electrodes::get_mindist( ) const
{
    return mindist;
}

Electrodes::point_t Electrodes::get_interpolated( const std::string &a, const std::string &b, const std::string &c, const std::string &d ) const
{
    point_t x(0,0);
    point_t y = get(a);
    x.x += y.x / 4.0;
    x.y += y.y / 4.0;
    y = get(b);
    x.x += y.x / 4.0;
    x.y += y.y / 4.0;
    y = get(c);
    x.x += y.x / 4.0;
    x.y += y.y / 4.0;
    y = get(d);
    x.x += y.x / 4.0;
    x.y += y.y / 4.0;
    return x;
}

Electrodes::point_t Electrodes::get_interpolated( const std::string &a, const std::string &b, const std::string &c ) const
{
    point_t x(0,0);
    point_t y = get(a);
    x.x += y.x / 3.0;
    x.y += y.y / 3.0;
    y = get(b);
    x.x += y.x / 3.0;
    x.y += y.y / 3.0;
    y = get(c);
    x.x += y.x / 3.0;
    x.y += y.y / 3.0;
    return x;
}

std::string Electrodes::label( const std::string &a, int b )
{
    std::string s = a;
    // C8 -> T8, CP7 -> TP7, FC10 -> FT10, etc...
    // but CFC8 -> CFC8, ...
    if( b >= 7 && a.length() <= 2 )
    {
        if( s[0] == 'C' ) s[0] = 'T';
        if( s[1] == 'C' ) s[1] = 'T';
    }
    else if( b >= 9 && a.length() <= 3 )
    {
        if( s[0] == 'C' ) s[0] = 'T';
        if( s[1] == 'C' ) s[1] = 'T';
        if( s[2] == 'C' ) s[2] = 'T';
    }

    if( b==0 )
        return s + 'z';

    std::stringstream ss;
    ss << b;

    return s + ss.str();
}

void Electrodes::set_position( const std::string &a, int b, double x, double y, bool mirror )
{
    if( b==0 )
        m_elcache[label(a,b)] = point_t(x,y);
    else
    {
        m_elcache[label(a,b)] = point_t(x,y);
        if( mirror )
        {
            if( b%2 == 0 )
                b -= 1;
            else
                b += 1;
            set_position( a, b, -x, y, false );
        }
    }
}

void Electrodes::set_position(const std::string &a, int b, Electrodes::point_t info, bool mirror)
{
    set_position( a, b, info.x, info.y, mirror );
}

void Electrodes::set_intersected_position(const std::string &a, int b, bool mirror, double d)
{
    point_t ca;
    if( !get( label(a,0), &ca ) )
        throw std::runtime_error( label(a,0)+" not defined." );

    point_t cb;
    if( !get( label(a,8), &cb ) )
        throw std::runtime_error( label(a,8)+" not defined." );

    point_t cc;
    if( !get( label(a,7), &cc ) )
        throw std::runtime_error( label(a,7)+" not defined." );

    circle_t C0( ca, cb, cc );
    circle_t C1( point_t(0,d), point_t(0,-d), point_t(0.1*b,0) );

    set_position( a, b, C0.intersect(C1), mirror );
}

void Electrodes::set_intersected_position(const std::string &a, int b, double c, point_t ca, point_t cb, bool mirror, double d)
{
    point_t cc = cb;
    cc.x = -cc.x;
    circle_t C0( ca, cb, cc );
    circle_t C1( point_t(0,d), point_t(0,-d), point_t(c,0) );

    set_position( a, b, C0.intersect(C1), mirror );
}

void Electrodes::add_position( const std::string &label, double x, double y )
{
    const_iterator it = m_elcache.find( label );
    if( it != m_elcache.end() )
        throw std::runtime_error( "Position " + label + " already exists." );
    m_elcache[label] = point_t( x, y );
}

void Electrodes::calc_positions( )
{
    // coronal positions
    m_elcache["AFz"] = point_t( 0, 0.6 );
    m_elcache["Fz"] = point_t( 0, 0.4 );
    m_elcache["FCz"] = point_t( 0, 0.2 );
    m_elcache["CPz"] = point_t( 0, -0.2 );
    m_elcache["Pz"] = point_t( 0, -0.4 );
    m_elcache["POz"] = point_t( 0, -0.6 );
    m_elcache["Oz"] = point_t( 0, -0.8 );
    m_elcache["Iz"] = point_t( 0, -1.0 );

    // sagittal positions
    for( int c=0; c<=6; c+=2 )
        set_position( "C", c, c*0.1, 0, true );

    // inner circle electrodes
    set_position( "Fp", 0, pol2cart(0.8, 0.0), true );
    set_position( "Fp", 2, pol2cart(0.8, 0.1), true );
    set_position( "AF", 8, pol2cart(0.8, 0.2), true );
    set_position( "F", 8, pol2cart(0.8, 0.3), true );
    set_position( "FT", 8, pol2cart(0.8, 0.4), true );
    set_position( "T", 8, pol2cart(0.8, 0.5), true );
    set_position( "TP", 8, pol2cart(0.8, 0.6), true );
    set_position( "P", 8, pol2cart(0.8, 0.7), true );
    set_position( "PO", 8, pol2cart(0.8, 0.8), true );
    set_position( "O", 2, pol2cart(0.8, 0.9), true );

    // constructed positions
    int C[] = {2,4,6};
    std::string S[] = {"F", "FC", "CP", "P"};
    for( size_t s=0; s<4; s++)
    {
        for( size_t c=0; c<3; c++ )
        {
            set_intersected_position( S[s], C[c], true );
        }
        set_intersected_position( S[s], 10, true, 1.0 );
    }

    // remaining outer circle (skirt) electrodes
    set_position( "PO", 10, pol2cart(1.0, 0.8), true );
    set_position( "O", 10, pol2cart(1.0, 0.9), true );

    // manual (adjusted) positions
    set_intersected_position( "AF", 4, true );
    m_elcache["AF4"].x += 0.05;
    m_elcache["AF4"].y -= 0.01;
    m_elcache["AF3"].x -= 0.05;
    m_elcache["AF3"].y -= 0.01;
    set_intersected_position( "PO", 4, true );
    m_elcache["PO4"].x += 0.02;
    m_elcache["PO4"].y += 0.01;
    m_elcache["PO3"].x -= 0.02;
    m_elcache["PO3"].y += 0.01;

    // constructed ultra-extended positions
    for( int c=1; c<=9; c+= 2)
    {
        set_intersected_position( "FFC", c+1, 0.1*c, point_t(0,0.3), pol2cart(0.8,0.35), true );
        set_intersected_position( "CFC", c+1, 0.1*c, point_t(0,0.1), pol2cart(0.8,0.45), true );
        set_intersected_position( "CCP", c+1, 0.1*c, point_t(0,-0.1), pol2cart(0.8,0.55), true );
        set_intersected_position( "PCP", c+1, 0.1*c, point_t(0,-0.3), pol2cart(0.8,0.65), true );
    }
    set_intersected_position( "PPO", 2, 0.15, point_t(0,-0.5), pol2cart(0.8,0.75), true );
    set_intersected_position( "PPO", 6, 0.55, point_t(0,-0.5), pol2cart(0.8,0.75), true );

    // interpolated ultra-extended positions
    set_position( "AFp", 2, get_interpolated( "Fpz", "Fp2", "AFz", "AF4" ), true );
    set_position( "FAF", 2, get_interpolated( "AFz", "AF4", "F2" ), true );
    set_position( "FAF", 6, get_interpolated( "AF4", "AF8", "F4", "F6" ), true );
    set_position( "PPO", 10, get_interpolated( "P8", "P10", "PO8", "PO10" ), true );
    set_position( "OPO", 2, get_interpolated( "POz", "PO4", "Oz", "O2" ), true );
    set_position( "OPO", 10, get_interpolated( "O2", "PO8", "PO10", "O10" ), true );
    set_position( "OI", 2, get_interpolated( "Oz", "O2", "Iz", "O10" ), true );

    // unset mindist;
    mindist = -1;
}

void Electrodes::calc_mindist( )
{
    mindist = 1.0 / 0.0;
    const_iterator it1 = m_elcache.begin( );
    for( ; it1 != m_elcache.end(); it1++ )
    {
        const_iterator it2 = it1; it2++;
        for( ; it2 != m_elcache.end(); it2++ )
        {
            double dx = (it1->second.x - it2->second.x);
            double dy = (it1->second.y - it2->second.y);
            mindist = std::min( mindist, dx*dx + dy*dy );
        }
    }

    mindist = sqrt( mindist );
}

Electrodes::point_t Electrodes::pol2cart( double r, double alpha )
{
    double x = r * cos( (0.5-alpha)*M_PI );
    double y = r * sin( (0.5-alpha)*M_PI );
    return point_t(x,y);
}

Electrodes::circle_t::circle_t( Electrodes::point_t A, Electrodes::point_t B, Electrodes::point_t C)
{
    if( B.x-A.x == 0 )
    {
        double s2 = (C.y-B.y) / (C.x-B.x);
        y = (B.y+A.y) * 0.5;
        x = ((C.y + B.y - 2.0*y) * s2  + C.x + B.x) * 0.5;
    }
    else if( C.x-B.x == 0 )
    {
        double s1 = (B.y-A.y) / (B.x-A.x);
        y = (C.y+B.y) * 0.5;
        x = ((A.y + B.y - 2.0*y) * s1  + A.x + B.x) * 0.5;
    }
    else
    {
        double s1 = (B.y-A.y) / (B.x-A.x);
        double s2 = (C.y-B.y) / (C.x-B.x);
        x = (s1*s2*(A.y-C.y) + s2*(A.x+B.x) - s1*(B.x+C.x)) / (2*(s2-s1));
        if( s1==0 )
            y = (C.y+B.y)*0.5 - (x - (C.x+B.x)*0.5)/s2;
        else
            y = (A.y+B.y)*0.5 - (x - (A.x+B.x)*0.5)/s1;
    }
    r = sqrt( (A.x-x)*(A.x-x) + (A.y-y)*(A.y-y) );
}

Electrodes::point_t Electrodes::circle_t::intersect( const Electrodes::circle_t &other )
{
    double d2 = (x-other.x) * (x-other.x) + (y-other.y) * (y-other.y);
    double d = sqrt(d2);

    double a = (r*r - other.r*other.r + d2) / (2*d);
    double h = sqrt( r*r - a*a );

    double ex =  h * (other.y-y) / d;
    double ey = -h * (other.x-x) / d;

    point_t p;
    p.x = x + a *(other.x-x)/d;
    p.y = y + a *(other.y-y)/d;

    // only return the intersection with positive x
    if( p.x+ex > 0 )
    {
        p.x = p.x + ex;
        p.y = p.y + ey;
    }
    else
    {
        p.x = p.x - ex;
        p.y = p.y - ey;
    }

    return p;
}
