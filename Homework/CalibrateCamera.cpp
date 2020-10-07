////
//// Created by mustang on 2020/10/6.
//// Homework: Calibrate the Camera
////
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace cv;

void calibrateCamera() {
    const int board_w = 9, board_h = 6;
    const int board_n = board_w * board_h;
    Size board_size( 9, 6 );

    Mat img, gray_img, drawn_img;
    std::vector< Point2f > point_pix_pos_buf;
    std::vector< std::vector<Point2f> > point_pix_pos;

    int found, successes = 0;
    Size img_size;

    for (int i = 0; i <=40; i++ ) {
        String loc = "../chess/" + std::__cxx11::to_string( i ) + ".jpg";
        img = imread( loc );
        if ( img.empty() ) {
            std::cout << "failed to read image: " << loc << std::endl;
            continue;
        } else {
            std::cout << "read image " << loc << "successfully" << std::endl;
        }
        if ( !i ) {
            img_size.width = img.cols;
            img_size.height = img.rows;
        }
        found = findChessboardCorners( img, board_size, point_pix_pos_buf );
        if ( found && point_pix_pos_buf.size() == board_n ) {
            successes++;
            cvtColor( img, gray_img, COLOR_BGR2GRAY );
            find4QuadCornerSubpix( gray_img, point_pix_pos_buf, Size( 5, 5 ) );
            point_pix_pos.push_back( point_pix_pos_buf );
            drawn_img = img.clone();
            drawChessboardCorners( drawn_img, board_size, point_pix_pos_buf, found );
        } else
            std::cout << "\tbut failed to found all chess board corners in this image" << std::endl;
        point_pix_pos_buf.clear();
    }
    std::cout << successes << " useful chess boards" << std::endl;

    Size square_size( 10, 10 );
    std::vector< std::vector< Point3f > > point_grid_pos;
    std::vector< Point3f > point_grid_pos_buf;
    std::vector< int > point_count;

    Mat camera_matrix( 3, 3, CV_32FC1, Scalar::all( 0 ) );
    Mat dist_coeffs( 1, 5, CV_32FC1, Scalar::all( 0 ) );
    std::vector< Mat > rvecs;
    std::vector< Mat > tvecs;

    for (int i = 0; i < successes; i++ ) {
        for (int j = 0; j < board_h; j++ ) {
            for (int k = 0; k < board_w; k++ ){
                Point3f pt;
                pt.x = k * square_size.width;
                pt.y = j * square_size.height;
                pt.z = 0;
                point_grid_pos_buf.push_back( pt );
            }
        }
        point_grid_pos.push_back( point_grid_pos_buf );
        point_grid_pos_buf.clear();
        point_count.push_back( board_h * board_w );
    }

    calibrateCamera( point_grid_pos, point_pix_pos, img_size, camera_matrix, dist_coeffs, rvecs, tvecs );
    std::cout << "Successfully calibrate camera!" << std::endl;
    std::cout << "The camera matrix is:\n" << camera_matrix << std::endl;
    std::cout << "This distortion coeff is: \n" << dist_coeffs << std::endl;

    Mat map_x( img_size, CV_32FC1 );
    Mat map_y( img_size, CV_32FC1 );
    initUndistortRectifyMap(
            camera_matrix,
            dist_coeffs,
            Mat(),
            camera_matrix,
            img_size,
            CV_32FC1,
            map_x,
            map_y
            );

    Mat output_img;
    namedWindow( "Calibrated Image", WINDOW_AUTOSIZE );
    for (int i = 0; i < 40; i++ ){
        String loc = "../chess/" + std::__cxx11::to_string( i ) + ".jpg";
        img = imread( loc );
        Mat tmp = img.clone();
        remap( tmp, img, map_x, map_y, INTER_LINEAR );
        imshow( "Calibrated Image", img );
        output_img = img.clone();

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

//    namedWindow( "drawn chess board", WINDOW_AUTOSIZE );
//    namedWindow( "output", WINDOW_AUTOSIZE );
//    pyrDown( drawn_img, drawn_img, Size( img_size.width / 2, img_size.height / 2) );
//    pyrDown( output_img, output_img, Size( img_size.width / 2, img_size.height / 2) );
//    imshow( "drawn chess board", drawn_img );
//    imshow( "output", output_img );
//
//    waitKey( 0 );
//
//    destroyAllWindows();
}

int main() {
    calibrateCamera();
    return 0;
}