#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>
#include <set>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>


cv::Mat image1 = cv::imread("/home/satyesh/code/astar/img.jpg");
cv::Mat image;

cv::Mat image_copy;


int si; int sj; int ei; int ej, c = 0;
float **dsrc;
cv::Point **p;
cv::Point end, start;
int ep = 8;

struct less 
{
bool operator()(cv::Point p1, cv::Point p2)
{
	if(p1.x < p2.x)
	{
		return true;
	}
	else if(p1.x == p2.x)
	{
		if(p1.y < p2.y)
			return true;
		else
			return false;
	}
	else
		return false;
}
};



float h(cv::Point p)
{
	return sqrt((p.x-ei) * (p.x-ei) + (p.y - ej) * (p.y - ej) );
}


 struct compare  
 {  
   bool operator()(const cv::Point& l, const cv::Point& r)  
   {  
       return (dsrc[l.x][l.y] + ep * h(l)) > (dsrc[r.x][r.y] + ep * h(r));  
   }  
 };  

typedef struct compare compare;	

std::priority_queue<cv::Point, std::vector<cv::Point>, compare> open;
std::queue<cv::Point> incons;
std::set<cv::Point, less> closed;


float fvalue(cv::Point p)
{
	return (dsrc[p.x][p.y] + ep * h(p));
}
void ImprovePath()
{
	int i, j;int flag = 1;
	cv::Point min, node;
	float dist;
	while(fvalue(end) > fvalue(open.top()))
	{
		if(flag == 1)
		{
		std::cout<<"saty \n";
		flag = 0;
		}
		min = open.top();
		closed.insert(min);
		open.pop();
		image_copy.at<uchar>(min.x,min.y) = 80;
		for(i = -1; i <= 1; i++)
			{
				for(j=-1; j <= 1; j++)
				{
					if(min.x == 0 && i == -1)
						continue;
					if(min.y == 0 && j == -1)
						continue;
					if(min.x == image_copy.rows-1 && i == 1)
						continue;
					if(min.y == image_copy.cols-1 && j == 1)
						continue;

					if(i == 0 || j == 0)
						dist = 1;
					else
						dist = 1.414;

					if(dsrc[min.x + i][min.y + j] > (dsrc[min.x][min.y] + dist))
					{
						dsrc[min.x + i][min.y + j] = dsrc[min.x][min.y] + dist;
						p[min.x + i][min.y + j].x = min.x; 
						p[min.x + i][min.y + j].y = min.y;
						node.x = min.x + i;
						node.y = min.y + j;

						if(closed.find(node) == closed.end())
							open.push(node);
						else
							incons.push(node);
					}
					
					
				}
			}
		//cv::imshow("Path" , image_copy);
		//cv::waitKey(1);
	}
}

void ARAstar()
{

	c = 0;
	dsrc = new float*[image_copy.rows];
	for(int i = 0; i < image_copy.rows; ++i)
		dsrc[i] = new float[image_copy.cols];

	p = new cv::Point*[image_copy.rows];
	for(int i = 0; i < image_copy.rows; ++i)
		p[i] = new cv::Point[image_copy.cols];
	
	int i, j, temp;
	
	for(i = 0; i < image_copy.rows; i++ )
		{
			for (j = 0; j < image_copy.cols; ++j)
			{
				//visited[i][j] = 0;
				if(image_copy.at<uchar>(i,j) <= 100)
					dsrc[i][j] = -1;
				else
					dsrc[i][j] = INT_MAX;

				// indexset[i][j] = false;
				p[i][j].x = -1;
				p[i][j].y = -1;
			}
		}

		dsrc[si][sj] = 0;


		open.push(start);
		ImprovePath();
		i = ei;
		j = ej;
		while(i != si || j != sj)
		{
			//std::cout<<i<<","<<j<<"\n";
			image_copy.at<uchar>(i, j) = 0;
			temp = p[i][j].x;
			j = p[i][j].y;
			i = temp;

		}
		cv::imshow("Path", image_copy);

		while(ep > 1)
		{
			c++;
			std::cout<<"A\n";
    		cv::cvtColor(image, image_copy, CV_BGR2GRAY);
			ep/=2;
			while(!incons.empty())
			{
				open.push(incons.front());
				incons.pop();
			}

			closed.clear();
			ImprovePath();
			i = ei;
			j = ej;
			while(i != si || j != sj)
			{	
				//std::cout<<i<<","<<j<<"\n";
				image_copy.at<uchar>(i, j) = 0;
				temp = p[i][j].x;
				j = p[i][j].y;
				i = temp;

			}
			if(c == 1)
				cv::imshow("Path1", image_copy);

			else if(c == 2)
				cv::imshow("Path2", image_copy);
			else if(c == 3)
				cv::imshow("Path3", image_copy);
			//cv::waitKey(0);


		}


		
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	
     if  ( event == cv::EVENT_LBUTTONDOWN )
     {
     	c++;
     	
          si = y;
          sj = x;
          start.x = si;
          start.y = sj;
     }
     else if  ( event == cv::EVENT_RBUTTONDOWN )
     {
     	c++;
     	
     	  ei = y;
     	  ej = x;
     	  end.x = ei;
     	  end.y = ej;
     }
     if(c == 2)
     {
     	ARAstar();
     	c++;
     }
     
}

int main()
{
	  //Create a window
    cv::namedWindow("Image", 1);
    cv::namedWindow("Path1", 1);

    cv::namedWindow("Path2", 1);
    cv::namedWindow("Path3", 1);
	cv::Size size(200,200);
	cv::resize(image1,image,size);
    cv::imshow("Image", image);

    cv::cvtColor(image, image_copy, CV_BGR2GRAY);
 //   cv::Size size(200,200);//the dst image size,e.g.100x100
//cv::Mat image_copy;//dst image
//src image
//cv::resize(src,image_copy,size);//resize image

      //set the callback function for any mouse event
    
    cv::setMouseCallback("Image", CallBackFunc, NULL);


      //show the image
    //cv::imshow("Path" , image_copy);
	
      // Wait until user press some key
    cv::waitKey(0);

 
    return 0;
}