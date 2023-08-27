#include "shape.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <Eigen/Eigen>

constexpr int WINDOW_WIDTH = 400;
constexpr int WINDOW_HEIGHT = 400;
constexpr int LINE_NUM = 20;
constexpr int RECTANGLE_WIDTH = 30;
constexpr int MAX_DISTANCE = 800;




std::unique_ptr<Eigen::MatrixXd> NDSort(std::unique_ptr<Eigen::MatrixXd> f1,
                                             std::unique_ptr<Eigen::MatrixXd> f2,
                                             std::unique_ptr<Eigen::MatrixXd> f3,
                                             std::unique_ptr<Eigen::MatrixXd> f4) {

  std::cout << "NDSort Start" << std::endl;
    std::unique_ptr<Eigen::MatrixXd> result = std::make_unique<Eigen::MatrixXd>(f1->rows(), f1->cols());
    for (int i = 0; i < f1->rows(); ++i) {
        for (int j = 0; j < f1->cols(); ++j) {
            double v1 = (*f1)(i, j);
            double v2 = (*f2)(i, j);
            double v3 = (*f3)(i, j);
            double v4 = (*f4)(i, j);
            int rank = 1;
            for (int k = 0; k < f1->rows(); ++k) {
                for (int l = 0; l < f1->cols(); ++l) {
                    if (k == i && l == j) {
                        continue;
                    }

                    double vk1 = (*f1)(k, l);
                    double vk2 = (*f2)(k, l);
                    double vk3 = (*f3)(k, l);
                    double vk4 = (*f4)(k, l);

                    if ((vk1 <= v1 && vk2 <= v2 && vk3 <= v3 && vk4 <= v4) &&
                        (vk1 < v1 || vk2 < v2 || vk3 < v3 || vk4 < v4)) {
                        ++rank;
                    }
                }
            }
            (*result)(i, j) = rank;
        }
    }
    std::cout << "NDSort End" << std::endl;
    return result;
}



int main(int argc, char *argv[]) {

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, std::min(WINDOW_WIDTH, WINDOW_HEIGHT));

  cv::Mat window = cv::Mat::zeros(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
  std::unique_ptr<Eigen::MatrixXd> circle_obj_distances = std::make_unique<Eigen::MatrixXd>(WINDOW_HEIGHT, WINDOW_WIDTH);
  std::unique_ptr<Eigen::MatrixXd> circle_tar_distances = std::make_unique<Eigen::MatrixXd>(WINDOW_HEIGHT, WINDOW_WIDTH);
  std::unique_ptr<Eigen::MatrixXd> rectangle_obj_distances = std::make_unique<Eigen::MatrixXd>(WINDOW_HEIGHT, WINDOW_WIDTH);
  std::unique_ptr<Eigen::MatrixXd> rectangle_tar_distances = std::make_unique<Eigen::MatrixXd>(WINDOW_HEIGHT, WINDOW_WIDTH);

  int x, y;
  for (int i = 0; i < LINE_NUM; ++i) {
    x = dis(gen);
    y = dis(gen);
    Line line(cv::Point(x, y),
              cv::Point(std::min(WINDOW_HEIGHT, dis(gen)),
                        std::min(WINDOW_WIDTH, dis(gen))),
              cv::Scalar(255, 157, 204));
    line.draw(window);
  }

  x = dis(gen);
  y = dis(gen);
  Rectangle rectangle_tar(cv::Point(x, y), RECTANGLE_WIDTH, RECTANGLE_WIDTH,
                       cv::Scalar(181, 231, 210));

  x = dis(gen);
  y = dis(gen);
  Rectangle rectangle_obj(cv::Point(x, y), RECTANGLE_WIDTH, RECTANGLE_WIDTH,
                       cv::Scalar(255, 187, 157));
 rectangle_tar.draw(window);
 rectangle_obj.draw(window);


  x = dis(gen);
  y = dis(gen);
  Circle circle_tar(cv::Point(x, y), 30, cv::Scalar(181, 231, 210));
  circle_tar.draw(window);

  x = dis(gen);
  y = dis(gen);
  Circle circle_obj(cv::Point(x, y), 30, cv::Scalar(255, 187, 157));


  // 计算window上的每个像素到圆的距离

  for (int i = 0; i < WINDOW_HEIGHT; ++i) {
    for (int j = 0; j < WINDOW_WIDTH; ++j) {
      cv::Vec3b pixel_color = window.at<cv::Vec3b>(i, j);
      if (pixel_color[0] == 0 && pixel_color[1] == 0 && pixel_color[2] == 0) {
      (*circle_obj_distances)(i, j) = circle_obj.distance(cv::Point(i,j));
      (*circle_tar_distances)(i, j) = circle_tar.distance(cv::Point(i,j));
      (*rectangle_obj_distances)(i, j) = circle_obj.distance(cv::Point(i,j));
      (*rectangle_tar_distances)(i, j) = circle_tar.distance(cv::Point(i,j));
      //float weight_sum_distance = 0.25*(*circle_obj_distances)(i, j) + 0.25*(*circle_tar_distances)(i, j) + 0.25*(*rectangle_obj_distances)(i, j) + 0.25*(*rectangle_tar_distances)(i, j);
      //uchar color = static_cast<uchar>(255* weight_sum_distance / MAX_DISTANCE);
      //window.at<cv::Vec3b>(i, j) = color;
      }
    }
  }
  std::unique_ptr<Eigen::MatrixXd> ranks = NDSort(
    std::move(circle_obj_distances), 
    std::move(circle_tar_distances), 
    std::move(rectangle_obj_distances), 
    std::move(rectangle_tar_distances)
);
  // 根据rank值进行着色
  for (int i = 0; i < WINDOW_HEIGHT; ++i) {
    for (int j = 0; j < WINDOW_WIDTH; ++j) {
      cv::Vec3b pixel_color = window.at<cv::Vec3b>(i, j);
      if (pixel_color[0] == 0 && pixel_color[1] == 0 && pixel_color[2] == 0) {
        if((*ranks)(i,j) <= 20){
          window.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 144, 255);
        }
      }
    }
  }


  std::cout << *ranks << std::endl;

  



  circle_obj.draw(window);


  cv::namedWindow("demo", cv::WINDOW_AUTOSIZE);

  cv::imshow("demo", window);

  cv::waitKey(0);
}

