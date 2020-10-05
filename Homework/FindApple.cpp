//
// Created by mustang on 2020/10/5.
// Homework: find the apple
//
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
void findApple( Mat input ) {
    Mat img;
    pyrDown( input, img, Size( input.cols / 2, input.rows / 2 ) );  //缩小原图方便处理
    Mat channel[3], gray_img, canny_img, threshold_img            ;                     //channel 储存分离后的BGR三通道
                                                                                        //gray_img 灰度化的图像
                                                                                        //canny_img 图像轮廓，但是事实上并没有用到
                                                                                        //threshold_img 自适应二值化得到的矩阵
    Mat B_channel, G_channel, R_channel;                                                //BGR 三通道分离矩阵

    std::vector<std::vector<Point>> contours;                                           //储存轮廓

    split( input, channel );
    subtract( channel[2], channel[1], R_channel );
    subtract( channel[0], channel[1], B_channel );                             //由于考虑到苹果的G通道值较小，所以将R和B通道减去G通道
                                                                                        //以此使得G通道较高的叶子区域被筛选出来
    channel[1] = Mat::zeros( input.rows, input.cols, CV_8UC1 );
    G_channel = channel[1].clone();
    threshold( R_channel, channel[2], 30, 255, THRESH_TOZERO );
    threshold( B_channel, channel[0], 0, 255, THRESH_TOZERO );    //剔除B和R通道较小的像素点，也就是可能的叶子部分

    merge( channel, 3, img );


    cvtColor( img, gray_img, COLOR_BGR2GRAY );
    erode( gray_img, gray_img, Mat());
    erode( gray_img, gray_img, Mat());                                           //此处乃玄学操作，针对这一图片，通过两次侵蚀可以让发图像边缘的噪点被清除
    adaptiveThreshold( gray_img, threshold_img, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 21, 5 );

    dilate( threshold_img, threshold_img, Mat());
    dilate( threshold_img, threshold_img, Mat());                                //做两次扩张，使得图像上不连续的点连接起来

    Canny( threshold_img, canny_img, 5, 30 );                       //这句命令在最后并没有被用到

    findContours( threshold_img, contours, RETR_LIST, CHAIN_APPROX_SIMPLE );
    for (size_t i = 0; i < contours.size(); ++i )
        if ( contourArea( contours[i] ) > 1000 ) {
            Rect bounding_rect = boundingRect( contours[i] );
            rectangle( input, Point( bounding_rect.x, bounding_rect.y ), Point( bounding_rect.x + bounding_rect.width, bounding_rect.y + bounding_rect.height),
                       Scalar( 0, 255, 0 ), 3);
        }                                                                              //绘画符合要求的轮廓

//    namedWindow( "R_channel", WINDOW_AUTOSIZE );
//    namedWindow( "gray_img", WINDOW_AUTOSIZE );
//    namedWindow( "threshold_img", WINDOW_AUTOSIZE );
//    namedWindow( "canny", WINDOW_AUTOSIZE );
    namedWindow( "result", WINDOW_AUTOSIZE );
//    imshow( "R_channel", img );
//    imshow( "gray_img", gray_img );
//    imshow( "threshold_img", threshold_img );
//    imshow( "canny", canny_img );
    imshow( "result", input );

    waitKey( 0 );
    destroyAllWindows();
}
int main() {
    Mat img = imread( "../apple.png" );

    findApple( img );

    return 0;
}