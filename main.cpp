#include "main.hpp"
using namespace cv;
using namespace std;

int edgeThresh = 1;
Mat image, gray, edge, cedge;

// define a trackbar callback
static void onTrackbar(int, void*)
{
	blur(gray, edge, Size(3, 3));

	// Run the edge detector on grayscale
	Canny(edge, edge, edgeThresh, edgeThresh * 3, 3);
	cedge = Scalar::all(0);

	image.copyTo(cedge, edge);
	imshow("Edge map", cedge);
}

void help(char** av) {
	cout << "The program captures frames from a video file, image sequence (01.jpg, 02.jpg ... 10.jpg) or camera connected to your computer." << endl
		<< "Usage:\n" << av[0] << " <video file, image sequence or device number>" << endl
		<< "q,Q,esc -- quit" << endl
		<< "space   -- save frame" << endl << endl
		<< "\tTo capture from a camera pass the device number. To find the device number, try ls /dev/video*" << endl
		<< "\texample: " << av[0] << " 0" << endl
		<< "\tYou may also pass a video file instead of a device number" << endl
		<< "\texample: " << av[0] << " video.avi" << endl
		<< "\tYou can also pass the path to an image sequence and OpenCV will treat the sequence just like a video." << endl
		<< "\texample: " << av[0] << " right%%02d.jpg" << endl;
}

const char* keys =
{
	"{help h||}{@image |../data/fruits.jpg|input image name}"
};

int process(VideoCapture& capture) {
	int n = 0;
	char filename[200];
	string window_name = "video | q or esc to quit";
	cout << "press space to save a picture. q or esc to quit" << endl;
	namedWindow(window_name, WINDOW_KEEPRATIO); //resizable window;
	Mat frame;

	for (;;) {
		capture >> frame;
		if (frame.empty())
			break;

		imshow(window_name, frame);
		char key = (char)waitKey(30); //delay N millis, usually long enough to display and capture input

		switch (key) {
		case 'q':
		case 'Q':
		case 27: //escape key
			return 0;
		case ' ': //Save an image
			sprintf(filename, "filename%.3d.jpg", n++);
			imwrite(filename, frame);
			cout << "Saved " << filename << endl;
			break;
		default:
			break;
		}
	}
	return 0;
}

int main(int argc, char** argv)
{
	cout << "Welcome to CVSpark Version " << CVSpark_VERSION_MAJOR << "." << CVSpark_VERSION_MINOR << endl;

	CommandLineParser parser(argc, argv, "{help h||}{@input||}");
	if (parser.has("help"))
	{
		help(argv);
		return 0;
	}
	string arg = parser.get<string>("@input");

	VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file or image sequence
	if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
		capture.open(atoi(arg.c_str()));
	if (!capture.isOpened()) {
		cerr << "Failed to open the video device, video file or image sequence!\n" << endl;
		help(argv);
		return 1;
	}
	return process(capture);
}