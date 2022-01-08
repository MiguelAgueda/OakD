#include "OakD.hpp"

bool OakD::Connect()
{
    // Define sources and outputs
    this->pipeline = dai::Pipeline();
    this->monoLeft = this->pipeline.create<dai::node::MonoCamera>();
    this->monoRight = this->pipeline.create<dai::node::MonoCamera>();
    this->rgbMiddle = this->pipeline.create<dai::node::ColorCamera>();
    this->stereo = this->pipeline.create<dai::node::StereoDepth>();
    this->stereoOut = this->pipeline.create<dai::node::XLinkOut>();
    this->rgbOut = this->pipeline.create<dai::node::XLinkOut>();

    this->stereoOut->setStreamName("stereo");
    this->rgbOut->setStreamName("rgb");

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

    this->rgbMiddle->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    this->rgbMiddle->setBoardSocket(dai::CameraBoardSocket::RGB);
    this->rgbMiddle->preview.link(this->rgbOut->input);

    // Connect to device and start pipeline
    this->device = std::make_shared<dai::Device>(pipeline);

    // Output queue will be used to get the disparity frames from the outputs defined above
    this->rgbOutputQueue = device->getOutputQueue("rgb", 4, true);
    this->stereoOutputQueue = device->getOutputQueue("stereo", 4, true);
    return true;
}

cv::Mat OakD::GetImage()
{
    std::shared_ptr<dai::ImgFrame> inDepth = this->rgbOutputQueue->get<dai::ImgFrame>();
    return inDepth->getCvFrame();
}

cv::Mat OakD::GetStereoImage()
{
    std::shared_ptr<dai::ImgFrame> stereoFrame = this->stereoOutputQueue->get<dai::ImgFrame>();
    return stereoFrame->getCvFrame();
}


int OakD::StartStream()
{
    while (cv::waitKey(1) != 'q')
    {
        cv::Mat frame = this->GetImage();
        cv::Mat stereoFrame = this->GetStereoImage();
        cv::imshow("Frame", frame);
        cv::imshow("Stereo Frame", stereoFrame);
    }

    return 0;
}