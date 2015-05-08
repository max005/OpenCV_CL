//
//  process_test.cpp
//  OpenCV_CL
//
//  Created by 黃昱彰 on 2015/3/12.
//  Copyright (c) 2015年 黃昱彰. All rights reserved.
//

#include "process_test.h"

void findROI(UMat dataMat, int thresh)
{
    UMat cannyOutput;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    /// Detect edges using canny
    Canny(dataMat, cannyOutput, thresh, thresh*2, 3 );
    imshow("canny", cannyOutput);
    /// Find contours
    findContours(cannyOutput, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    UMat dst = UMat::zeros( cannyOutput.size(), CV_8UC1 );
    UMat mask = UMat::zeros( cannyOutput.size(), CV_8UC1 );;
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(255);
        drawContours( mask, contours, 2, color, 2, 8, hierarchy);
    }
    imshow( "Contours", mask);
    dataMat.copyTo(dst, mask);
    imshow( "ROI", dst);
}

void findROI_white(UMat src, int block_remove_undesir,
                  int numberCutBlackLine, int numberFillBlackLine) {
    Mat dataMat = src.getMat(ACCESS_WRITE);
    int width = dataMat.cols;
    int height = dataMat.rows;
    int max_up = 0    , max_down = 0;
    int max_up_pos = 0, max_down_pos = 0;
    int count_remove = 0;
    const int block_remove_threshold = (block_remove_undesir+1)*(block_remove_undesir+1)/2;
    uchar *ptr;
    int data[width*height], dataTmp[width*height];
    
    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            data[y*width+x] = ptr[x];
            dataTmp[y*width+x] = ptr[x];
        }
    }
    
    for( int x = 0; x < width; x++ )
    {
        for( int y = height/2; y > 0; y-- )
        {
            if( data[y*width+x] - data[(y-1)*width+x] > max_up)
            {
                max_up = data[y*width+x] - data[(y-1)*width+x];
                max_up_pos = y;
            }
        }
        max_up = 0;
        
        for( int y = height/2; y < height-1; y++ )
        {
            if( data[y*width+x] - data[(y+1)*width+x] > max_down)
            {
                max_down = data[y*width+x] - data[(y+1)*width+x];
                max_down_pos = y;
            }
        }
        max_down = 0;
        
        for( int y = 0; y < height; y++ )
        {
            if( y <= max_up_pos )
                data[y*width+x] = 255;
            if( y >= max_down_pos )
                data[y*width+x] = 255;
        }
        max_up_pos = 0;
        max_down_pos = 0;
    }
    
    // Remove undesirable mask
    for( int y = block_remove_undesir; y < height-block_remove_undesir; y++ )
    {
        for( int x = block_remove_undesir; x < width-block_remove_undesir; x++ )
        {
            for( int un = (-1)*block_remove_undesir; un <= block_remove_undesir; un++ )
            {
                if( data[(y+un)*width+(x+un)] == 255 )
                    count_remove++;
            }
            if( count_remove >= block_remove_threshold )
                data[y*width+x] = 255;
            count_remove = 0;
        }
    }
    
    // Remove unnecessary black line
    for( int y = 0; y < height; y++ )
    {
        for( int num_cut = 1; num_cut < numberCutBlackLine; num_cut++ )
        {
            for( int x = 0; x < width; x++ )
            {
                if( data[y*width+x] == 255 && data[y*width+x-1] != 255 && data[y*width+x+num_cut] != 255)
                    data[y*width+x] = dataTmp[y*width+x];
            }
        }
    }
    
    // Fill necessary line
    for( int y = 0; y < height; y++ )
    {
        for( int num_fill = 1; num_fill < numberFillBlackLine; num_fill++ )
        {
            for( int x = 0; x < width; x++ )
            {
                if(data[y*width+x] != 255 && data[y*width+x-1] == 255 && data[y*width+x+num_fill] == 255)
                    data[y*width+x] = 255;
            }
        }
    }
    
    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            ptr[x] = data[y*width+x];
        }
    }
}


UMat clahe(UMat dataMat, int clip, int size)
{
    UMat dst;
    
    Ptr<CLAHE> pFilter = createCLAHE();
    pFilter->setClipLimit(clip);
    pFilter->setTilesGridSize(Size(size,size));
    pFilter->apply(dataMat, dst);
    
    return dst;
}
