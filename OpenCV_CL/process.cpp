//
//  process.cpp
//  OpenCV_CL
//
//  Created by 黃昱彰 on 2015/4/13.
//  Copyright (c) 2015年 黃昱彰. All rights reserved.
//

#include "process.h"

void findROI_CGAL(UMat src, int block_remove_undesir,
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
            if( y < max_up_pos )
                data[y*width+x] = 0;
            if( y > max_down_pos )
                data[y*width+x] = 0;
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
                if( data[(y+un)*width+(x+un)] == 0 )
                    count_remove++;
            }
            if( count_remove >= block_remove_threshold )
                data[y*width+x] = 0;
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
                if( data[y*width+x] == 0 && data[y*width+x-1] != 0 && data[y*width+x+num_cut] != 0)
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
                if(data[y*width+x] != 0 && data[y*width+x-1] == 0 && data[y*width+x+num_fill] == 0)
                    data[y*width+x] = 0;
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

void localCE(UMat src, int maskSize)
{
    Mat dataMat = src.getMat(ACCESS_WRITE);
    int width = dataMat.cols;
    int height = dataMat.rows;
    uchar *ptr;
    int data[width*height], dataTmp[width*height];
    
    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            data[y*width+x] = ptr[x];
            dataTmp[y*width+x] = ptr[x];
        }
    }
    
    for(int y = maskSize; y < height - maskSize; y++)
    {
        for(int x = maskSize; x < width - maskSize; x++)
        {
            int sum = 0;
            int hist[256] = {0};
            double grayNow;
            for(int j = y - maskSize; j <= y + maskSize; j++)
            {
                for(int i = x - maskSize; i <= x + maskSize; i++)
                    hist[data[j*width+i]]++;
            }
            for(int i = 0; i <= data[y*width+x]; i++)
                sum += hist[i];
            grayNow = 255 * (double)sum / (double)((2*maskSize+1)*(2*maskSize+1));
            if((int)grayNow > 255)
                grayNow = 255;
            else if((int)grayNow < 0)
                grayNow = 0;
            dataTmp[y*width+x] = (uchar)grayNow;
        }
    }
    
    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            ptr[x] = dataTmp[y*width+x];
        }
    }
}

void binarization(UMat src, int threshhold, int boundary)
{
    Mat dataMat = src.getMat(ACCESS_WRITE);
    int width = dataMat.cols;
    int height = dataMat.rows;
    uchar *ptr;
    
    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            if(ptr[x] > threshhold || x < boundary || x > width-boundary || y < boundary || y > height-boundary)
                ptr[x] = 255;
            else
                ptr[x] = 0;
        }
    }
    
}

void PGF(UMat src, int blockSize)
{
    Mat dataMat = src.getMat(ACCESS_WRITE);
    int width = dataMat.cols;
    int height = dataMat.rows;
    uchar *ptr;
    int data[width*height];
    int cntBlack = 0;
    int cntWhite = 0;
    
    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            data[y*width+x] = ptr[x];
        }
    }
    
    for( int y = blockSize; y < height-blockSize; y++ )
    {
        for( int x = blockSize; x < width-blockSize; x++ )
        {
            for( int m = -1*blockSize; m <= blockSize; m++ )
            {
                for( int n = -1*blockSize; n <= blockSize; n++ )
                {
                    if(data[(y+m)*width+(x+n)] == 0 )
                        cntBlack++;
                    else if(data[(y+m)*width+(x+n)] == 255)
                        cntWhite++;
                }
            }
            if( cntBlack > cntWhite )
            {
                data[y*width+x] = 0;
            }
            else if( cntBlack < cntWhite )
            {
                data[y*width+x] = 255;
            }
            cntBlack = cntWhite = 0;
        }
    }

    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            ptr[x] = data[y*width+x];
        }
    }
}


