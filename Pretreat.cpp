#include "Pretreat.h"


Pretreat::Pretreat()
{
}


Pretreat::~Pretreat()
{
}

void Pretreat::GetRegion(Mat &inimg, Mat &outimg)
{
	Mat original;
	inimg.copyTo(original);
	//imshow("original", original);

	GaussianBlur(inimg, inimg, Size(GAUSSIANBLUR, GAUSSIANBLUR),0, 0, BORDER_DEFAULT);

	cvtColor(inimg, inimg, CV_BGR2GRAY);

	//边缘检测
	Canny(inimg, inimg, CARDCANNY_1, CARDCANNY_2);

	stringstream ss(stringstream::in | stringstream::out);
	ss << "source/temp/cardcanny.jpg";
	imwrite(ss.str(), inimg);

	//imshow(" canny", inimg);
	threshold(inimg, inimg, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

	vector<vector<Point> > contours;
	cv::findContours(inimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓  
	vector<Rect> rect;

	for (int i = 0; i < contours.size(); ++i)
	{
		//cout << contours[i].size() << endl;

		if (contours[i].size() > CARDCONTOURS)//将比较小的轮廓剔除掉  
		{
			rect.push_back( boundingRect(contours[i]) );
		}
	}
	
	//cout << rect.size() << endl;
	for (int j = 0; j < rect.size(); j++)
	{
		float r = (float)rect[j].width / (float)rect[j].height;
		if (r < 1) {
			r = (float)rect[j].height / (float)rect[j].width;
		}
		//cout << r << ",,," << rect[j].width << ",,," << rect[j].height << endl;
		if (r <= CARDRATIO_MAX && r >= CARDRATIO_MIN){
			original(rect[j]).copyTo(outimg); // copy the region rect1 from the image to roi1
			//imshow("re", outimg);
		}
	}

}

void Pretreat::CharSegmentation(Mat &inimg, Mat &outimg)
{
	Mat original, outTemp;
	inimg.copyTo(original);
	//imshow("img", original);

	//rectangle(inimg, Rect(30, 110, 275, 40), Scalar(255, 0, 0), 1, 1, 0);
	Rect rect(CHARRECT_X, CHARRECT_Y, CHARRECT_WIDTH, CHARRECT_HEIGHT);
	original(rect).copyTo(inimg);
	//imshow("rect", inimg);

	cvtColor(inimg, inimg, CV_BGR2GRAY);

	//边缘检测
	Canny(inimg, inimg, CHARCANNY_1, CHARCANNY_2);
	threshold(inimg, inimg, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	imshow(" threshold",inimg);

	//inimg.copyTo(outimg);
	resize(inimg, outimg, Size(CHARBIGSIZE_WIDTH, CHARBIGSIZE_HEIGHT), 0, 0, INTER_LINEAR);

	for (int row = 0; row < inimg.rows; row++)
	{
		//消除多余行
		Rect rectNum(0, row, inimg.cols, 1);
		Mat temp;
		inimg(rectNum).copyTo(temp);
		int n = countNonZero(temp);
		//cout << "Zerorow" << n << ",,," << inimg.rows << endl;
		if (n < COUNTNONZERO_ROW)
		{
			inimg.row(row).setTo(Scalar(0));
		}
	}

	for (int col = 0; col < inimg.cols; col++)
	{
		//消除多余列
		Rect rectNum(col, 0, 1, inimg.rows);
		Mat temp;
		inimg(rectNum).copyTo(temp);
		int n = countNonZero(temp);
		//cout << "Zerocol: " << n << endl;
		if (n < COUNTNONZERO_COL)
		{
			inimg.col(col).setTo(Scalar(0));
		}
	}
	imshow(" countNonZero", inimg);

	Mat img;
	resize(inimg, img, Size(CHARBIGSIZE_WIDTH, CHARBIGSIZE_HEIGHT), 0, 0, INTER_LINEAR);
	imshow("img", img);
	inimg = img;

	stringstream ss(stringstream::in | stringstream::out);
	ss << "source/temp/char2.jpg";
	imwrite(ss.str(), inimg);

	inimg.copyTo(outTemp);
	
	vector<vector<Point>> contours;
	// find 
	findContours(inimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	// draw 
	Mat result(inimg.size(), CV_8U, Scalar(0));
	drawContours(result, contours, -1, Scalar(255, 0, 255), 1);
	vector<Rect> charRect;

	for (int i = 0; i < contours.size(); ++i)
	{
		//cout << contours[i].size() << endl;
		if (contours[i].size() > CHARCONTOURS)//将比较小的轮廓剔除掉  
		{
			charRect.push_back( boundingRect(contours[i]) );
		}
	}
	//cout << charRect.size() << endl;
	for (int j = 0; j < charRect.size(); j++)
	{
		if (charRect[j].height > ONECHARRECT_HEIGTH)
		{
			cout << charRect[j].height << ",,," << j << endl;
			if (charRect[j].width <= ONECHARRECT_WIDTH) {
				rectangle(outTemp, charRect[j], Scalar(100), 1);
				Mat charTempImg;
				outimg(charRect[j]).copyTo(charTempImg);

				//cout << "width" << charRect[j] << endl;
				stringstream ss(stringstream::in | stringstream::out);
				ss << "source/char/char" << "_" << j << ".jpg";
				imwrite(ss.str(), charTempImg);
			}else {
				int n = charRect[j].width / ONECHARRECT_WIDTH_NUM;

				for (int k = 0; k < n; k++) {
					float nt = charRect[j].width / n;
					Rect recttemp(charRect[j].x + n*nt - (k+1)*nt, charRect[j].y, nt, charRect[j].height);

					Mat charTempImg;
					outimg(recttemp).copyTo(charTempImg);

					//cout << "width" << recttemp << endl;
					stringstream ss(stringstream::in | stringstream::out);
					ss << "source/char/char" << "_" << j << "_" << k << ".jpg";
					imwrite(ss.str(), charTempImg);
				}
			}
		 }
	}

	imshow(" drawContours", outTemp);
}
