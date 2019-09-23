#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

#include "image.h"
#include "misc.h"
#include "segment-image.h"

#include <iostream>

#define IMAGE "../images/image.ppm"
//#define IMAGE "../images/hall.jpg"
#define IMAGE2 "../images/output.ppm"

using namespace cv;
using namespace std;

Mat image2cv(image<rgb> * im)
{
    Mat cv_mat = Mat(im->height(), im->width(), CV_8UC3);
    std::memcpy(cv_mat.data, im->data, cv_mat.total()*sizeof(uchar[3]));
    cvtColor(cv_mat, cv_mat, CV_BGR2RGB);

    return cv_mat;
}

void cv2image(const Mat cv_mat, image<rgb> * imptr)
{
    Mat cp;
    cvtColor(cv_mat, cp, CV_BGR2RGB);
    std::memcpy(imptr->data, cv_mat.data, cv_mat.total()*sizeof(uchar[3]));
}

int main(int argc, char *argv[])
{
    Mat inputImage;
    inputImage = imread(IMAGE, CV_LOAD_IMAGE_COLOR);
    if (inputImage.type() != CV_8UC3)
        inputImage.convertTo(inputImage, CV_8UC3);

    image<rgb> *im = new image<rgb>(inputImage.cols, inputImage.rows);

    cv2image(inputImage, im);

    float sigma = 0.65;
    float k = 150;
    int min_size = 16500;
    int num_ccs;
    image<rgb> *seg = segment_image(im, sigma, k, 500, &num_ccs);

    Mat imageSegmented = image2cv(seg);

    Mat combinedImage;
    addWeighted(inputImage, 0.75, imageSegmented, 0.25, 0.0, combinedImage);

    imshow("first", combinedImage);
    waitKey(0);


    // add some trackable features
    vector<Point2f> points;
    cvtColor(inputImage, inputImage, CV_RGB2GRAY);
    TermCriteria termcrit(TermCriteria::COUNT+TermCriteria::EPS,20,0.03);
    goodFeaturesToTrack(inputImage, points, 300, 0.01, 10, Mat(), 3, 0, 0.04);

    for (int i(0);i < points.size();i++)
        circle(combinedImage, points.at(i), 1, Scalar(0,0,255), -1);

    imwrite(IMAGE2, combinedImage);

    return(0);
}
