#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat image1 = cv::imread("/home/satyesh/code/astar/img.jpg");
cv::Mat image;

cv::Mat image_copy;

int si; int sj; int ei; int ej, c = 0;
float **dsrc;
int ep = 10;
/*struct node
{
	int r;
	int c;
};
*/

float h(cv::Point p)
{
	return ep * sqrt((p.x-ei) * (p.x-ei) + (p.y - ej) * (p.y - ej) );
}

 struct compare  
 {  
   bool operator()(const cv::Point& l, const cv::Point& r)  
   {  
       return (dsrc[l.x][l.y] + h(l)) > (dsrc[r.x][r.y] + h(r));  
   }  
 };  

typedef struct compare compare;	
	
																																																																																																																																																																																																																																																																																																	
// cv::Point MinDist(int **dsrc, int rows, int cols, bool **indexset)
// {
// 	int i, j, min = INT_MAX;
// 	cv::Point m;

// 	for(i = 0; i < rows; i++ )
// 		{
// 			for (j = 0; j < cols; ++j)
// 			{
// 				if(dsrc[i][j] >= 0 && dsrc[i][j] < min && indexset[i][j] == false)
// 				{
// 					min = dsrc[i][j];
// 					m.x = i;
// 					m.y = j;
// 				}
// 			}
// 		}
// 	return m;

// }

void Astar(cv::Mat G ) //assumes that the image is in graysclae
{
		//cv::Mat visited = G;
		//cv::Mat dsrc(G.rows(), G.cols(), CV_8UC1, Scalar(255));
		//int visited[G.rows][G.cols];

		dsrc = new float*[G.rows];
		for(int i = 0; i < G.rows; ++i)
    		dsrc[i] = new float[G.cols];

    	cv::Point node;
		cv::Point p[G.rows][G.cols];
		cv::Point min;
		float dist;
		int i, j, temp;

     	std::priority_queue<cv::Point, std::vector<cv::Point>, compare> pq;


		// bool** indexset = new bool*[G.rows];
		// for(int i = 0; i < G.rows; ++i)
  //   		indexset[i] = new bool[G.cols];

		for(i = 0; i < G.rows; i++ )
		{
			for (j = 0; j < G.cols; ++j)
			{
				//visited[i][j] = 0;
				if(G.at<uchar>(i,j) <= 100)
					dsrc[i][j] = -1;
				else
					dsrc[i][j] = INT_MAX;

				// indexset[i][j] = false;
				p[i][j].x = -1;
				p[i][j].y = -1;
			}
		}
		
		dsrc[si][sj] = 0;
		std::cout<<si<<","<<sj<<" "<<ei<<","<<ej<<"a\n";

		min.x = si;
		min.y = sj;
		pq.push(min);

		while(pq.top().x != ei || pq.top().y != ej)
		{
			
			min = pq.top();
			pq.pop();
			//min = MinDist(dsrc, G.rows, G.cols, indexset);
			//indexset[min.x][min.y] = true;
			image_copy.at<uchar>(min.x,min.y) = 80;
			//visited[min.x][min.y] = 1;
			std::cout<<min.x<<" "<<min.y<<"b\n";
			for(i = -1; i <= 1; i++)
			{
				for(j=-1; j <= 1; j++)
				{
					if(min.x == 0 && i == -1)
						continue;
					if(min.y == 0 && j == -1)
						continue;
					if(min.x == G.rows-1 && i == 1)
						continue;
					if(min.y == G.cols-1 && j == 1)
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
						pq.push(node);
					}
					
					//visited[min.x + i][min.y + j] = 1;
				}
			}
			cv::imshow("Path" , image_copy);
			cv::waitKey(1);
		}

		std::cout<<"c\n";
		i = ei;
		j = ej;
	//			std::cout<<p[i][j].x<<","<<p[i][j].y<<"a\n";

		while(i != si || j != sj)
		{
			std::cout<<i<<","<<j<<"\n";
			image_copy.at<uchar>(i, j) = 0;
			temp = p[i][j].x;
			j = p[i][j].y;
			i = temp;

		}
		std::cout<<"D\n";
	cv::imshow("Path", image_copy);
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	
     if  ( event == cv::EVENT_LBUTTONDOWN )
     {
     	c++;
     	
          si = y;
          sj = x;
     }
     else if  ( event == cv::EVENT_RBUTTONDOWN )
     {
     	c++;
     	
     	  ei = y;
     	  ej = x;
     }
     if(c == 2)
     {
     	Astar(image_copy);
     	c++;
     }
     
}


int main()
{
	  //Create a window
    cv::namedWindow("Image", 1);
    cv::namedWindow("Path", 1);

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
    cv::imshow("Path" , image_copy);
	
      // Wait until user press some key
    cv::waitKey(0);

 
    return 0;
}