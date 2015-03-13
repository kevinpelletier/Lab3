#include "stdafx.h"
#include "device_launch_parameters.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "Traitement.h"

using namespace std;
using namespace cv;

Mat Photo;
Mat PhotoGrise;
Vec3b PixelBgr;
bool ModePhoto;
int SourisX;
int SourisY;
bool hPresser,dPresser,ePresser;
Vec3b PixelCourantCouleur;
Mat image;

#define TAB_KEY 9
#define ENTER_KEY 13
#define ESCAPE_KEY 27
#define SPACE_KEY 32
#define C_KEY 99
#define D_Key 100
#define E_KEY 101
#define F_KEY 102
#define G_KEY 103
#define H_KEY 104
#define M_KEY 109
#define P_KEY 112
#define R_KEY 114
#define S_KEY 115
#define T_KEY 116

CTraitement* Traitement;


bool Fichier_exist(string NomFichier)
{
	ifstream infile(NomFichier.c_str());
	return infile.good();
}

//Convertit meme si matrice de base en couleur
void ConversionTotalGris(CTraitement* Traitement)
{
	if(Photo.channels() == 3)	PhotoGrise = Traitement->ConvertitEnGris(Photo);
	else						PhotoGrise = Photo;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_RBUTTONDOWN && ModePhoto) //Right click
	{
		PixelBgr = Photo.at<Vec3b>(Point(x, y));
		printf("Couleur sauvegardee\n");				
	}
	else if  ( event == EVENT_LBUTTONDOWN && ModePhoto) //Left click
	{
		if(hPresser)
		{
			ConversionTotalGris(Traitement);
			Traitement->CreerDiagrammeLigneNiveauNoir(SourisY,PhotoGrise);
			hPresser = false;
			imshow("GRAPHIQUE NOIR ET BLANC", PhotoGrise);
		}
		else
		{
			Traitement->DessineCroix(Photo,Point(x,y),PixelBgr);
		}		
		
		vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(9);
		imwrite("snapshot.png", Photo, compression_params);
	}
	else if (event == EVENT_MOUSEMOVE) //Mouse move
	{
		if (x >= 0 && x < image.cols && y >= 0 && y < image.rows)
		{
			SourisX = x;
			SourisY = y;
			PixelCourantCouleur = image.at<Vec3b>(Point(x, y));
		}
	}
}

int main(int argc, char** argv)
{
	Traitement = new CTraitement();
	SourisX = 0;
	SourisY = 0;
	Mat cotes;
	ModePhoto = false;
	hPresser = false;
	dPresser = false;
	ePresser = false;
	PixelBgr = (0,0,0);
	PixelCourantCouleur = (0,0,0);
	Point centerPoint = Point();
	vector<Point> corners;
	
	cotes = imread("breadfish.png", CV_LOAD_IMAGE_COLOR);

		imshow("cotes", cotes);

		VideoCapture cap(0); // open the default camera
		if(!cap.isOpened())  // check if we succeeded
			return -1;
		Photo = Mat(480,640,CV_8UC3);
		Photo.setTo(0);
		image = Mat(480,640,CV_8UC3);
		image.setTo(0);
		int key=0;
		string chemin;
		bool fichierCharge=false;
		char Message[200];
		namedWindow("image", 1);
		
		//Quit lapplication
		while(key != ESCAPE_KEY)
		{
			cap >> image; 
			memset(Message,0,200);
			sprintf(Message,"(%i, %i) = (%i, %i, %i)", SourisX, SourisY, PixelCourantCouleur[2], PixelCourantCouleur[1], PixelCourantCouleur[0]); //text out
			setMouseCallback("image", CallBackFunc, NULL);
			key = waitKey(30);

			//Charger nouveau fichier
			if(key==F_KEY)
			{
				cout << "Veuillez entrer le nom du fichier que vous desirez charger. (nom.extension)" << endl;
				getline(cin, chemin);
				if (Fichier_exist(chemin))
				{
					Photo = imread(chemin, CV_LOAD_IMAGE_COLOR);
					cout << "Le fichier a ete charge." << endl;
					fichierCharge = true;
				}
				else
				{
					cout << "Le fichier na pas ete trouve." << endl;
					chemin="";
				}
			}
			
			//Prends une photo
			if(key==ENTER_KEY)
			{
				vector<int> compression_params;
				compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
				compression_params.push_back(9);
				imwrite("snapshot.png", image, compression_params);
				image.copyTo(Photo);
			}	

			//mode image
			if (key==TAB_KEY || key==ENTER_KEY || (key==F_KEY && fichierCharge))
			{
				ModePhoto = true;
				key=0;
				while(key != TAB_KEY) 
				{
					key = waitKey(30);

					switch(key)
					{
						//Trace Ligne touge
					case R_KEY: line(Photo, Point(0,0), Point(Photo.cols, Photo.rows), cv::Scalar(0, 0, 200), 1, 1); 
						break; 

						//Filtre de sobel
					case S_KEY:
						ConversionTotalGris(Traitement);
						PhotoGrise = Traitement->Sobel(PhotoGrise, 8);
						imshow("SOBEL",PhotoGrise);
						break;

						//Efface l<image
					case SPACE_KEY:Photo.setTo(0); 
						break; 

						//Convertit en gris
					case G_KEY:
						PhotoGrise = Traitement->ConvertitEnGris(Photo);
						imshow("CONVERSION GRIS",PhotoGrise);
						break;

					//Detection carre
					case C_KEY:
						ConversionTotalGris(Traitement);
						int cotes[4];
						Traitement->TrouveCote(PhotoGrise, cotes);
						Traitement->DessineCote(PhotoGrise,cotes);
						centerPoint = Point(cotes[0]+((cotes[1]-cotes[0])/2),cotes[2]+((cotes[3] - cotes[2])/2));

						//left,right,top,bottom
						Traitement->DessineCroix(PhotoGrise,centerPoint,Vec3b(255,255,255));		
						imshow("DETECTION CARRE",PhotoGrise);
					break;

					//Histogramme simple 
					case H_KEY:
						hPresser=true;
					break;

					//Histogramme Pourcentage noir/blanc
					case P_KEY:
						ConversionTotalGris(Traitement);
						Traitement->CreerDiagrammeSeuilAutomatique(PhotoGrise);
						imshow("GRAPHIQUE NOIR ET BLANC", PhotoGrise);
					break;

					//Histogramme SeuilAuto Etirer
					case E_KEY:
						ConversionTotalGris(Traitement);
						Traitement->CreerDiagrammeSeuilAutomatique(PhotoGrise);
						Traitement->CreerDiagrammeSeuilAutomatique(PhotoGrise);
						Traitement->EtirerHistogramme(PhotoGrise);
					break;

					//Histogramme couleur
					case D_Key:
						Traitement->CreerDiagrammeCouleur(Photo);
					break;

					//Croix et angle
					case T_KEY:
						ConversionTotalGris(Traitement);
						corners = Traitement->TrouverRectangle(PhotoGrise);						
						Traitement->TrouveAngleDessineCroix(PhotoGrise, corners);
						imshow("CROIX ET ANGLE", PhotoGrise);
					break;

					case M_KEY:
						Traitement->FiltreMoyenne(Photo,3,3);
						break;

					}

					

					imshow("image", Photo);
				}
				ModePhoto = false;
			}
			putText(image, Message, Point(10,450), CV_FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(0,0,200));
			imshow("image", image);
		}
		
		return 0;
}



