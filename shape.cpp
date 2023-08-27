#include "shape.hpp"
#include <algorithm>

float Shape::distance(const cv::Point point) const {
  return cv::norm(position - point);
}

float Circle::distance(const cv::Point point) const {
  return std::abs(cv::norm(position - point) - radius);
}

float Rectangle::distance(const cv::Point point) const {
  cv::Point nearest_point;
  nearest_point.x = std::max(position.x, std::min(point.x, position.x + width));
  nearest_point.y = std::max(position.y, std::min(point.y, position.y + height));
  return cv::norm(nearest_point - point);
}

