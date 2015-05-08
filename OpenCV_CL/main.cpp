//
//  main.cpp
//  OpenCV_CL
//
//  Created by 黃昱彰 on 2015/2/5.
//  Copyright (c) 2015年 黃昱彰. All rights reserved.
//

#include "opencv2/core/core.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include "process.h"
#include "process_test.h"

using namespace cv;
using namespace std;

clock_t start, finish;
double duration;

int main(int argc, char *argv[])
{
    UMat src;
    UMat dst, dst1;
    
    ocl::setUseOpenCL(true);
    
    imread("/Users/HuangYuChang/Documents/workspace/image/finger01.bmp", -1).copyTo(src);
    cvtColor(src, dst, COLOR_BGR2GRAY);
    imshow("Gray", dst);
    
    findROI_CGAL(dst, 2, 70, 70);
    imshow("Roi", dst);
    
    localCE(dst, 20);
    imshow("LocalCE", dst);
    
    binarization(dst, 90, 30);
    imshow("Bin", dst);
    
    PGF(dst, 2);
    imshow("PGF", dst);
    
    waitKey(0);
    
    return 0;
}

