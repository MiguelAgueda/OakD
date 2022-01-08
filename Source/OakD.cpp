/* Mostly borrowed from https://docs.luxonis.com/projects/api/en/latest/samples/StereoDepth/stereo_depth_video/#stereo-depth-video
 */

#include "OakD.hpp"


/* @brief Class constructor.
 */
OakD::OakD()
{
    // Empty Constructor.
}


/* @brief Establish a connection to the OakD stereo camera. 
 * @return Returns true if the connection is successfully established, false otherwise.
 * @note Currently throws std::runtime_error if no camera is found.
 * @todo Add try-catch for case when camera is not found, return false when caught.
 */
bool OakD::Connect()
{
    // Define sources and outputs
    this->pipeline = dai::Pipeline();

    this->monoLeft = this->pipeline.create<dai::node::MonoCamera>();
    this->monoRight = this->pipeline.create<dai::node::MonoCamera>();
    this->rgbMiddle = this->pipeline.create<dai::node::ColorCamera>();
    this->stereo = this->pipeline.create<dai::node::StereoDepth>();
    this->rgbOut = this->pipeline.create<dai::node::XLinkOut>();
    this->stereoOut = this->pipeline.create<dai::node::XLinkOut>();

    this->rgbOut->setStreamName("rgb");
    this->stereoOut->setStreamName("stereo");

    // Define properties.
    this->monoLeft->setResolution(dai::MonoCameraProperties::SensorResolution::THE_720_P);
    this->monoLeft->setBoardSocket(dai::CameraBoardSocket::LEFT);
    this->monoRight->setResolution(dai::MonoCameraProperties::SensorResolution::THE_720_P);
    this->monoRight->setBoardSocket(dai::CameraBoardSocket::RIGHT);

    // Set Up Stereo.
    this->stereo->initialConfig.setConfidenceThreshold(245);
    this->stereo->setRectifyEdgeFillColor(0); // black, to better see the cutout
    this->stereo->initialConfig.setMedianFilter(dai::MedianFilter::KERNEL_5x5);
    this->stereo->setLeftRightCheck(false);
    this->stereo->setExtendedDisparity(false);
    this->stereo->setSubpixel(false);


    // Linking
    this->monoLeft->out.link(this->stereo->left);
    this->monoRight->out.link(this->stereo->right);
    this->stereo->depth.link(this->stereoOut->input);

    this->rgbMiddle->setBoardSocket(dai::CameraBoardSocket::RGB);
    this->rgbMiddle->setVideoSize(1080, 720);
    this->rgbMiddle->video.link(this->rgbOut->input);
    this->rgbOut->input.setBlocking(false);
    this->rgbOut->input.setQueueSize(1);

    // Connect to device and start pipeline
    this->device = std::make_shared<dai::Device>(pipeline);

    // Output queue will be used to get the disparity frames from the outputs defined above
    this->rgbOutputQueue = device->getOutputQueue("rgb", 4, true);
    this->stereoOutputQueue = device->getOutputQueue("stereo", 4, true);

    // If control made it this far, connection was successfully established.
    return true;
}


/* @brief Retrieve an RGB image from the OakD center camera.
 * @return Returns an OpenCV cv::Mat RGB image. 
 *      Size determined during instance initialization.
 */
cv::Mat OakD::GetImage()
{
    std::shared_ptr<dai::ImgFrame> inDepth = this->rgbOutputQueue->get<dai::ImgFrame>();
    return inDepth->getCvFrame();
}


/* @brief Retrieve a stereo image from the OakD camera.
 * @return Returns an OpenCV cv::Mat stereo image. 
 *      Size determined during instance initialization.
 * @todo Make the returned image hold values that can be correlated to real-life distance in meters.
 */
cv::Mat OakD::GetStereoImage()
{
    std::shared_ptr<dai::ImgFrame> stereoFrame = this->stereoOutputQueue->get<dai::ImgFrame>();
    return stereoFrame->getCvFrame();
}


/* @brief Start a process that will display both RGB and depth images in full resolution.
 * @note Only way out of this call is for the user to input 'q', or ctrl-c. 
 * @todo Add conditional flag to while loop that can be accessed from outside loop,
 *       will allow the stream to be programmatically closed. 
 */
void OakD::StartStream()
{
    while (cv::waitKey(1) != 'q')
    {
        cv::Mat frame = this->GetImage();
        cv::Mat stereoFrame = this->GetStereoImage();
        cv::imshow("Frame", frame);
        cv::imshow("Stereo Frame", stereoFrame);
    }
}