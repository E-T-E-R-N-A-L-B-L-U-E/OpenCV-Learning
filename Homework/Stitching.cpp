//
// Created by mustang on 2020/10/29.
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
#include <vector>
using namespace cv;
Size getTransformSize( Mat homo, Mat pic, int &move_width, int &move_height ) {
    std::vector< Point2d > pic_corners, trans_corners;
    pic_corners.push_back( Point2d( 0, 0 ) );
    pic_corners.push_back( Point2d( 0, pic.rows ) );
    pic_corners.push_back( Point2d( pic.cols, 0 ) );
    pic_corners.push_back( Point2d( pic.cols, pic.rows ) );
    perspectiveTransform( pic_corners, trans_corners, homo );
    int trans_height, trans_width, min_height = INT_MAX, min_width = INT_MAX, max_height = 0, max_width = 0;
    for ( int i = 0; i < 4; i++ ) {
        min_height = MIN( min_height, trans_corners[ i ].y );
        min_width = MIN( min_width, trans_corners[ i ].x );
        max_height = MAX( max_height, trans_corners[ i ].y );
        max_width = MAX( max_width, trans_corners[ i ].x );
    }
    trans_height = max_height - min_height;
    trans_width = max_width - min_width;
    move_height = - min_height;
    move_width = - min_width;
    homo.at< double >( 0, 0 ) += homo.at< double >( 2, 0 ) * ( -min_width );
    homo.at< double >( 0, 1 ) += homo.at< double >( 2, 1 ) * ( -min_width );
    homo.at< double >( 0, 2 ) += homo.at< double >( 2, 2 ) * ( -min_width );
    homo.at< double >( 1, 0 ) += homo.at< double >( 2, 0 ) * ( -min_height );
    homo.at< double >( 1, 1 ) += homo.at< double >( 2, 1 ) * ( -min_height );
    homo.at< double >( 1, 2 ) += homo.at< double >( 2, 2 ) * ( -min_height );
//    homo.at< double >( 0, 2 ) += -min_width;
//    homo.at< double >( 1, 2 ) += -min_height;
    return Size( trans_width, trans_height );
}

void stitching(){
    Mat pic[ 3 ];
    pic[ 1 ] = imread( "../stereo-data/0_orig.jpg" );
    pic[ 2 ] = imread( "../stereo-data/1_orig.jpg" );
    pic[ 0 ] = imread( "../stereo-data/2_orig.jpg" );

//    SiftFeatureDetector sift_detector;
    std::vector< KeyPoint > key_points[ 3 ];
    Ptr<SiftFeatureDetector> sift_detector = SiftFeatureDetector::create();
    for ( int i = 0; i < 3; i++ ) {
        pyrDown( pic[ i ], pic[ i ] );
        sift_detector->detect( pic[ i ], key_points[ i ] );
    }

    Mat feature_pics[ 3 ];
    for ( int i = 0; i < 3; i++ ) {
        drawKeypoints( pic[ i ], key_points[ i ], feature_pics[ i ], Scalar::all( -1 ) );
    }

//    namedWindow( "pic0", WINDOW_AUTOSIZE );
//    namedWindow( "pic1", WINDOW_AUTOSIZE );
//    namedWindow( "pic2", WINDOW_AUTOSIZE );
//    imshow( "pic0", feature_pics[ 0 ] );
//    imshow( "pic1", feature_pics[ 1 ] );
//    imshow( "pic2", feature_pics[ 2 ] );

    Ptr< SiftDescriptorExtractor > sift_descriptor = SiftDescriptorExtractor::create();
    Mat description[ 3 ];
    for ( int i = 0; i < 3; i++ ){
        sift_descriptor->compute( pic[ i ], key_points[ i ], description[ i ] );
    }

//    Ptr< BFMatcher > bfmatcher = BFMatcher::create();
    BFMatcher bfmatcher;
    std::vector< DMatch > matchs1, matchs2;

    bfmatcher.match( description[ 0 ], description[ 1 ], matchs1 );
    bfmatcher.match( description[ 1 ], description[ 2 ], matchs2 );

//    nth_element( matchs1.begin(), matchs1.begin() + 20, matchs1.end() );
//    nth_element( matchs2.begin(), matchs2.begin() + 20, matchs2.end() );
//    matchs1.erase( matchs1.begin() + 21, matchs1.end() );
//    matchs2.erase( matchs2.begin() + 21, matchs2.end() );

    Mat matched_pic1, matched_pic2;
    drawMatches( pic[ 0 ], key_points[ 0 ], pic[ 1 ], key_points[ 1 ], matchs1, matched_pic1, Scalar( 0, 255, 0), Scalar::all( -1 ) );
    drawMatches( pic[ 1 ], key_points[ 1 ], pic[ 2 ], key_points[ 2 ], matchs2, matched_pic2, Scalar( 0, 255, 0), Scalar::all( -1 ) );

//    namedWindow( "match1", WINDOW_AUTOSIZE );
//    namedWindow( "match2", WINDOW_AUTOSIZE );
//
//    imshow( "match1", matched_pic1 );
//    imshow( "match2", matched_pic2 );

    std::vector< Point2f > query_matches[ 2 ], train_matches[ 2 ];
    for ( int i = 0; i < matchs1.size(); i++ ) {
        query_matches[ 0 ].push_back( key_points[ 0 ][ matchs1[i].queryIdx ].pt );
        train_matches[ 0 ].push_back( key_points[ 1 ][ matchs1[i].trainIdx ].pt );
    }
    for ( int i = 0; i < matchs2.size(); i++ ) {
        query_matches[ 1 ].push_back( key_points[ 1 ][ matchs2[i].queryIdx ].pt );
        train_matches[ 1 ].push_back( key_points[ 2 ][ matchs2[i].trainIdx ].pt );
    }

    Mat homo[ 2 ];
    homo[ 0 ] = findHomography( query_matches[ 0 ], train_matches[ 0 ], RANSAC);
    homo[ 1 ] = findHomography( train_matches[ 1 ], query_matches[ 1 ], RANSAC );

    Mat trans_pic1, trans_pic2;
    Size trans_size[ 2 ];
    int trans_height1, trans_width1, trans_height2, trans_width2;
    trans_size[ 0 ] = getTransformSize( homo[ 0 ], pic[ 0 ], trans_width1, trans_height1 );
    trans_size[ 1 ] = getTransformSize( homo[ 1 ], pic[ 2 ], trans_width2, trans_height2 );
    warpPerspective( pic[ 0 ], trans_pic1, homo[ 0 ], trans_size[ 0 ] );
    warpPerspective( pic[ 2 ], trans_pic2, homo[ 1 ], trans_size[ 1 ] );

//    namedWindow( "trans1", WINDOW_AUTOSIZE );
//    namedWindow( "trans2", WINDOW_AUTOSIZE );
//
//    imshow( "trans1", trans_pic1 );
//    imshow( "trans2", trans_pic2 );

    int tmp_width, tmp_height;
// TO-DO
    tmp_width = trans_size[ 0 ].width, tmp_height = trans_size[ 0 ].height;
    tmp_width = MAX( tmp_width, trans_width1 + pic[ 1 ].cols );
    tmp_height = MAX( tmp_height, trans_height1 + pic[ 1 ].rows );

    Mat tmp( tmp_height, tmp_width, CV_8UC3, Scalar::all( 0 ) );
    trans_pic1.copyTo( tmp( Rect( 0, 0, trans_pic1.cols, trans_pic1.rows ) ) );
    pic[ 1 ].copyTo( tmp( Rect( trans_width1, trans_height1, pic[ 1 ].cols, pic[ 1 ].rows ) ) );
//    pyrDown( tmp, tmp, Size( tmp.cols / 2, tmp.rows / 2 ) );

//    namedWindow( "tmp", WINDOW_AUTOSIZE );
//    imshow( "tmp", tmp );

    int top_left_x3 = trans_width1 - trans_width2, top_left_y3 = trans_height1 + (-trans_height2);
    int destination_trans_width, destination_trans_height;
    destination_trans_width = -MIN( 0, top_left_x3 );
    destination_trans_height = -MIN( 0, top_left_y3 );
    int destination_width, destination_height;
    destination_width = MAX( tmp.cols + destination_trans_width, top_left_x3 + trans_pic2.cols + destination_trans_width );
    destination_height = MAX( tmp.rows + destination_trans_height, top_left_y3 + trans_pic2.rows + destination_trans_height );
    Mat destination( destination_height, destination_width, CV_8UC3, Scalar::all( 0 ) );
    trans_pic1.copyTo( destination( Rect( destination_trans_width, destination_trans_height, trans_pic1.cols, trans_pic1.rows ) ) );
    trans_pic2.copyTo( destination( Rect( destination_trans_width + top_left_x3, destination_trans_height + top_left_y3, trans_pic2.cols, trans_pic2.rows ) ) );
    pic[ 1 ].copyTo( destination( Rect( destination_trans_width + trans_width1, destination_trans_height + trans_height1, pic[ 1 ].cols, pic[ 1 ].rows ) ) );
//    pyrDown( destination, destination );

//    namedWindow( "destination", WINDOW_AUTOSIZE );
//    imshow( "destination", destination );
//    GaussianBlur( destination, destination, Size( 3, 3 ), 3 );
    imwrite( "../stitched picture.jpg", destination );

    waitKey( 0 );
    destroyAllWindows();
}

int main(){
    stitching();
    return 0;
}

