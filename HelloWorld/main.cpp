#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

void example2_4( Mat image ) {
    Mat out;

    namedWindow( "Example4 - in" );
    namedWindow( "Example4 - out" );

    imshow( "Example4 - in", image);
    GaussianBlur( image, out, Size(51, 51), 0 );


    imshow( "Example4 - out", out );


    waitKey( 0 );
}

void doPyrDown( Mat img ) {
    Mat out;

    namedWindow( "input" );
    namedWindow( "output" );

    imshow( "input", img );

    pyrDown( img, out, Size( img.cols / 2, img.rows / 2 ) );

    imshow( "output", out );

    waitKey( 0 );
}

void doCanny( Mat img, bool showInput = false, bool waitForKey = true) {
    Mat out, tmp;

    namedWindow( "input" );
    namedWindow( "output" );

    if( showInput )
        imshow( "input", img );

    GaussianBlur( img, tmp, Size( 11, 11 ), 0 );

    Canny( tmp, out, 100, 110 );

    imshow( "output", out );

    if ( waitForKey )
        waitKey( 0 );

    if( showInput )
        destroyWindow( "input" );
    destroyWindow( "output" );
}


void doHoughLines( Mat img, Mat output ) {
    vector<Vec2f> lines;
    HoughLines( img, lines, 1, 3.1415 / 180, 80, 0, 0 );
    for (size_t i = 0; i < lines.size(); i++ ) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound( x0 + 1000 * (b) );
        pt1.y = cvRound( y0 + 1000 * (-a) );
        pt2.x = cvRound( x0 - 1000 * (b) );
        pt2.y = cvRound( y0 - 1000 * (-a) );
        line( output, pt2, pt1, Scalar( 0, 0, 255 ), 3, LINE_AA );
    }
}

void doHoughLinesP( Mat img, Mat output ) {
    vector<Vec4f> lines;
    HoughLinesP( img, lines, 1, 3.1415 / 180, 80, 30, 10 );
    for (size_t i = 0; i < lines.size(); i++ ) {
        Point pt1( lines[i][0], lines[i][1] ), pt2( lines[i][2], lines[i][3] );
        line( output, pt1, pt2, Scalar( 0, 255, 0 ), 3, LINE_AA );
    }
}

void showVideo() {
    Mat img, tmp, out, grayImg, binaryImg, linedImg;

    VideoCapture capture( "../VideoRecord.avi" );
    if ( !capture.isOpened() )
        cout << "failed to load video" << endl;

    long totalFrameNumber = capture.get( CAP_PROP_FRAME_COUNT );

    long frameToStart = 1;
    capture.set( CAP_PROP_POS_FRAMES, frameToStart );

    long frameToStop = 300;

    double rate = capture.get( CAP_PROP_FPS );

    long currentFrame = frameToStart;

    namedWindow( "input" );
    namedWindow( "output" );

    while ( true ) {
        if ( !capture.read( img ) ){
            cout << "failed to read img" << endl;
        }

        pyrDown( img, tmp, Size( img.cols / 2, img.rows / 2 ) );
//        doCanny( img );
        cvtColor( tmp, grayImg, COLOR_RGB2GRAY );
//        GaussianBlur( grayImg, tmp, Size( 15, 15 ), 0 );
//        morphologyEx( grayImg, tmp, MORPH_GRADIENT, Mat() );
//        threshold( grayImg, out, 30, 255, THRESH_BINARY );
        adaptiveThreshold( grayImg, out, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 71, 5 ) ;
        Canny( out, out, 10, 50 );

        cvtColor( out, linedImg, COLOR_GRAY2RGB );

        doHoughLines( out, linedImg );

//        erode( out, out, Mat() );
//        morphologyEx( out, out, MORPH_GRADIENT, Mat() );

        imshow( "input", grayImg );
        imshow( "output", linedImg );
        
        char c = waitKey( 30 );

        if ( c == 27 || currentFrame > frameToStop )
            break;

        if ( c >= 0 )
            waitKey( 0 );

        currentFrame++;
    }

    destroyWindow( "input" );
    destroyWindow( "output" );
}

void doROI( Mat img, Rect rt ) {
    Mat tmp;
    Mat result;
    img.copyTo( tmp );
    result = tmp( rt );
    add( result, Scalar( CV_RGB( 100, 100, 0)), result );

    namedWindow( "input" );
    namedWindow( "output" );

    imshow( "input", img );
    imshow( "output", tmp );

    waitKey( 0 );

    destroyAllWindows();
}

void drawLine( Mat img, Point start, Point end ) {
    line( img, start, end, CV_RGB( 12, 150, 160 ), 5 );

    namedWindow( "output" );
    imshow( "output", img );

    waitKey( 0 );
    destroyWindow( "output" );
}

void doErodeAndDilate( Mat img ) {
    Mat erodeMat, dilateMat, tmp;
    pyrDown( img, tmp, Size( img.cols / 2, img.rows / 2) );
    erode( tmp, erodeMat, Mat() );
    dilate( tmp, dilateMat, Mat() );

    namedWindow( "input" );
    namedWindow( "erode" );
    namedWindow( "dilate" );

    imshow( "input", tmp );
    imshow( "erode", erodeMat );
    imshow( "dilate", dilateMat) ;

    waitKey( 0 );

    destroyAllWindows();
}


int main(int argc, char** argv) {
    Mat img = imread( "../levi.jpg" );
    Mat output = img.clone();
//    example2_4( img );
//    doPyrDown( img );
//    doCanny( img );
    showVideo();
//    doROI( img, Rect( 0, 0, 500, 500) );
//    drawLine( img, Point( 0, 0 ), Point( 500, 600 ));
//    doErodeAndDilate( img );
//    cvtColor( img, img, COLOR_RGB2GRAY );
//    Canny( img, img, 50, 200, 3 );
//    output = img.clone();
//    cvtColor( output, output, COLOR_GRAY2BGR );
//    doHoughLinesP( img, output );
//    namedWindow( "output" );
//    imshow( "output", output );
//
//    waitKey( 0 );
//    destroyWindow( "output" );
    return 0;
}