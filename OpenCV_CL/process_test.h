//
//  process_test.h
//  OpenCV_CL
//
//  Created by 黃昱彰 on 2015/3/12.
//  Copyright (c) 2015年 黃昱彰. All rights reserved.
//

#ifndef __OpenCV_CL__process_test__
#define __OpenCV_CL__process_test__

#include "opencv2/core/core.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>

#endif /* defined(__OpenCV_CL__process_test__) */

using namespace cv;
using namespace std;

UMat clahe(UMat dataMat, int clip, int size);
void findROI(UMat dataMat, int thresh);
void findROI_white(UMat src, int block_remove_undesir,
                   int numberCutBlackLine, int numberFillBlackLine);
