#include <iostream>
#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>

using namespace cv;


void treshold(Mat I, Mat& O, unsigned int t){

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

void convolution(Mat I, Mat& O, Mat& K){

}
char* characters=".'`^\",:;Il!i><~+_-?][}{1)(|\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
void asciify(Mat I, Mat& O){
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
		putText(O,std::string(1,characters[int(pTmp[x]/3.7)]),Point(x*5,y*5), FONT_HERSHEY_SIMPLEX ,0.25,(255,255,255),1);
	}
}

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
	info.setString( std::string("0. None\n")+
			std::string("1. Treshold\n")+
	 		std::string("2. Absolute Motion\n")+
	 		std::string("3. Temporal Filtering\n")+
	 		std::string("4. Convolution\n")+
	 		std::string("5. Sobel Edge\n")+
	 		std::string("6. Morphological\n")+
	 		std::string("7. Median\n")+
	 		std::string("8. Adaptive Treshold\n")+
	 		std::string("9. Asciify")
	 		);
	info.setPosition(50.f,400.f);
	 		
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
	Mat idkFrame;
	
	
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

	outputSprite.setPosition(700.f,0.f);
	while(window.isOpen()){
		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type==sf::Event::Closed){
			window.close();
			}
			else if(event.type==sf::Event::TextEntered){
			if(event.text.unicode>='0'&& event.text.unicode<='9')
				key=event.text.unicode;
				
			}
		}
		
		cam>>inputFrame;
		switch(key){
		case NONE:
			outputFrame=inputFrame.clone();
		break;
		case TRESHOLD:
			treshold(inputFrame.clone(),outputFrame,125);
		break;
		case ABSOLUTE_MOTION:
			absoluteMotion(inputFrame.clone(),previousFrame,outputFrame);
		break;
		case TEMPORAL_FILTERING:
			temporalFiltering(inputFrame.clone(),outputFrame,0.3);
		break;
		case CONVOLUTION:
		
		break;
		case SOBEL_EDGE:
		
		break;
		case MORPHOLOGICAL:
		
		break;
		case MEDIAN:
		
		break;
		case ADAPTIVE_TRESHOLD:
		
		break;
		case ASCIIFY:
			asciify(inputFrame.clone(),outputFrame);
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
		window.display();
		
		cam>>previousFrame;
	}
	return 0;
}
