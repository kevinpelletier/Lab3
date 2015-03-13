#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <list>
#include <vector>


using namespace std;
using namespace cv;

class CTraitement
{

public:	
	CTraitement(void);
	void DessineCroix(cv::Mat,Point, Vec3b);
	void TrouveCote(Mat, int*);
	void CreerDiagrammeLigneNiveauNoir(int rownumber, Mat frame);
	void DessineCote(Mat frame , int * edges);
	void DessineContour(Mat frame, list<Point> borderPoints);
	void TrouveAngleDessineCroix(Mat frame, vector<Point> corners);
	void CreerDiagrammeSeuilAutomatique(Mat frame);
	void CreerDiagrammeCouleur(Mat frame);
	void EtirerHistogramme(Mat frame);
	Mat ConvertitEnGris(cv::Mat);
	Mat Sobel(Mat frame, int edgeLevel);
	Mat DiagrammeBinarise;
	vector<Point> TrouverRectangle(Mat frame);
	list<Point> AvoirContour(int middleX, int middleY, Mat frame);

	//Moyenne IndexMax Seuillage auto
	int indexMaxS1,indexMaxS2,IndexSeuilAutomatique;
	int AxeX,AxeY;
	float GrossissementY;
	int OccurenceTab[255];
	Point PointPrecedent, PointCourant;

	//filtre
	void FiltreMoyenne(Mat Image,int DimX, int DimY);
};

