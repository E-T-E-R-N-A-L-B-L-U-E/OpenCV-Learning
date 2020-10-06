//
// Created by mustang on 2020/10/6.
// Homework: Calibrate the Camera
//
#include <iostream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace cv;
using namespace std;


void calibrateCamera( ) {
    const int board_w = 9, board_h = 6;
    const int board_n = board_w * board_h;
    const int n_board = 40;
    Mat img, gray_img, drawn_img;
    Mat point_pix_pos( n_board * board_n, 3, CV_32FC1 );
    Mat point_grid_pos( n_board * board_n, 3, CV_32FC1 );
    Mat point_count( n_board, 1, CV_32SC1 );
    Mat intrinsic_matrix( 3, 3, CV_32FC1 );
//    Mat intrinsic_matrix;
    Mat distortion_coeffs( 5, 1, CV_32FC1 );
//    Mat distortion_coeffs;
    std::vector< Point2f > corners;
    int corner_count = 0;
    int successes = 0, step = 0;

    namedWindow( "drawn_chessboard", WINDOW_AUTOSIZE );
    for (int i = 0; i <= 40; i++ ) {
        String loc = "../chess/" + std::__cxx11::to_string( i ) + ".jpg";
        img = imread( loc );
        cvtColor( img, gray_img, COLOR_BGR2GRAY );
        int found = findChessboardCorners( gray_img,
                                           Size( board_w, board_h ),
                                           corners
                                           );
        drawn_img = img.clone();
        drawChessboardCorners( drawn_img, Size( board_w, board_h ), corners, found );

        imshow( "drawn_chessboard", drawn_img );

        if ( corners.size() == board_n ) {
            for (int j = 0; j < board_n; j++, step++) {
                point_pix_pos.at< float >( step, 0 ) = corners[ j ].x;
                point_pix_pos.at< float >( step, 1 ) = corners[ j ].y;
                point_grid_pos.at< float >( step, 0 ) = j / board_w;
                point_grid_pos.at< float >( step, 1 ) = j % board_w;
                point_grid_pos.at< float >( step, 2 ) = 0.0f;
            }
            point_count.at< float >( successes, 1 ) = board_n;
            successes++;
        }

        corners.clear();

        int c = waitKey( 1000 );
        if ( c == 'p' ) {
            c = 0;
            while ( c != 'p' && c != 27 )
                c = waitKey( 1000 );
        }
        if ( c == 27 )
            break;
    }
    destroyWindow( "drawn_chessboard" );

    std::vector<Vec2f> point_pix_pos2;
    std::vector<Vec3f> point_grid_pos2;

    Mat rvec(3,1, CV_64FC1);
    Mat tvec(3,1, CV_64FC1);
    vector<Mat> rvecs;
    vector<Mat> tvecs;
    rvecs.push_back(rvec);
    tvecs.push_back(tvec);

    Mat point_count2( successes * board_n, 1, CV_32SC1 );

    for (int i = 0; i < successes * board_n; i++) {
        point_pix_pos2.push_back( Vec2f(
                point_pix_pos.at< float >( i, 0 ),
                point_pix_pos.at< float >( i, 1 )
                ) );
        point_grid_pos2.push_back( Vec3f(
                point_grid_pos.at< float >( i, 0 ),
                point_grid_pos.at< float >( i, 1 ),
                point_grid_pos.at< float >( i, 2 )
                ) );
//        point_pix_pos2.at< float >( i, 0 ) = ;
//        point_pix_pos2.at< float >( i, 1 ) = ;
//        point_grid_pos2.at< float >( i, 0 ) = ;
//        point_grid_pos2.at< float >( i, 1 ) = ;
//        point_grid_pos2.at< float >( i, 2 ) = ;
    }
    for (int i = 0; i <successes; i++) {
        point_count2.at< int >( i, 1 ) = point_count.at< int >( i, 1 );
    }

    intrinsic_matrix.zeros( Size( 3, 3 ), CV_32FC1 );
    distortion_coeffs.zeros( Size( 5, 1 ), CV_32FC1 );
//    tvecs.push_back(Mat());
//    rvecs.push_back(Mat());
    calibrateCamera( point_grid_pos2, point_pix_pos2, img.size(), intrinsic_matrix, distortion_coeffs, rvecs, tvecs );
    Mat map_x, map_y;

#define IMG_SIZE ( Size( img.cols, img.rows ) )
    initUndistortRectifyMap(
            intrinsic_matrix,
            distortion_coeffs,
            Mat(),
            getOptimalNewCameraMatrix( intrinsic_matrix, distortion_coeffs, IMG_SIZE, 1, IMG_SIZE, 0),
            IMG_SIZE,
            CV_16SC2,
            map_x,
            map_y
            );
#undef IMG_SIZE

    namedWindow( "Calibrated Image", WINDOW_AUTOSIZE );
    for (int i = 0; i < n_board; i++ ){
        String loc = "../chess/" + std::__cxx11::to_string( i ) + ".jpg";
        img = imread( loc );
        Mat tmp = img.clone();
        remap( tmp, img, map_x, map_y, INTER_LINEAR );
        imshow( "Calibrated Image", img );

        int c = waitKey( 1000 );
        if ( c == 'p') {
            c = 0;
            while ( c != 'p' || c != 27 )
                c = waitKey( 1000 );
        }
        if ( c == 27 )
            break;
    }
    destroyWindow( "Calibrated Image" );
}
int main() {
    calibrateCamera();
    return 0;
}
//Mat gray_img, output;
//std::vector< Point2f > corners;
//int corners_count, pattern_was_found;
//cvtColor( input, gray_img, COLOR_BGR2GRAY );
//pattern_was_found = findChessboardCorners( gray_img, Size( 9, 6 ), corners, corners_count );
//output = input.clone();
//drawChessboardCorners( output, Size( 9, 6 ), corners, pattern_was_found );
//
//namedWindow( "input", WINDOW_AUTOSIZE );
//namedWindow( "output", WINDOW_AUTOSIZE );
//
//imshow( "input", input );
//imshow( "output", output );
//
//waitKey( 0 );
//
//destroyWindow( "input" );
//destroyWindow( "output" );