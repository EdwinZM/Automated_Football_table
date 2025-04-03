#include <opencv2/opencv.hpp>
#include <iostream>

int detectBall() {
    // Open the default camera
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open the camera." << std::endl;
        return -1;
    }

    // Define the color range for the ball (to be determined)
    // Example: Detecting a red ball
    cv::Scalar lowerColor(0, 120, 70);  // Lower HSV range
    cv::Scalar upperColor(10, 255, 255); // Upper HSV range

    cv::Mat frame, hsvFrame, mask, blurredMask;
    while (true) {
        cap >> frame; // Capture a frame
        if (frame.empty()) {
            std::cerr << "Error: Empty frame captured." << std::endl;
            break;
        }

        // Convert the frame to HSV color space
        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

        // Threshold the HSV image to get only the desired color
        cv::inRange(hsvFrame, lowerColor, upperColor, mask);

        // Apply Gaussian blur to reduce noise
        cv::GaussianBlur(mask, blurredMask, cv::Size(9, 9), 2, 2);

        // Detect circles using Hough Circle Transform
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(blurredMask, circles, cv::HOUGH_GRADIENT, 1, blurredMask.rows / 8, 100, 20, 5, 50);

        // Find the largest circle (assuming it's the ball)
        cv::Point2f ballCenter(0, 0);
        float ballRadius = 0;
        for (const auto& circle : circles) {
            float x = circle[0], y = circle[1], radius = circle[2];
            if (radius > ballRadius) {
                ballCenter = cv::Point2f(x, y);
                ballRadius = radius;
            }
        }

        std::vector<cv::Point2f> ballCoords;
        // Draw the detected ball and display its coordinates
        if (ballRadius > 0) {
            cv::circle(frame, ballCenter, static_cast<int>(ballRadius), cv::Scalar(0, 255, 0), 2);
            int centerX = frame.cols / 2;
            int centerY = frame.rows / 2;
            int ballX = ballCenter.x - centerX;
            int ballY = centerY - ballCenter.y; // Invert Y-axis for Cartesian coordinates
            std::cout << "Ball detected at: (" << ballX << ", " << ballY << ")" << std::endl;
            ballCoords.push_back(cv::Point2f(ballX, ballY));
        }

        // Display the frame
        cv::imshow("Ball Detection", frame);

        std::vector<cv::Point2f> playersCenters = detectPlayers(frame); // Call the function to detect players

        std::cout << "Player coordinates: " << std::endl;
        for (const auto& player : playersCenters) {
            std::cout << "(" << player.x << ", " << player.y << ")" << std::endl;
            cv::circle(frame, player, 5, cv::Scalar(255, 0, 0), -1); // Draw player positions
        }
        // Break the loop on 'q' key press
        if (cv::waitKey(1) == 'q') {
            break;
        }

        // CODE FOR MOVING PLAYERS
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
std::vector<cv::Point2f> detectPlayers(const cv::Mat& frame) {
    // Example: Detecting players based on color (e.g., blue players)
    cv::Scalar lowerPlayerColor(100, 150, 0);  // Lower HSV range for player color
    cv::Scalar upperPlayerColor(140, 255, 255); // Upper HSV range for player color

    cv::Mat hsvFrame, playerMask, blurredPlayerMask;
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
    cv::inRange(hsvFrame, lowerPlayerColor, upperPlayerColor, playerMask);
    cv::GaussianBlur(playerMask, blurredPlayerMask, cv::Size(9, 9), 2, 2);

    // Find contours of the players
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(blurredPlayerMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point2f> playerCenters;
    int centerX = frame.cols / 2;
    int centerY = frame.rows / 2;

    for (const auto& contour : contours) {
        cv::Moments moments = cv::moments(contour);
        if (moments.m00 > 0) {
            float x = moments.m10 / moments.m00;
            float y = moments.m01 / moments.m00;

            // Adjust coordinates to be relative to the center of the camera
            float relativeX = x - centerX;
            float relativeY = centerY - y; // Invert Y-axis for Cartesian coordinates
            playerCenters.emplace_back(relativeX, relativeY);
        }
    }

    return playerCenters;
}