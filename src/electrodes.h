#ifndef ELECTRODES_H
#define ELECTRODES_H

#include <map>
#include <string>
#include <vector>

class Electrodes
{
public:
    Electrodes();
    Electrodes( std::vector<std::string> labels );
    Electrodes( std::vector< std::pair<std::string,size_t> > labels );
    virtual ~Electrodes( );

    struct point_t
    {
        point_t( double X=0, double Y=0 ) : x(X), y(Y) { }
        static point_t mirror( point_t a ) { return point_t(-a.x,a.y); }
        double x, y;
    };

    struct circle_t
    {
        circle_t( double X, double Y, double R ) : x(X), y(Y), r(R) { }
        circle_t( point_t A, point_t B, point_t C);
        point_t intersect( const circle_t &other );
        double x, y, r;
    };

    typedef std::map<std::string,point_t> container;
    typedef container::const_iterator const_iterator;

    bool get(const std::string &label, point_t *info ) const;
    point_t get(const std::string &label ) const;

    void add_position( const std::string &label, double x, double y );

    point_t operator[]( size_t idx ) const { return m_idx_pos.find(idx)->second; }

    double get_distance( const std::string &a, const std::string &b );
    double get_mindist( ) const;

    point_t get_interpolated( const std::string &a, const std::string &b, const std::string &c, const std::string &d ) const;
    point_t get_interpolated( const std::string &a, const std::string &b, const std::string &c ) const;

    std::string label( const std::string &a, int b );

    const_iterator begin( ) const { return m_elcache.begin(); }
    const_iterator end( ) const { return m_elcache.end(); }
    size_t size() const { return m_elcache.size(); }

    void set_position(const std::string &a, int b, double x , double y, bool mirror=false);
    void set_position(const std::string &a, int b, point_t info, bool mirror=false);
    void set_intersected_position(const std::string &a, int b, bool mirror=false, double d=0.8);
    void set_intersected_position(const std::string &a, int b, double c, point_t ca, point_t cb, bool mirror=false, double d=0.8);

    void calc_positions( );
    void calc_mindist( );

private:
    container m_elcache;
    std::map<size_t,std::string> m_idx_label;
    std::map<size_t,point_t> m_idx_pos;

    double mindist;

private:
    point_t pol2cart(double r, double alpha );
};

#endif // ELECTRODES_H
