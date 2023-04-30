#include <iostream>
#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <fstream>
#include <algorithm>

using namespace cv;


void treshold(Mat I, Mat& O, unsigned int t){

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

void absoluteMotion(Mat I, Mat prevI, Mat& O){

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

void temporalFiltering(Mat I, Mat& O, float t){
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

void convolution(Mat I, Mat& O, float* K){
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
			float sum=0.f;
			for(int n=-1; n<2; n++)
				for(int m=-1; m<2; m++)
					sum+=float(*I.ptr<uchar>(y+n,x+m)) * K[(n+1)*3+(m+1)];
			*O.ptr<uchar>(y,x)=sum;
		}
	}
}

void sobelEdge(Mat I, Mat& O, float* kHorizontal, float* kVertical){
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

//These 3 functions are a result of late night thinking
void morphologicalDilation(Mat I, Mat& O, uchar t){
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;
treshold(I,I,t);


for(int y=0; y<nRows; y++){
		for(int x=0; x<nCols; x++){	
			int sum=0;
			for(int n=-1; n<2; n++)
				for(int m=-1; m<2; m++)
					sum += (*I.ptr<uchar>(y+n,x+m));
			if(sum) *O.ptr<uchar>(y,x)=255;
			else *O.ptr<uchar>(y,x)=0;
		}
	}
}

void morphologicalErosion(Mat I, Mat& O, uchar t){
//
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;

treshold(I,I,t);

for(int y=0; y<nRows; y++){
		for(int x=0; x<nCols; x++){	
			int sum=0;
			for(int n=-1; n<2; n++)
				for(int m=-1; m<2; m++)
					sum += (*I.ptr<uchar>(y+n,x+m)) && 255;
			if(sum<9) *O.ptr<uchar>(y,x)=0;
			else *O.ptr<uchar>(y,x)=255;
		}
	}
}

void morphologicalEdge(Mat I, Mat& O, uchar t){
CV_Assert(I.depth()==CV_8U);
CV_Assert(O.depth()==CV_8U);

if(I.channels()!=1)
cvtColor(I,I,COLOR_BGR2GRAY);
if(O.channels()!=1)
cvtColor(O,O,COLOR_BGR2GRAY);

int nRows=I.rows;
int nCols=I.cols;

treshold(I,I,t);
for(int y=0; y<nRows; y++){
		for(int x=0; x<nCols; x++){	
			int sum=0;
			for(int n=-1; n<2; n++)
				for(int m=-1; m<2; m++)
					sum += (*I.ptr<uchar>(y+n,x+m)) && 255;
			if(sum==8) *O.ptr<uchar>(y,x)=255;
			else *O.ptr<uchar>(y,x)=0;
		}
	}
}

void medianFilter(Mat I, Mat& O){
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

void adaptiveTreshold(Mat I, Mat &O, float t){
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
			float sum=0;
			for(int n=-2; n<3; n++)
				for(int m=-2; m<3; m++)
					sum+=(*I.ptr<uchar>(y+n,x+m));
			sum/=25.f;
			
			*O.ptr<uchar>(y,x) = (*I.ptr<uchar>(y,x)) < (sum*t) ? 0.f : 255.f;
		}
	}
	
}
char* characters=".'`^\",:;Il!i><~+_-?][}{1)(|\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
void asciify(Mat I, Mat& O, int divider, float fontSize){
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
		putText(O,std::string(1,characters[int(pTmp[x]/3.7)]),Point(x*divider,y*divider), FONT_HERSHEY_SIMPLEX ,fontSize,(255,255,255),1);
	}
}

}


std::string loadTextFromFile(std::string fileName){
	std::string line;
	std::string descriptionString;
	std::ifstream desc(fileName);
	while(getline(desc,line)){
		descriptionString+=line;
		descriptionString+="\n";
	}
	desc.close();
	return descriptionString;
}
int main()
{
	//create window
	sf::RenderWindow window(sf::VideoMode(1600,1000), "elo");
	
	//sfml objects for displaying
	sf::Image inputImage;
	sf::Texture inputTexture;
	sf::Sprite inputSprite;
	sf::Image outputImage;
	sf::Texture outputTexture;
	sf::Sprite outputSprite;
	
	//font
	sf::Font font;
	if(!font.loadFromFile("font.ttf")){
	std::cout<<"COULD NOT OPEN FONT";
	return -1;
	}
	sf::Text info;
	info.setFont(font);
	info.setString(loadTextFromFile("info.txt"));
	info.setPosition(950.f,50.f);
	 		
	 sf::Text description;
	 description.setFont(font);
	 
	 description.setCharacterSize(24);
	 description.setPosition(950.f,400.f);
	 sf::Text inputLabel;
	 inputLabel.setFont(font);
	 inputLabel.setString("INPUT");
	 inputLabel.setPosition(0.f,0.f);
	 inputLabel.setCharacterSize(40);
	 sf::Text outputLabel;
	 outputLabel.setFont(font);
	 outputLabel.setString("OUTPUT");
	 outputLabel.setPosition(0.f,500.f);
	 outputLabel.setCharacterSize(40);
	//capture camera
	VideoCapture cam;
	cam.open(0);
	if(!cam.isOpened()){
		std::cout<<"COULD NOT FIND A CAMERA";
		return -1;
	}
	
	//opencv Matrices
	Mat inputFrame;
	Mat previousFrame;
	Mat outputFrame;
	
	unsigned int tresholdValue=127;
	
	float temporalFilterValue=0.3;
	
	int asciifyDivider=5;
	float asciifyFontSize=0.25;
	//enum that represents current image processing mode
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
	
	char key=NONE;
	cam>>inputFrame;
	outputSprite.setPosition(0.f,550.f);
	inputSprite.setPosition(0.f,50.f);
	description.setString(loadTextFromFile("0.txt"));
	
	float blurKernel[9]={
	0.f, 0.125f, 0.f,
	0.125f, 0.5f, 0.125f,
	0.f, 0.125f, 0.f,
	};
	
	float sharpenKernel[9]={
	0.f, -1.f, 0.f,
	-1.f, 5.0f, -1.f,
	0.f, -1.f, 0.f,
	};
	
	float illusionKernel[9]={
	0.f, 10.f, 0.f,
	0.f, 1.f, 0.f,
	0.f, 10.f, 0.f,
	};
	
	float kernelSobelVertical[9]={
	-1.f, 0.f, 1.f,
	-2.f, 0.f, 2.f,
	-1.f, 0.f, 1.f,
	};
	float kernelSobelHorizontal[9]={
	-1.f, -2.f, -1.f,
	0.f, 0.f, 0.f,
	1.f, 2.f, 1.f,
	};
	float *selectedKernel = blurKernel;
	
	int selectedMorphological=0;
	while(window.isOpen()){
		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type==sf::Event::Closed){
			window.close();
			}
			else if(event.type==sf::Event::TextEntered){
			if(event.text.unicode>='0'&& event.text.unicode<='9'){
				key=event.text.unicode;
				description.setString(loadTextFromFile(std::string(1,key)+".txt"));
				}
			switch(key){
					case TRESHOLD:
						
						if(event.text.unicode=='e' && tresholdValue>0) tresholdValue--;
						else if(event.text.unicode=='q' && tresholdValue<255) tresholdValue++;
					break;
					case TEMPORAL_FILTERING:
						if(event.text.unicode=='e' && temporalFilterValue<=0.95f) temporalFilterValue+=0.05f;
						else if(event.text.unicode=='q' && temporalFilterValue >=0.05f) temporalFilterValue-=0.05f;
					break;
					case CONVOLUTION:
						if(event.text.unicode=='q') selectedKernel=blurKernel;
						else if(event.text.unicode=='w') selectedKernel=sharpenKernel;
						else if(event.text.unicode=='e') selectedKernel=illusionKernel;
					break;
					case MORPHOLOGICAL:
						if(event.text.unicode=='e' && tresholdValue>0) tresholdValue--;
						else if(event.text.unicode=='q' && tresholdValue<255) tresholdValue++;
						if(event.text.unicode=='a') selectedMorphological=0;
						else if(event.text.unicode=='s') selectedMorphological=1;
						else if(event.text.unicode=='d') selectedMorphological=2;
					break;
					case ASCIIFY:
						if(event.text.unicode=='e' && asciifyDivider>1) asciifyDivider--;
						else if(event.text.unicode=='q' && asciifyDivider<20) asciifyDivider++;
						if(event.text.unicode=='d' && asciifyFontSize<=1) asciifyFontSize+=0.02;
						else if(event.text.unicode=='a' && asciifyFontSize>=0.1) asciifyFontSize-=0.02;
					break;
					
				}
			}
		}
		
		cam>>inputFrame;
		resize(inputFrame,inputFrame,Size(800,450));
		switch(key){
		case NONE:
			outputFrame=inputFrame.clone();
		break;
		case TRESHOLD:
			treshold(inputFrame.clone(),outputFrame,tresholdValue);
	
		break;
		case ABSOLUTE_MOTION:
			absoluteMotion(inputFrame.clone(),previousFrame,outputFrame);

		break;
		case TEMPORAL_FILTERING:
			temporalFiltering(inputFrame.clone(),outputFrame,temporalFilterValue);

		break;
		case CONVOLUTION:
			convolution(inputFrame.clone(),outputFrame,selectedKernel);
		break;
		case SOBEL_EDGE:
			sobelEdge(inputFrame.clone(),outputFrame,kernelSobelHorizontal,kernelSobelVertical);
		break;
		case MORPHOLOGICAL:
			switch(selectedMorphological)
			{
			case 0:morphologicalErosion(inputFrame.clone(),outputFrame,tresholdValue); break;
			case 1: morphologicalDilation(inputFrame.clone(),outputFrame,tresholdValue); break;
			case 2: morphologicalEdge(inputFrame.clone(),outputFrame,tresholdValue);break;
			}
		break;
		case MEDIAN:
			medianFilter(inputFrame.clone(),outputFrame);
		break;
		case ADAPTIVE_TRESHOLD:
			adaptiveTreshold(inputFrame.clone(),outputFrame,1.f);
		break;
		case ASCIIFY:
			asciify(inputFrame.clone(),outputFrame,asciifyDivider,asciifyFontSize);
		break;
		default:
		
		break;
		}
		if(inputFrame.channels()==3) cvtColor(inputFrame,inputFrame,COLOR_BGR2RGBA);
		else cvtColor(inputFrame,inputFrame,COLOR_GRAY2RGBA);
		if(outputFrame.channels()==3) cvtColor(outputFrame,outputFrame,COLOR_BGR2RGBA);
		else cvtColor(outputFrame,outputFrame,COLOR_GRAY2RGBA);
		inputImage.create(inputFrame.cols,inputFrame.rows,inputFrame.ptr());
		outputImage.create(outputFrame.cols,outputFrame.rows,outputFrame.ptr());
		if(!inputTexture.loadFromImage(inputImage)) break;
		if(!outputTexture.loadFromImage(outputImage)) break;
		inputSprite.setTexture(inputTexture);
		outputSprite.setTexture(outputTexture);
		
		window.clear(sf::Color(80,80,80));
		window.draw(inputSprite);
		window.draw(outputSprite);
		window.draw(info);
		window.draw(description);
		window.draw(inputLabel);
		window.draw(outputLabel);
		window.display();
		
		cam>>previousFrame;
		resize(previousFrame,previousFrame,Size(800,450));
	}
	return 0;
}
