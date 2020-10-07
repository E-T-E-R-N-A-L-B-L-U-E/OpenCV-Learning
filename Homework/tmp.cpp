////
//// Created by mustang on 2020/10/6.
//// Homework: Calibrate the Camera
////
//#include <iostream>
//#include <sstream>
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
//
//using namespace cv;
//using namespace std;
//
//
//void calibrateCamera( ) {
//    const int board_w = 9, board_h = 6;
//    const int board_n = board_w * board_h;
//    const int n_board = 40;
//    Mat img, gray_img, drawn_img;
//    Mat point_pix_pos( n_board * board_n, 3, CV_32FC1 );
//    Mat point_grid_pos( n_board * board_n, 3, CV_32FC1 );
//    Mat point_count( n_board, 1, CV_32SC1 );
//    Mat intrinsic_matrix( 3, 3, CV_32FC1 );
////    Mat intrinsic_matrix;
//    Mat distortion_coeffs( 5, 1, CV_32FC1 );
////    Mat distortion_coeffs;
//    std::vector< Point2f > corners;
//    int corner_count = 0;
//    int successes = 0, step = 0;
//
//    namedWindow( "drawn_chessboard", WINDOW_AUTOSIZE );
//    for (int i = 0; i <= 40; i++ ) {
//        String loc = "../chess/" + std::__cxx11::to_string( i ) + ".jpg";
//        img = imread( loc );
//        cvtColor( img, gray_img, COLOR_BGR2GRAY );
//        int found = findChessboardCorners( gray_img,
//                                           Size( board_w, board_h ),
//                                           corners
//                                           );
//        drawn_img = img.clone();
//        drawChessboardCorners( drawn_img, Size( board_w, board_h ), corners, found );
//
//        imshow( "drawn_chessboard", drawn_img );
//
//        if ( corners.size() == board_n ) {
//            for (int j = 0; j < board_n; j++, step++) {
//                point_pix_pos.at< float >( step, 0 ) = corners[ j ].x;
//                point_pix_pos.at< float >( step, 1 ) = corners[ j ].y;
//                point_grid_pos.at< float >( step, 0 ) = j / board_w;
//                point_grid_pos.at< float >( step, 1 ) = j % board_w;
//                point_grid_pos.at< float >( step, 2 ) = 0.0f;
//            }
//            point_count.at< float >( successes, 1 ) = board_n;
//            successes++;
//        }
//
//        corners.clear();
//
//        int c = waitKey( 1000 );
//        if ( c == 'p' ) {
//            c = 0;
//            while ( c != 'p' && c != 27 )
//                c = waitKey( 1000 );
//        }
//        if ( c == 27 )
//            break;
//    }
//    destroyWindow( "drawn_chessboard" );
//
//    std::vector<Point2f> point_pix_pos2;
//    std::vector<Point3f> point_grid_pos2;
//
////    Mat rvec(3,1, CV_64FC1);
////    Mat tvec(3,1, CV_64FC1);
//    vector<vector<double>> rvecs;
//    vector<vector<double>> tvecs;
//
//    Mat point_count2( successes * board_n, 1, CV_32SC1 );
//
//    for (int i = 0; i < successes * board_n; i++) {
//        point_pix_pos2.push_back( Point2f(
//                point_pix_pos.at< float >( i, 0 ),
//                point_pix_pos.at< float >( i, 1 )
//                ) );
//        point_grid_pos2.push_back( Point3f(
//                point_grid_pos.at< float >( i, 0 ),
//                point_grid_pos.at< float >( i, 1 ),
//                point_grid_pos.at< float >( i, 2 )
//                ) );
////        point_pix_pos2.at< float >( i, 0 ) = ;
////        point_pix_pos2.at< float >( i, 1 ) = ;
////        point_grid_pos2.at< float >( i, 0 ) = ;
////        point_grid_pos2.at< float >( i, 1 ) = ;
////        point_grid_pos2.at< float >( i, 2 ) = ;
//    }
//    for (int i = 0; i <successes; i++) {
//        point_count2.at< int >( i, 1 ) = point_count.at< int >( i, 1 );
//    }
//
//    intrinsic_matrix.zeros( Size( 3, 3 ), CV_32FC1 );
//    distortion_coeffs.zeros( Size( 5, 1 ), CV_32FC1 );
////    tvecs.push_back(Mat());
////    rvecs.push_back(Mat());
//    calibrateCamera( point_grid_pos2, point_pix_pos2, img.size(), intrinsic_matrix, distortion_coeffs, rvecs, tvecs );
//    Mat map_x, map_y;
//
//#define IMG_SIZE ( Size( img.cols, img.rows ) )
//    initUndistortRectifyMap(
//            intrinsic_matrix,
//            distortion_coeffs,
//            Mat(),
//            getOptimalNewCameraMatrix( intrinsic_matrix, distortion_coeffs, IMG_SIZE, 1, IMG_SIZE, 0),
//            IMG_SIZE,
//            CV_16SC2,
//            map_x,
//            map_y
//            );
//#undef IMG_SIZE
//
//    namedWindow( "Calibrated Image", WINDOW_AUTOSIZE );
//    for (int i = 0; i < n_board; i++ ){
//        String loc = "../chess/" + std::__cxx11::to_string( i ) + ".jpg";
//        img = imread( loc );
//        Mat tmp = img.clone();
//        remap( tmp, img, map_x, map_y, INTER_LINEAR );
//        imshow( "Calibrated Image", img );
//
//        int c = waitKey( 1000 );
//        if ( c == 'p') {
//            c = 0;
//            while ( c != 'p' || c != 27 )
//                c = waitKey( 1000 );
//        }
//        if ( c == 27 )
//            break;
//    }
//    destroyWindow( "Calibrated Image" );
//}
//int main() {
//    calibrateCamera();
//    return 0;
//}
////Mat gray_img, output;
////std::vector< Point2f > corners;
////int corners_count, pattern_was_found;
////cvtColor( input, gray_img, COLOR_BGR2GRAY );
////pattern_was_found = findChessboardCorners( gray_img, Size( 9, 6 ), corners, corners_count );
////output = input.clone();
////drawChessboardCorners( output, Size( 9, 6 ), corners, pattern_was_found );
////
////namedWindow( "input", WINDOW_AUTOSIZE );
////namedWindow( "output", WINDOW_AUTOSIZE );
////
////imshow( "input", input );
////imshow( "output", output );
////
////waitKey( 0 );
////
////destroyWindow( "input" );
////destroyWindow( "output" );
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;
int main()
{
    ifstream fin("../calibdata.txt"); /* 标定所用图像文件的路径 */
    ofstream fout("../caliberation_result.txt");  /* 保存标定结果的文件 */
    //读取每一幅图像，从中提取出角点，然后对角点进行亚像素精确化
    cout << "开始提取角点………………";
    int image_count = 0;  /* 图像数量 */
    Size image_size;  /* 图像的尺寸 */
    Size board_size = Size(9, 6);    /* 标定板上每行、列的角点数 */
    vector<Point2f> image_points_buf;  /* 缓存每幅图像上检测到的角点 */
    vector<vector<Point2f>> image_points_seq; /* 保存检测到的所有角点 */
    string filename;
    int count = -1;//用于存储角点个数。
    while (getline(fin, filename))
    {
        image_count++;
        // 用于观察检验输出
        cout << "正在检验第 " << image_count <<"幅图像"<< endl;

        Mat imageInput = imread(filename);
        if (image_count == 1)  //读入第一张图片时获取图像宽高信息
        {
            image_size.width = imageInput.cols;
            image_size.height = imageInput.rows;
            cout << "image_size.width = " << image_size.width << endl;
            cout << "image_size.height = " << image_size.height << endl;
        }
        /* 提取角点 */
        if (0 == findChessboardCorners(imageInput, board_size, image_points_buf))
        {
            cout << "can not find chessboard corners!\n"; //找不到角点
            exit(1);
        }
        else
        {
            Mat view_gray;
            cvtColor(imageInput, view_gray, CV_RGB2GRAY);
            /* 亚像素精确化 */
            find4QuadCornerSubpix(view_gray, image_points_buf, Size(5, 5)); //对粗提取的角点进行精确化
            //cornerSubPix(view_gray,image_points_buf,Size(5,5),Size(-1,-1),TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,0.1));
            image_points_seq.push_back(image_points_buf);  //保存亚像素角点
            /* 在图像上显示角点位置 */
            drawChessboardCorners(view_gray, board_size, image_points_buf, false); //用于在图片中标记角点
            //imshow("Camera Calibration", view_gray);//显示图片
            //waitKey(50);//暂停0.05S
        }
    }
    int total = image_points_seq.size(); //所有图像加起来一共多少角点
    cout << "所有图像的所有角点 = " << total << endl;
    int CornerNum = board_size.width * board_size.height;  //每张图片上总的角点数
    for (int ii = 0; ii < total; ii++)
    {
        if (0 == ii % CornerNum)// 24 是每幅图片的角点个数。此判断语句是为了输出 图片号，便于控制台观看
        {
            int i = -1;
            i = ii / CornerNum;
            int j = i + 1;
            cout << "--> 第 " << j << "图片的数据 --> : " << endl;
        }
        if (0 == ii % 3)    // 此判断语句，格式化输出，便于控制台查看
        {
            cout << endl;
        }
        else
        {
            cout.width(10);
        }
        //输出所有的角点
        cout << " x: " << image_points_seq[ii][0].x;
        cout << " y: " << image_points_seq[ii][0].y;
    }
    cout << "角点提取完成！\n";
    //以下是摄像机标定
    cout << "开始标定………………";
    /*棋盘三维信息*/
    Size square_size = Size(10, 10);  /* 实际测量得到的标定板上每个棋盘格的大小 */
    vector<vector<Point3f>> object_points; /* 保存标定板上角点的三维坐标 */
    /*内外参数*/
    Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 摄像机内参数矩阵 */
    vector<int> point_counts;  // 每幅图像中角点的数量
    Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
    vector<Mat> tvecsMat;  /* 每幅图像的旋转向量 */
    vector<Mat> rvecsMat; /* 每幅图像的平移向量 */
    /* 初始化标定板上角点的三维坐标 */
    int i, j, t;
    for (t = 0; t < image_count; t++)
    {
        vector<Point3f> tempPointSet;
        for (i = 0; i < board_size.height; i++)
        {
            for (j = 0; j < board_size.width; j++)
            {
                Point3f realPoint;
                /* 假设标定板放在世界坐标系中z=0的平面上 */
                realPoint.x = i * square_size.width;
                realPoint.y = j * square_size.height;
                realPoint.z = 0;
                tempPointSet.push_back(realPoint);
            }
        }
        object_points.push_back(tempPointSet);
    }
    /* 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板 */
    for (i = 0; i < image_count; i++)
    {
        point_counts.push_back(board_size.width * board_size.height);
    }
    /* 开始标定 */
    calibrateCamera(object_points, image_points_seq, image_size, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);
    cout << "标定完成！\n";
    //对标定结果进行评价
    cout << "开始评价标定结果………………\n";
    double total_err = 0.0; /* 所有图像的平均误差的总和 */
    double err = 0.0; /* 每幅图像的平均误差 */
    vector<Point2f> image_points2; /* 保存重新计算得到的投影点 */
    cout << "\t每幅图像的标定误差：\n";
    fout << "每幅图像的标定误差：\n";
    for (i = 0; i < image_count; i++)
    {
        vector<Point3f> tempPointSet = object_points[i];
        /* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
        projectPoints(tempPointSet, rvecsMat[i], tvecsMat[i], cameraMatrix, distCoeffs, image_points2);
        /* 计算新的投影点和旧的投影点之间的误差,z这个标定结果反应的是标定算法的好坏*/
        vector<Point2f> tempImagePoint = image_points_seq[i];
        Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
        Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
        for (int j = 0; j < tempImagePoint.size(); j++)
        {
            image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
            tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
        }
        err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
        total_err += err /= point_counts[i];
        std::cout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
        fout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
    }
    std::cout << "总体平均误差：" << total_err / image_count << "像素" << endl;
    fout << "总体平均误差：" << total_err / image_count << "像素" << endl << endl;
    std::cout << "评价完成！" << endl;
    //保存定标结果
    std::cout << "开始保存定标结果………………" << endl;
    Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */
    fout << "相机内参数矩阵：" << endl;
    fout << cameraMatrix << endl << endl;
    fout << "畸变系数：\n";
    fout << distCoeffs << endl << endl << endl;
    for (int i = 0; i < image_count; i++)
    {
        fout << "第" << i + 1 << "幅图像的旋转向量：" << endl;
        fout << tvecsMat[i] << endl;
        /* 将旋转向量转换为相对应的旋转矩阵 */
        Rodrigues(tvecsMat[i], rotation_matrix);
        fout << "第" << i + 1 << "幅图像的旋转矩阵：" << endl;
        fout << rotation_matrix << endl;
        fout << "第" << i + 1 << "幅图像的平移向量：" << endl;
        fout << rvecsMat[i] << endl << endl;
    }
    std::cout << "完成保存" << endl;
    fout << endl;
    /************************************************************************
    显示定标结果
    *************************************************************************/
    Mat mapx = Mat(image_size, CV_32FC1);
    Mat mapy = Mat(image_size, CV_32FC1);
    Mat R = Mat::eye(3, 3, CV_32F);
    std::cout << "保存矫正图像" << endl;
    String imageFileName;
    std::stringstream StrStm;
    i = -1;
    fin.close();
    fin.open("../calibdata.txt");
    cv::namedWindow( "resultImage" );
    while (getline(fin, filename))
    {
        i++;
        std::cout << "Frame #" << i + 1 << "..." << endl;
        initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, image_size, CV_32FC1, mapx, mapy);
        StrStm.clear();
        //imageFileName.clear();

        Mat imageSource = imread(filename);
        Mat newimage = imageSource.clone();
        if (imageSource.empty()) {
            cout << "can't find " << filename << endl;
            exit(-1);
        }
        //另一种不需要转换矩阵的方式
        //undistort(imageSource,newimage,cameraMatrix,distCoeffs);
        try {
            remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);
        }
        catch (Exception e) {
            cout << e.what() << endl;
        }
        StrStm.clear();
        char* fullname = (char*)filename.data();
        const char* b = ".";
        imageFileName = strtok(fullname, b);
        imageFileName += "_d.jpg";
        cout << imageFileName << endl;
        imwrite(imageFileName, newimage);
        cv::imshow("resultImage", newimage);
        cv::waitKey(1000 );
    }
    std::cout << "保存结束" << endl;
    system("pause");
    return 0;
}