#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
#include <chrono>
using namespace cv;


int main()
{
    Mat frame;

    VideoCapture cap(0);

    CascadeClassifier face_cascade;
    CascadeClassifier eye_cascade;

    face_cascade.load("Dependencies/OpenCV/haarcascades/haarcascade_frontalface_default.xml");
    eye_cascade.load("Dependencies/OpenCV/haarcascades/haarcascade_eye.xml");

    std::chrono::steady_clock::time_point end;
    std::chrono::steady_clock::time_point start;
    bool has_found_face = false;;
    int count = 0;
    while (true)
    {

        cap.read(frame);

        if (face_cascade.empty())
        {
            std::cout << "not loaded" << std::endl;
        }

        std::vector<Rect> faces;
        std::vector<Rect> eyes;
        Mat gray_frame;
        cvtColor(frame,gray_frame, COLOR_BGR2GRAY);
        face_cascade.detectMultiScale(gray_frame, faces, 1.2, 10);
        if (waitKey(5) == (int)'q')
            break;

        if (faces.size() == 0 && has_found_face)
        {
            end = std::chrono::steady_clock::now();
            std::cout << "Elapsed time in seconds: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " sec";
            break;
        }
        else 
        {
            start = std::chrono::steady_clock::now();
            for (int i = 0; i < faces.size(); i++)
            {
                Mat face_roi = gray_frame(faces[i]);
                std::cout << "found face\n";
                std::cout << faces[i].tl() << std::endl;
                std::cout << faces[i].br() << std::endl;
                eye_cascade.detectMultiScale(face_roi, eyes, 1.2, 5);
                rectangle(frame, faces[i].tl(), faces[i].br(), Scalar(0, 0, 255), 3);


                count++;
                for (int ii = 0; ii < eyes.size(); ii++)
                {
                    Point center(faces[i].x + eyes[ii].x + eyes[ii].width * 0.5, faces[i].y + eyes[ii].y + eyes[ii].height * 0.5);
                    int radius = cvRound((eyes[ii].width + eyes[ii].height) * 0.25);
                    circle(frame, center, radius, Scalar(0, 255, 0), 4, 8, 0);
                }
                has_found_face = true;
            }
        }

        imshow("Live", frame);
    }


    cap.release();
    return 0;
}