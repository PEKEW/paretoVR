
#ifndef DEMO_SHAPE_HPP
#define DEMO_SHAPE_HPP

#include <opencv2/opencv.hpp>


class Shape {
public:
  Shape() = default;
  Shape(const Shape &) = default;
  virtual ~Shape() = default;
  virtual void draw(cv::Mat &window) const = 0;
  cv::Point position;
  cv::Scalar color;

  float distance(const cv::Point) const;
};

class Line : public Shape {

public:
  cv::Point end;
  Line(const cv::Point &position, const cv::Point &end, const cv::Scalar &color) {
    this->position = position;
    this->end = end;
    this->color = color;
  }
  void draw(cv::Mat &window) const override {
    cv::line(window, position, end, color, 2);
  }
};

class Rectangle : public Shape {

public:
  int width;
  int height;
  Rectangle(const cv::Point &position, int width, int height,
            const cv::Scalar &color) {
    this->position = position;
    this->width = width;
    this->height = height;
    this->color = color;
  }
  void draw(cv::Mat &window) const override {
    cv::rectangle(window, position, position + cv::Point(width, height),
                  color, -1);
  }

  float distance(const cv::Point) const;
};

class Circle : public Shape {
public:
  int radius;
  Circle(const cv::Point &position, int radius, const cv::Scalar &color) {
    this->position = position;
    this->radius = radius;
    this->color = color;
  }
  void draw(cv::Mat &window) const override {
    cv::circle(window, position, radius,color, -1);
  }
  float distance(const cv::Point) const;
};

#endif // DEMO_SHAPE_HPP
