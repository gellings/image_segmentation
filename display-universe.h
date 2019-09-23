#ifndef DISPLAY_UNI
#define DISPLAY_UNI

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

#include "image.h"
#include "imutil.h"
#include "disjoint-set.h"

using namespace cv;

// random color
rgb random_rgb(){
  rgb c;
  double r;

  c.r = (uchar)random();
  c.g = (uchar)random();
  c.b = (uchar)random();

  return c;
}

Mat image2cv(image<rgb> * im)
{
    Mat cv_mat = Mat(im->height(), im->width(), CV_8UC3);
    std::memcpy(cv_mat.data, im->data, cv_mat.total()*sizeof(uchar[3]));
    cvtColor(cv_mat, cv_mat, CV_BGR2RGB);

    return cv_mat;
}

class displayer {
public:
    displayer();
    ~displayer();
    void display_intermediate(universe *u);

private:
    rgb *colors;
    int width;
    int height;

};

displayer::displayer() :
    width(640),
    height(480)
{
    // pick random colors for each component
    colors = new rgb[width*height];
    for (int i = 0; i < width*height; i++)
      colors[i] = random_rgb();
}

displayer::~displayer()
{
    delete colors;
}


void displayer::display_intermediate(universe *u)
{
    image<rgb> *output = new image<rgb>(width, height);

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int comp = u->find(y * width + x);
        imRef(output, x, y) = colors[comp];
      }
    }

    imshow("here", image2cv(output));
    waitKey(0);
}

#endif
