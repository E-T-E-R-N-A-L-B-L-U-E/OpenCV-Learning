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

Mat g_camera_matrix( 3, 3, CV_32FC1, Scalar( 0 ) );
Mat g_distortion_coeffs( 1, 5, CV_32FC1, Scalar( 0 ) );
std::vector< std::vector< Point3f > > g_point_grid_pos;
std::vector< std::vector< Point2f > > g_point_pix_pos;
std::vector< Mat > g_r_mats;
std::vector< Mat > g_t_vecs;

void calibrateCamera( bool showImgResult ) {
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
    g_camera_matrix = camera_matrix.clone();
    g_distortion_coeffs = dist_coeffs.clone();
    g_point_grid_pos.assign( point_grid_pos.begin(), point_grid_pos.end() );
    g_point_pix_pos.assign( point_pix_pos.begin(), point_pix_pos.end() );

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

    if ( !showImgResult ) {
        return ;
    }

    Mat output_img;
    namedWindow( "Calibrated Image", WINDOW_AUTOSIZE );
    for (int i = 0; i < 40; i++ ){
        String loc = "../chess/" + std::__cxx11::to_string( i ) + ".jpg";
        img = imread( loc );
        Mat tmp = img.clone();
        remap( tmp, img, map_x, map_y, INTER_LINEAR );
        imshow( "Calibrated Image", img );
        output_img = img.clone();

//        int c = waitKey( 1000 );
//        if ( c == 'p') {
//            c = 0;
//            while ( c != 'p' || c != 27 )
//                c = waitKey( 1000 );
//        }
//        if ( c == 27 )
//            break;
    }
    destroyWindow( "Calibrated Image" );

    namedWindow( "drawn chess board", WINDOW_AUTOSIZE );
    namedWindow( "output", WINDOW_AUTOSIZE );
    pyrDown( drawn_img, drawn_img, Size( img_size.width / 2, img_size.height / 2) );
    pyrDown( output_img, output_img, Size( img_size.width / 2, img_size.height / 2) );
    imshow( "drawn chess board", drawn_img );
    imshow( "output", output_img );

    waitKey( 0 );

    destroyAllWindows();
}

void findRTVec( bool echo = true ) {
    Mat r_vec;
    Mat t_vec;
    Mat r_mat;
    g_r_mats.clear();
    g_t_vecs.clear();
    for ( int i = 0; i < g_point_pix_pos.size(); i++ ) {
        g_r_mats.push_back( Mat( 3, 3, CV_32FC1, Scalar( 0 ) ) );
        g_t_vecs.push_back( Mat( 3, 1, CV_32FC1, Scalar( 0 ) ) );
        solvePnP( g_point_grid_pos[ i ], g_point_pix_pos[ i ], g_camera_matrix, g_distortion_coeffs, r_vec, t_vec );
        Rodrigues( r_vec, r_mat );
        g_r_mats[ i ] = r_mat.clone();
        g_t_vecs[ i ] = t_vec.clone();
        if ( echo )
            std::cout << "The r_vec and the t_vec of the img NO." << i << " is:\n" << g_r_mats[ i ] << std::endl << g_t_vecs[ i ] << std::endl << std::endl;
    }
//    std::cout<<"CHECK:" <<std::endl << g_r_mats[0] << std::endl<<g_r_mats[1]<<std::endl;
//    solvePnP( g_point_grid_pos, g_point_pix_pos, g_camera_matrix, g_distortion_coeffs, r_vecs, t_vecs );
//    std::cout << "The r vec is:\n" << r_vecs << "\nThe t vec is:\n" << std::endl << t_vecs;
}
//triangulatePoints

void get3DPoint() {
    std::cout << "start triangulate points" << std::endl;
    Mat pos1;
    Mat pos2;
    Mat result;
    std::vector< Point2f > proj_points1;
    std::vector< Point2f > proj_points2;
    undistortPoints( g_point_pix_pos[ 0 ], proj_points1, g_camera_matrix, g_distortion_coeffs );
    undistortPoints( g_point_pix_pos[ 1 ], proj_points2, g_camera_matrix, g_distortion_coeffs );
//    findRTVec( false );
//    std::cout << g_r_mats[ 0 ] << std::endl << g_r_mats[ 1 ] << std::endl;
    hconcat( g_r_mats[ 0 ], g_t_vecs[ 0 ], pos1 );
    hconcat( g_r_mats[ 1 ], g_t_vecs[ 1 ], pos2 );

    std::cout << "The position of the first camera is: " << std::endl;
    std::cout << pos1 << std::endl;
    std::cout << "The position of the second camera is: " << std::endl;
    std::cout << pos2 << std::endl << std::endl;

    triangulatePoints( pos1, pos2, proj_points1, proj_points2, result );
//    std::cout << "The Point is: " << std::endl << result << std::endl;
//    std::cout << "The Point is" << std::endl << result.at< float >( 0, 0 )
//        << " " << result.at< float >( 1, 0 ) << " " << result.at< float >( 2, 0 )
//        << " " << result.at< float >( 3, 0 ) << std::endl;
    for ( int i = 0; i < result.cols; i++ )
        result.col( i ) /= result.at< float >( 3, i );
    transpose( result, result );

/*     std::cout << "The Points are:" << std::endl;
    for ( int i = 0; i < result.cols; i++ ) {
        std::cout << "[ ";
        for ( int j = 0; j < result.rows; j++ ) {
            std::cout << result.at<float>(j, i);
            if ( j == result.rows - 1 )
                std::cout << " ]" << std::endl ;
            else
                std::cout << ", ";
        }
    }*/
    for ( int i = 0; i < result.rows; i++ ) {
        std::cout << "Point No." << i << ": \n" << result.row( i ) << std::endl;
    }
}
int main() {
    calibrateCamera( false );
    findRTVec();
    get3DPoint();
    return 0;
}