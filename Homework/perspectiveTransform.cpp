//
// Created by mustang on 2020/10/4.
// Homework: perspective transform
//
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;

void doPerspectiveTransform( Mat input, Mat& output ) {
    std::vector<Point2f> srcQuad( 4 ), dstQuad( 4 );
    output = input.clone();
    srcQuad[0].x = 605, srcQuad[1].x = 771, srcQuad[2].x = 598, srcQuad[3].x = 766;
    srcQuad[0].y = 387, srcQuad[1].y = 388, srcQuad[2].y = 331, srcQuad[3].y = 331;
    dstQuad[0].x = 0, dstQuad[1].x = 770, dstQuad[2].x = 0, dstQuad[3].x = 770;
    dstQuad[0].y = dstQuad[1].y = 390, dstQuad[2].y = dstQuad[3].y = 0;

    Mat warp_matrix = getPerspectiveTransform( srcQuad, dstQuad );

    warpPerspective( input, output, warp_matrix, Size( 770, 390 ) );
}


int main() {
    Mat img = imread("../car.jpg");
    Mat result;

    doPerspectiveTransform( img, result );

    namedWindow( "input" );
    namedWindow( "output" );

    imshow( "input", img );
    imshow( "output", result );

    waitKey( 0 );

    destroyWindow( "input" );
    destroyWindow( "output" );
    return 0;
}
