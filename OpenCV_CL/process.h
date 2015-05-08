//
//  process.h
//  OpenCV_CL
//
//  Created by 黃昱彰 on 2015/4/13.
//  Copyright (c) 2015年 黃昱彰. All rights reserved.
//

#ifndef __OpenCV_CL__process__
#define __OpenCV_CL__process__

#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#endif /* defined(__OpenCV_CL__process__) */

using namespace cv;
using namespace std;

void findROI_CGAL(UMat src, int block_remove_undesir,
                  int numberCutBlackLine, int numberFillBlackLine);
void localCE(UMat src, int maskSize);
void binarization(UMat src, int threshhold, int boundary);
void PGF(UMat src, int blockSize);

