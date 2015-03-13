#include "StdAfx.h"
#include "Traitement.h"


CTraitement::CTraitement()
{


}

void CTraitement::DessineCroix(Mat image, Point p, Vec3b pixcouleur)
{
	if(p.x >= 0 && p.y >= 0)
	{
		line(image, Point(p.x-10, p.y), Point(p.x+10, p.y), Scalar(pixcouleur[0], pixcouleur[1], pixcouleur[2]), 3,1);
		line(image, Point(p.x,p.y-10), Point(p.x, p.y+10), Scalar(pixcouleur[0], pixcouleur[1], pixcouleur[2]),3,1);
	}
}

Mat CTraitement::ConvertitEnGris(Mat image)
{
	cv::Mat imageGris;
	cv::cvtColor(image, imageGris, CV_BGR2GRAY);
	return imageGris;
}

void CTraitement::FiltreMoyenne(Mat image,int DimX, int DimY)
{
	Mat Resultante = ConvertitEnGris(image);
	int PixelEnCours;
	int* PixelEnvironnant = new int[DimX * DimY];
	int* KernelMatrice = new int[DimX * DimY];

	KernelMatrice[0] = 1;
	KernelMatrice[1] = 1;
	KernelMatrice[2] = 1;
	KernelMatrice[3] = 1;
	KernelMatrice[4] = 1;
	KernelMatrice[5] = 1;
	KernelMatrice[6] = 1;
	KernelMatrice[7] = 1;
	KernelMatrice[8] = 1;

		for (int col = 1; col<Resultante.cols-1;col++)
		{
			for (int row = 1; row<Resultante.rows-1;row++)
			{
					PixelEnCours = image.at<uchar>(row,col);
					
					/*if(DimX == 3)
					{
						PixelEnvironnant[0] = image.at<uchar>(row - 1,col - 1) * KernelMatrice[0];
						PixelEnvironnant[1] = image.at<uchar>(row + 0,col - 1) * KernelMatrice[1];
						PixelEnvironnant[2] = image.at<uchar>(row + 1,col - 1) * KernelMatrice[2];
						PixelEnvironnant[3] = image.at<uchar>(row - 1,col + 0) * KernelMatrice[3];
						PixelEnvironnant[4] = PixelEnCours					   * KernelMatrice[4];
						PixelEnvironnant[5] = image.at<uchar>(row + 1,col + 0) * KernelMatrice[5];
						PixelEnvironnant[6] = image.at<uchar>(row - 1,col + 1) * KernelMatrice[6];
						PixelEnvironnant[7] = image.at<uchar>(row + 0,col + 1) * KernelMatrice[7];
						PixelEnvironnant[8] = image.at<uchar>(row + 1,col + 1) * KernelMatrice[8];
					}
					
					int Somme =0;
					for(int IndexSomme =0; IndexSomme < DimX * DimY; IndexSomme++)
					{
						Somme += PixelEnvironnant[IndexSomme];
					}*/

					//Somme /= (DimX * DimY);
					Resultante.at<uchar>(Point(col,row)) = PixelEnCours;
			
			}
		}

		imshow(" Normal",image);
	imshow("FiltreMoyenne",Resultante);
}


void CreerDiagrammeBase(Mat Diagramme, int DecalageX, int DecalageY, bool Horizontal)
{
	int DecalageInitialeGraduation = DecalageX;
	int PasEntreGraduation = 100;
	int NbGraduation = 6;
	string Texte[6] = {"0","50","100","150","200","250"};

	//Ligne Axe
	line(Diagramme, Point(DecalageX, 20), Point(DecalageX, DecalageY), Scalar(0), 2,1);
	line(Diagramme, Point(DecalageX, DecalageY), Point(620, DecalageY), Scalar(0), 2,1);

	if(Horizontal)
	{
		int DecalageMotBarre = 15;
		for (int i = 0; i < NbGraduation; i++)
		{
			//graduation
			line(Diagramme, 
					Point(i * PasEntreGraduation + DecalageInitialeGraduation, DecalageY), 
					Point(i * PasEntreGraduation + DecalageInitialeGraduation, DecalageY - DecalageMotBarre), 
					Scalar(0), 2,1);

			//Texte
			putText(Diagramme, 
					Texte[i],   
					Point(i * PasEntreGraduation + DecalageInitialeGraduation,DecalageY + DecalageMotBarre),
					CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,200));
		}
	}
	else
	{
		DecalageInitialeGraduation = 30;
		PasEntreGraduation = 86;
		int LongeurGraduation = 8;
		int DecalageMotX = 5;
		int DecalageMotY = DecalageInitialeGraduation + 4;

		for(int i = 0; i < NbGraduation; i++)
		{
			//graduation
			line(Diagramme, 
				Point(DecalageX, ((NbGraduation-1) - i) * PasEntreGraduation + DecalageInitialeGraduation),
				Point(DecalageX + LongeurGraduation, ((NbGraduation - 1) - i) * PasEntreGraduation + DecalageInitialeGraduation), 
				Scalar(0), 2,1);

			//Texte
			putText(Diagramme, Texte[i], Point(DecalageMotX,((NbGraduation-1) - i) * PasEntreGraduation + DecalageMotY), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,200));
		}
	}
}

Point CreerPointGraphique(int DecalageX, int DecalageY, double GrossissementY, int valeur, int iPosition, bool AxeHorizontal)
{
	Point Pixel;
	Pixel.y = (int)(DecalageY - GrossissementY * valeur);
	if(AxeHorizontal) Pixel.x = (int)(DecalageX + (((double)iPosition/255)) * 500.0);
	else			  Pixel.x = (int)(DecalageX + (((double)iPosition/255))  *580.0);

	return Pixel;
}

void CTraitement::CreerDiagrammeCouleur(Mat image)
{
	Mat Diagramme;
	Diagramme.create(480,640,CV_8UC3);
	Diagramme.setTo(Scalar(255,255,255));

	//Position graphique
	AxeX=40;
	AxeY=460;
	 GrossissementY = 1.7254901960784313725490196078431;

	//Pixel
	Point PixelRougeCourant,PixelBleuCourant,PixelVertCourant;
	Point PixelRougePrecedant,PixelBleuPrecedant,PixelVertPrecedant;
	int IndexR = 0, IndexB = 0, IndexV = 0;

	//tableau couleur
	int RougeAuX[640];
	int BleuAuX[640];
	int VertAuX[640];

	//Creation Diagramme de base
	CreerDiagrammeBase(Diagramme,AxeX,AxeY,false);

	//initialize Tableau
	for(int i=0; i<640;i++)
	{
		RougeAuX[i] = 0;
		BleuAuX[i] = 0;
		VertAuX[i] = 0;
	}

	int SommeR = 0;
	int SommeB = 0;
	int SommeV = 0;
	for(int i=0; i < image.cols; i++)
	{
		for (int j = 0; j < image.rows; j++)
		{
			//Sommes des differentes couleur pour une ligne
			SommeB += image.data[(j * image.cols * 3) + (i * 3) + 0];
			SommeV += image.data[(j * image.cols * 3) + (i * 3) + 1];
			SommeR += image.data[(j * image.cols * 3) + (i * 3) + 2];
		}

		//Moyenne des couleurs par ligne
		SommeB/=image.rows;
		SommeV/=image.rows;
		SommeR/=image.rows;
		BleuAuX[i] = SommeB;
		VertAuX[i] = SommeV;
		RougeAuX[i] = SommeR;

		PixelBleuCourant = CreerPointGraphique(AxeX,AxeY,GrossissementY,BleuAuX[i],i,true);
		PixelVertCourant = CreerPointGraphique(AxeX,AxeY,GrossissementY,VertAuX[i],i,true);
		PixelRougeCourant = CreerPointGraphique(AxeX,AxeY,GrossissementY,RougeAuX[i],i,true);
		if (i>1)
		{
			line(Diagramme, PixelBleuPrecedant, PixelBleuCourant, Scalar(255,0,0), 1,1);
			line(Diagramme, PixelVertPrecedant, PixelVertCourant, Scalar(0,255,0), 1,1);
			line(Diagramme, PixelRougePrecedant, PixelRougeCourant, Scalar(0,0,255), 1,1);
		}
		PixelBleuPrecedant = PixelBleuCourant;
		PixelVertPrecedant = PixelVertCourant;
		PixelRougePrecedant = PixelRougeCourant;
		SommeB = 0;
	}

	imshow("DiagrammeCouleur",Diagramme);
}

void CTraitement::CreerDiagrammeSeuilAutomatique(Mat image)
{
	Mat Diagramme(480,640,CV_8U);
	Diagramme.setTo(Scalar(255));
	AxeX=40;
	AxeY=460;
	Point PointPrecedent, PointCourant;

	CreerDiagrammeBase(Diagramme,AxeX,AxeY,true);

	int pixValeur;
	int maxValeur=0,minValeur=999999999;
	GrossissementY = 0.03f;
	
	//initialize Occurence tab
	for(int i=0; i<255;i++)
	{
		OccurenceTab[i] = 0;
	}

	//Sommation par occurence de couleur(teinte de noir)
	for(int i=0; i<image.cols;i++)
	{
		for(int j=0; j<image.rows; j++)
		{
			pixValeur = image.data[j *image.cols + i]; //0-255
			OccurenceTab[pixValeur]++;
		}
	}

	// parcourir occurence Tab(dessin)
	for(int i=0; i< 255;i++)
	{
		PointCourant = CreerPointGraphique(AxeX,AxeY,GrossissementY,OccurenceTab[i],i,false);
		if (i>1)
		{
			line(Diagramme, PointPrecedent, PointCourant, Scalar(0), 1,1);
		}
		PointPrecedent = PointCourant;
	}

	//Calcul des 2 sommets 
	//SOMMET 1(max)
	indexMaxS1 = 0;
	double Max = -100;
	for(int i =0; i < 255; i++)
	{
		if(OccurenceTab[i] > Max) 
		{
			Max = OccurenceTab[i];
			indexMaxS1 = i;
		}
	}

	//Sommet 2 (calcul pdf)
	indexMaxS2 = 0;
	Max = -100;
	for(int i =0; i < 255; i++)
	{
		double Data = pow((double)indexMaxS1 - i,2) * OccurenceTab[i];
		if(Data > Max)
		{
			Max = Data;
			indexMaxS2 = i;
		}
	}

	//Determination de lindex ainsi que des position x dans le graphique pour tous les index
	IndexSeuilAutomatique = indexMaxS2 + (indexMaxS1 - indexMaxS2)/2;
	int XS1,XS2,XSA;
	XS1 = (int)(((double)indexMaxS1/255)*500.0);
	XS2 = (int)(((double)indexMaxS2/255)*500.0);
	XSA = (int)(((double)IndexSeuilAutomatique/255)*500.0);

	//dessins des lignes
	DiagrammeBinarise = Mat(480,640,CV_8U);
	DiagrammeBinarise.setTo(Scalar(255));
	CreerDiagrammeBase(DiagrammeBinarise,AxeX,AxeY,true);


	line(Diagramme, Point(AxeX + XS1, AxeY), Point(AxeX + XS1, AxeY - 455), Scalar(0), 1,1);
	line(Diagramme, Point(AxeX + XS2, AxeY), Point(AxeX + XS2, AxeY - 455), Scalar(0), 1,1);
	line(Diagramme, Point(AxeX + XSA, AxeY), Point(AxeX + XSA, AxeY - 455), Scalar(0), 1,1);

	imshow("DiagrammeSeuilAuto",Diagramme);
}

void CTraitement::EtirerHistogramme(Mat frame)
{
	// parcourir occurence Tab(dessin)
	for(int i=0; i< 255;i++)
	{
		if(i < indexMaxS2)
			PointCourant = CreerPointGraphique(AxeX,AxeY,GrossissementY,OccurenceTab[i],0,false);
		else if(i > indexMaxS1)
			PointCourant = CreerPointGraphique(AxeX,AxeY,GrossissementY,OccurenceTab[i],255,false);
		else
			PointCourant = CreerPointGraphique(AxeX,AxeY,GrossissementY,OccurenceTab[i],(i - indexMaxS2) * (indexMaxS1 / indexMaxS2),false);

		if (i>1)
		{
			line(DiagrammeBinarise, PointPrecedent, PointCourant, Scalar(0), 1,1);
		}
		PointPrecedent = PointCourant;
	}
	
	//EtirerHistogramme(DiagrammeBinarise);
	imshow("Digramme Etirer",DiagrammeBinarise);
}

void CTraitement::CreerDiagrammeLigneNiveauNoir(int rownumber, Mat image)
{
	Mat Diagramme(480,640,CV_8U);
	AxeX=40;
	AxeY=460;
	GrossissementY = 1.7254901960784313725490196078431;
	Point PointPrecedent, PointCourant;
	int YMedian;
	int pixValeur;
	int maxValeur=0,minValeur=999999999;

	//Initialisation et creation du graphique de 
	Diagramme.setTo(Scalar(255));
	CreerDiagrammeBase(Diagramme,AxeX,AxeY,false);

	
	//DESSIN GRAPHIQUE
	for(int i=0; i<image.cols;i++)
	{
		pixValeur = image.data[rownumber *image.cols + i]; //0-255
		
		//Determine le min et le max
		if (pixValeur > maxValeur)	maxValeur = pixValeur;
		if (pixValeur < minValeur)	minValeur = pixValeur;

		PointCourant = CreerPointGraphique(AxeX,AxeY,GrossissementY,pixValeur,i,false);

		if (i>1) line(Diagramme, PointPrecedent, PointCourant, Scalar(0), 1,1);
		PointPrecedent = PointCourant;
	}

	//TRAITEMENT MEDIANE(ligne/Texte)
	int ValeurMed = (int)(minValeur+((maxValeur-minValeur)/2.0));
	YMedian = (int)(AxeY - (GrossissementY * (ValeurMed)));
	line(Diagramme, Point(40, YMedian), Point(620, YMedian), Scalar(0), 1,1);
	
	char msg[10];
	memset(msg,0,10);
	sprintf(msg,"%i",ValeurMed);
	putText(Diagramme, msg, Point(580,YMedian-5), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,200));
	DiagrammeBinarise.copyTo(Diagramme);
	imshow("Diagramme Niveau Noir",Diagramme);
}

void CTraitement::DessineCote(Mat image , int * cotes)
{
	int gauche = cotes[0]; //x
	int droit = cotes[1]; //x
	int haut = cotes[2]; //y
	int bas = cotes[3]; //y

	line(image, Point(0, haut), Point(image.cols-1, haut), Scalar(0, 0, 0), 1,1);
	line(image, Point(0, bas), Point(image.cols-1, bas), Scalar(0, 0, 0),1,1);
	line(image, Point(gauche, 0), Point(gauche, image.rows -1), Scalar(0, 0, 0), 1,1);
	line(image, Point(droit, 0), Point(droit, image.rows -1), Scalar(0, 0, 0),1,1);	
}

void CTraitement::TrouveCote(Mat gris, int* cotes)
{	
	int* SommeLigne = new int[gris.rows];
	int* SommeColonne = new int[gris.cols];

	//init arrays
	for (int i=0; i<gris.rows; i++)
	{
		SommeLigne[i]=0;
	}
	for (int i=0; i<gris.cols; i++)
	{
		SommeColonne[i]=0;
	}
	
	for (int i=0; i<gris.rows; i++)
	{
		for (int j=0; j<gris.cols; j++)
		{
			SommeLigne[i]+=gris.data[i*gris.cols + j];
		}
	}

	for (int i=0; i<gris.cols; i++)
	{
		for (int j=0; j<gris.rows; j++)
		{
			SommeColonne[i]+=gris.data[j*gris.cols + i];
		}
	}

	int thresholdA = 500;
	
	int gauche=-1,droit=-1,haut=-1,bas=-1;
	int i=1;
	while (i<gris.cols && gauche == -1)
	{	
		if (abs(SommeColonne[i]-SommeColonne[i-1]) > thresholdA)
		{
			gauche = i;
		}
		i++;
	}

	i = gris.cols -1;
	while (i>0 && droit == -1)
	{
		if (abs(SommeColonne[i]-SommeColonne[i-1]) > (thresholdA))
		{
			droit = i;
		}
		i--;
	}

	i=1;
	while (i<gris.rows && haut == -1)
	{
		if (abs(SommeLigne[i]-SommeLigne[i-1]) >(thresholdA))
		{
			haut = i;
		}
		i++;
	}

	i = gris.rows -1;
	while (i>0 && bas == -1)
	{
		if (abs(SommeLigne[i]-SommeLigne[i-1]) > (thresholdA))
		{
			bas = i;
		}
		i--;
	}
	
	delete SommeLigne;
	delete SommeColonne;

	cotes[0] = gauche;
	cotes[1] = droit;
	cotes[2] = haut;
	cotes[3] = bas;
}

int GetPixelBdroitness(Mat image, Point position)
{
	return image.at<uchar>(Point(position.x, position.y));
}

vector<Point> CTraitement::TrouverRectangle(Mat image)
{
	Point pointhaut, pointgauche, pointdroit, pointbas;
	bool hautCoinTrouve = false, gaucheCoinTrouve = false, droitCoinTrouve = false, basCoinTrouve = false ;
	bool ObjetDepartTrouve;
	int x,y;
	int seuilNoir = 20;
	Point PointCourant;
	vector<Point> coins;

	//search haut point
	y=0;
	while (y < image.rows && !hautCoinTrouve)
	{
		x=0;
		while (x < image.cols && !hautCoinTrouve)
		{
			PointCourant = Point(x,y);
			if (GetPixelBdroitness(image, PointCourant) < seuilNoir)
			{
				hautCoinTrouve = true;
				pointhaut = PointCourant;
			}
			x++;
		}
		y++;
	}
	
	//search gauche point
	x=0;
	ObjetDepartTrouve=false;
	while (x < image.cols && !gaucheCoinTrouve)
	{
		y=0;
		while (y < image.rows && !gaucheCoinTrouve)
		{
			PointCourant = Point(x,y);
			if (GetPixelBdroitness(image, PointCourant) < seuilNoir)
			{				
				ObjetDepartTrouve=true;				
			}
			if(ObjetDepartTrouve && GetPixelBdroitness(image, PointCourant) >= seuilNoir)
			{
				gaucheCoinTrouve = true;
				pointgauche = PointCourant;
			}
			y++;
		}
		x++;
	}

	//search bas point
	y=image.rows-1;
	while (y >= 0 && !basCoinTrouve)
	{
		x=image.cols-1;
		while (x >= 0 && !basCoinTrouve)
		{
			PointCourant = Point(x,y);
			if (GetPixelBdroitness(image, PointCourant) < seuilNoir)
			{
				basCoinTrouve = true;
				pointbas = PointCourant;
			}
			x--;
		}
		y--;
	}

	//search droit point
	x=image.cols-1;
	ObjetDepartTrouve=false;
	while (x >= 0 && !droitCoinTrouve)
	{
		y=image.rows-1;
		while (y >= 0 && !droitCoinTrouve)
		{
			PointCourant = Point(x,y);
			if (GetPixelBdroitness(image, PointCourant) < seuilNoir)
			{				
				ObjetDepartTrouve=true;				
			}
			if(ObjetDepartTrouve && GetPixelBdroitness(image, PointCourant) >= seuilNoir)
			{
				droitCoinTrouve = true;
				pointdroit = PointCourant;
			}
			y--;
		}
		x--;
	}

	if(hautCoinTrouve && basCoinTrouve && gaucheCoinTrouve && droitCoinTrouve)
	{
		//haut,gauche,bas,droit
		coins.push_back(pointhaut);
		coins.push_back(pointgauche);
		coins.push_back(pointbas);
		coins.push_back(pointdroit);
	}
	return coins;
}

void CTraitement::TrouveAngleDessineCroix(Mat image, vector<Point> coins)
{
	Point pointhaut, pointgauche, pointdroit, pointbas;
	if(coins.size() == 4)
	{
		for (int i =0;i<4;i++)
		{
			DessineCroix(image,coins.at(i),0);
		}

		//haut,gauche,bas,droit
		pointhaut = coins.at(0);
		pointgauche = coins.at(1);
		
		double dy = pointgauche.y - pointhaut.y;
		double dx = pointhaut.x - pointgauche.x;
		
		double angle;

		angle = (pointgauche != pointhaut) ? atan2((double)(pointgauche.x - pointhaut.x), (double)(pointgauche.y - pointhaut.y))* 180.0 / 3.1415926535 : 90;
		
		char msg[20];
		sprintf(msg,"Angle = %i deg",(int)angle);
		putText(image, msg,Point(500,540), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,0));
	}
}

Mat CTraitement::Sobel(Mat image, int edge)
{
		Mat result = Mat(image.rows,image.cols, CV_8U);		
		int couleur;
		int couleurhaut;
		int couleurBack;
		edge = 8;

		for (int col = 1; col<result.cols-1;col++)
		{
			for (int row = 1; row<result.rows-1;row++)
			{
				couleur = image.at<uchar>(row,col);
				couleurhaut = image.at<uchar>(row,col-1);
				couleurBack = image.at<uchar>(row-1,col);

				if(abs(couleur - couleurhaut) > edge ||abs(couleur - couleurBack) > edge)
				{
					result.at<uchar>(Point(col,row)) = 255;
				}
				else
				{
					result.at<uchar>(Point(col,row)) = 0;
				}
			}
		}
		return result;
}