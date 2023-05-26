#include <iostream>
#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include "ImgProc.h"
#include "Util.h"


using namespace cv;

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
	sf::RenderWindow window(sf::VideoMode(1280,800), "elo");
	
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
	info.setPosition(820.f,50.f);
	 		
	 sf::Text description;
	 description.setFont(font);
	 
	 description.setCharacterSize(24);
	 description.setPosition(820.f,400.f);
	 
	 sf::Text saveLabel;
	 saveLabel.setFont(font);
	 saveLabel.setString("Press P to save output to a file");
	 saveLabel.setPosition(800.f,720.f);
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
	outputSprite.setPosition(0.f,400.f);
	inputSprite.setPosition(0.f,0.f);
	description.setString(loadTextFromFile("0.txt"));
	
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
			else if(event.text.unicode=='p'){
				if(!Util::doesDirectoryExist("images"))
					Util::createDirectory("images");
				if(key==ASCIIFY){ 
				Mat resized_up;
				resize(inputFrame.clone(), resized_up, Size(inputFrame.cols*2, inputFrame.rows*2), INTER_LINEAR);
				ImgProc::asciify(resized_up,resized_up,asciifyDivider*2,asciifyFontSize*2);
				imwrite("images/"+Util::getCurrentTime()+".png", resized_up);
				}
				else imwrite("images/"+Util::getCurrentTime()+".png", outputFrame);
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
		resize(inputFrame,inputFrame,Size(770,400));
		switch(key){
		case NONE:
			outputFrame=inputFrame.clone();
		break;
		case TRESHOLD:
			ImgProc::treshold(inputFrame.clone(),outputFrame,tresholdValue);
	
		break;
		case ABSOLUTE_MOTION:
			ImgProc::absoluteMotion(inputFrame.clone(),previousFrame,outputFrame);

		break;
		case TEMPORAL_FILTERING:
			ImgProc::temporalFiltering(inputFrame.clone(),outputFrame,temporalFilterValue);

		break;
		case CONVOLUTION:
			ImgProc::convolution(inputFrame.clone(),outputFrame,selectedKernel);
		break;
		case SOBEL_EDGE:
			ImgProc::sobelEdge(inputFrame.clone(),outputFrame,kernelSobelHorizontal,kernelSobelVertical);
		break;
		case MORPHOLOGICAL:
			switch(selectedMorphological)
			{
			case 0: ImgProc::morphologicalErosion(inputFrame.clone(),outputFrame,tresholdValue); break;
			case 1: ImgProc::morphologicalDilation(inputFrame.clone(),outputFrame,tresholdValue); break;
			case 2: ImgProc::morphologicalEdge(inputFrame.clone(),outputFrame,tresholdValue);break;
			}
		break;
		case MEDIAN:
			ImgProc::medianFilter(inputFrame.clone(),outputFrame);
		break;
		case ADAPTIVE_TRESHOLD:
			ImgProc::adaptiveTreshold(inputFrame.clone(),outputFrame,1.f);
		break;
		case ASCIIFY:
			ImgProc::asciify(inputFrame.clone(),outputFrame,asciifyDivider,asciifyFontSize);
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
		
		window.clear(sf::Color(40,40,40));
		window.draw(inputSprite);
		window.draw(outputSprite);
		window.draw(info);
		window.draw(description);
		window.draw(saveLabel);
		window.display();
		
		cam>>previousFrame;
		resize(previousFrame,previousFrame,Size(770,400));
	}
	return 0;
}
