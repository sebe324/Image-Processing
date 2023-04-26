#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;


void treshold(Mat& I, Mat& O, unsigned int t){

CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

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

void absoluteMotion(Mat& I, Mat& prevI, Mat& O){

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

void temporalFiltering(Mat& I, Mat& O, float t){
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

void convolution(Mat& I, Mat& O, Mat& K){

}
char* characters=".'`^\",:;Il!i><~+_-?][}{1)(|\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
void asciify(Mat& I, Mat& O){
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;

Mat tmp;
resize(I,tmp,Size(nCols/5,nRows/5));

O = Scalar(0,0,0);
int nRowsTmp=tmp.rows;
int nColsTmp=tmp.cols;
uchar* pOutput;
uchar* pTmp;

for(int y=0; y<nRowsTmp; y++){
	pTmp=tmp.ptr<uchar>(y);
	for(int x=0; x<nColsTmp; x++){
		putText(O,std::string(1,characters[int(pTmp[x]/3.7)]),Point(x*10,y*10), FONT_HERSHEY_SIMPLEX ,0.5,(255,255,255),1);
	}
}

}

int main()
{

	VideoCapture cam;
	cam.open(0);
	namedWindow("Window", WINDOW_AUTOSIZE);
	if(!cam.isOpened()){
		std::cout<<"COULD NOT FIND A CAMERA";
		return -1;
	}
	Mat inputFrame;
	Mat previousFrame;
	Mat outputFrame;
	Mat idkFrame;
	enum{
	NONE='0',
	TRESHOLD,
	ABSOLUTE_MOTION,
	TEMPORAL_FILTERING,
	CONVOLUTION,
	SOBEL_EDGE,
	MORPHOLOGICAL,
	MEDIAN,
	ADAPTIVE_TRESHOLD,
	ASCIIFY,
	};
	
	char key=TRESHOLD;
	cam>>inputFrame;
	outputFrame=inputFrame.clone();
	resize(outputFrame,idkFrame,Size(inputFrame.cols*2,inputFrame.rows*2));
	
	while(true){
	cam>>inputFrame;
	char input=waitKey(25);
	if(input!=-1) key=input;
	switch(key){
		case TRESHOLD:
		treshold(inputFrame,outputFrame,127);
		resize(outputFrame,idkFrame,Size(inputFrame.cols*2,inputFrame.rows*2));
		break;
		case ABSOLUTE_MOTION:
			absoluteMotion(inputFrame,previousFrame,outputFrame);
			resize(outputFrame,idkFrame,Size(inputFrame.cols*2,inputFrame.rows*2));
		break;
		case TEMPORAL_FILTERING:
			temporalFiltering(inputFrame,outputFrame,0.1f);
			resize(outputFrame,idkFrame,Size(inputFrame.cols*2,inputFrame.rows*2));
		break;
		case ASCIIFY:
		asciify(inputFrame,idkFrame);
		break;
		case NONE:
			cam>>outputFrame;
			resize(outputFrame,idkFrame,Size(inputFrame.cols*2,inputFrame.rows*2));
		break;
		case 'x':
			return -1;
		break;
	}
	imshow("Window",idkFrame);
	previousFrame=inputFrame.clone();
	}


	waitKey(0);
	return 0;
}
