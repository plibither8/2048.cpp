#ifndef POINT_2D_H
#define POINT_2D_H

#include <tuple>

class point2D_t {
  // Simple {x,y} datastructure = std::tuple<int, int>...
  using point_datatype_t = typename std::tuple<int, int>;
  point_datatype_t point_vector{};
  explicit point2D_t(const point_datatype_t pt) : point_vector{pt} {}

public:
  enum class PointCoord { COORD_X, COORD_Y };

  point2D_t() = default;
  point2D_t(const int x, const int y) : point2D_t(std::make_tuple(x, y)) {}

  template<PointCoord dimension>
  int get() const {
    return std::get<static_cast<int>(dimension)>(point_vector);
  }
  template<PointCoord dimension>
  void set(int value) {
    std::get<static_cast<int>(dimension)>(point_vector) = value;
  }
  point_datatype_t get() const { return point_vector; }
  void set(point_datatype_t value) { point_vector = value; }

  void set(const int x, const int y) { set(std::make_tuple(x, y)); }

  point2D_t &operator+=(const point2D_t &pt) {
    this->point_vector = std::make_tuple(
        get<PointCoord::COORD_X>() + pt.get<PointCoord::COORD_X>(),
        get<PointCoord::COORD_Y>() + pt.get<PointCoord::COORD_Y>());
    return *this;
  }

  point2D_t &operator-=(const point2D_t &pt) {
    this->point_vector = std::make_tuple(
        get<PointCoord::COORD_X>() - pt.get<PointCoord::COORD_X>(),
        get<PointCoord::COORD_Y>() - pt.get<PointCoord::COORD_Y>());
    return *this;
  }
};

inline point2D_t operator+(point2D_t l, const point2D_t &r) {
  l += r;
  return l;
}

inline point2D_t operator-(point2D_t l, const point2D_t &r) {
  l -= r;
  return l;
}

#endif
