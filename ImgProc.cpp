#include "ImgProc.h"


using namespace cv;



void ImgProc::treshold(Mat I, Mat& O, unsigned int t)
{
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);
int nRows=I.rows;
int nCols=I.cols;

uchar* pInput;
uchar* pOutput;

	for(int y=0; y<nRows; y++){
		pInput=I.ptr<uchar>(y);
		pOutput=O.ptr<uchar>(y);
		for(int x=0;x<nCols; x++){
			if(pInput[x]<t) pOutput[x]=0;
			else pOutput[x]=255;
		}
	}
}

void ImgProc::absoluteMotion(Mat I, Mat prevI, cv::Mat& O)
{
CV_Assert(I.depth()==CV_8U);
CV_Assert(prevI.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

cvtColor(I,I,COLOR_BGR2GRAY);
if(prevI.channels()!=1)
cvtColor(prevI,prevI,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;

uchar *pInput;
uchar *pPreviousInput;
uchar *pOutput;
for(int y=0; y<nRows; y++){
	pInput=I.ptr<uchar>(y);
	pPreviousInput=prevI.ptr<uchar>(y);
	pOutput=O.ptr<uchar>(y);
	for(int x=0; x<nCols; x++){
		pOutput[x]=abs(pInput[x]-pPreviousInput[x]);
	}
}
}

void ImgProc::temporalFiltering(Mat I, Mat& O, float t)
{
	CV_Assert(I.depth()==CV_8U);
	CV_Assert(O.depth()==CV_8U);
	if(O.channels()!=1)
	cvtColor(O,O,COLOR_BGR2GRAY);
	cvtColor(I,I,COLOR_BGR2GRAY);
	
	int nRows=I.rows;
	int nCols=I.cols;
	
	uchar* pInput;
	uchar* pOutput;
	
		for(int y=0; y<nRows; y++){
		pInput=I.ptr<uchar>(y);
		pOutput=O.ptr<uchar>(y);
		for(int x=0;x<nCols; x++){
			float a=pInput[x]-pOutput[x];
			a*=t;
			pOutput[x]+=a;
		}
	}	
	
}

void ImgProc::convolution(Mat I, Mat& O, float* K)
{
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;
	

	for(int y=1; y<nRows-1; y++){
		for(int x=1; x<nCols-1; x++){	
			float sum=0.f;
			for(int n=-1; n<2; n++)
				for(int m=-1; m<2; m++)
					sum+=float(*I.ptr<uchar>(y+n,x+m)) * K[(n+1)*3+(m+1)];
			*O.ptr<uchar>(y,x)=sum;
		}
	}
}

void ImgProc::sobelEdge(Mat I, Mat& O, float* kHorizontal, float* kVertical)
{
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;
	

	for(int y=1; y<nRows-1; y++){
		for(int x=1; x<nCols-1; x++){	
			float sumH=0.f;
			float sumV=0.f;
			for(int n=-1; n<2; n++)
				for(int m=-1; m<2; m++){
					sumH+=float(*I.ptr<uchar>(y+n,x+m)) * kHorizontal[(n+1)*3+(m+1)];
					sumV+=float(*I.ptr<uchar>(y+n,x+m)) * kVertical[(n+1)*3+(m+1)];
					}
			*O.ptr<uchar>(y,x)=fabs((sumH+sumV)/2.f);
		}
	}
}

void ImgProc::morphologicalDilation(Mat I, Mat& O, uchar t)
{
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;
treshold(I,I,t);


for(int y=1; y<nRows-1; y++){
		for(int x=1; x<nCols-1; x++){	
			int sum=0;
			for(int n=-1; n<2; n++)
				for(int m=-1; m<2; m++)
					sum += (*I.ptr<uchar>(y+n,x+m));
			if(sum) *O.ptr<uchar>(y,x)=255;
			else *O.ptr<uchar>(y,x)=0;
		}
	}
}

void ImgProc::morphologicalErosion(Mat I, Mat& O, uchar t)
{
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;

treshold(I,I,t);

for(int y=1; y<nRows-1; y++){
		for(int x=1; x<nCols-1; x++){	
			int sum=0;
			for(int n=-1; n<2; n++)
				for(int m=-1; m<2; m++)
					sum += (*I.ptr<uchar>(y+n,x+m)) && 255;
			if(sum<9) *O.ptr<uchar>(y,x)=0;
			else *O.ptr<uchar>(y,x)=255;
		}
	}
}

void ImgProc::morphologicalEdge(Mat I, Mat& O, uchar t)
{
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;

treshold(I,I,t);
for(int y=1; y<nRows-1; y++){
		for(int x=1; x<nCols-1; x++){	
			int sum=0;
			for(int n=-1; n<2; n++)
				for(int m=-1; m<2; m++)
					sum += (*I.ptr<uchar>(y+n,x+m)) && 255;
			if(sum==8) *O.ptr<uchar>(y,x)=255;
			else *O.ptr<uchar>(y,x)=0;
		}
	}
}

void ImgProc::medianFilter(Mat I, Mat& O)
{
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;

for(int y=0; y<nRows; y++){
		for(int x=0; x<nCols; x++){	
		float v[25];
			for(int n=-2; n<3; n++)
				for(int m=-2; m<3; m++)
					v[(n+2)*5+(m+2)]=(*I.ptr<uchar>(y+n,x+m));
					
			std::sort(std::begin(v),std::end(v));
			*O.ptr<uchar>(y,x)=v[12];
		}
	}
}

void ImgProc::adaptiveTreshold(Mat I, Mat &O, float t)
{
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);


int nRows=I.rows;
int nCols=I.cols;

for(int y=2; y<nRows-2; y++){
		for(int x=2; x<nCols-2; x++){	
			float sum=0;
			for(int n=-2; n<3; n++)
				for(int m=-2; m<3; m++)
					sum+=(*I.ptr<uchar>(y+n,x+m));
			sum/=25.f;
			
			*O.ptr<uchar>(y,x) = (*I.ptr<uchar>(y,x)) < (sum*t) ? 0.f : 255.f;
		}
	}
	
}

const char* characters=".'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

void ImgProc::asciify(Mat I, Mat& O, int divider, float fontSize, int thickness)
{
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;

Mat tmp;
resize(I,tmp,Size(nCols/divider,nRows/divider));

O = Scalar(0,0,0);
int nRowsTmp=tmp.rows;
int nColsTmp=tmp.cols;
uchar* pOutput;
uchar* pTmp;


for(int y=0; y<nRowsTmp; y++){
	pTmp=tmp.ptr<uchar>(y);
	for(int x=0; x<nColsTmp; x++){
		putText(O,std::string(1,characters[int(pTmp[x]/3.7)]),Point(x*divider,y*divider), FONT_HERSHEY_SIMPLEX ,fontSize,(255,255,255),thickness);
	}
}

}
