void showImage(Mat &image, const char *str = "Display Image"){
	namedWindow(str, WINDOW_KEEPRATIO); 
	imshow(str, image);
	waitKey(0);
}
void showImageWithLine(Mat &image, Vec4f &myline, const char *str = "Display Image"){
	Mat temp_img = image.clone();
	auto start = Point(myline[2],myline[3]);
	auto length = 200;
	auto end = Point(myline[2]+myline[0]*length, myline[3]+myline[1]*length);
	auto thickness = 2;
	auto lineType = LINE_8;
	auto color = Scalar(128,128,128);
	line(temp_img, start, end, color, thickness, lineType);
	showImage(temp_img,str);
	temp_img.release();
}