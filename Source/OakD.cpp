#include "OakD.hpp"

bool OakD::Connect()
{
    // Define sources and outputs
    this->pipeline = dai::Pipeline();
    this->monoLeft = pipeline.create<dai::node::MonoCamera>();
    this->monoRight = pipeline.create<dai::node::MonoCamera>();
    this->rgbMiddle = pipeline.create<dai::node::ColorCamera>();
    this->depth = pipeline.create<dai::node::StereoDepth>();
    this->xout = pipeline.create<dai::node::XLinkOut>();
    this->rgbOut = pipeline.create<dai::node::XLinkOut>();

    xout->setStreamName("disparity");
    rgbOut->setStreamName("rgb");

    rgbMiddle->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    rgbMiddle->setBoardSocket(dai::CameraBoardSocket::RGB);
    rgbMiddle->preview.link(rgbOut->input);

    // Connect to device and start pipeline
    this->device = std::make_shared<dai::Device>(pipeline);

    // Output queue will be used to get the disparity frames from the outputs defined above
    this->outputQueue = device->getOutputQueue("rgb", 4, true);
    return true;
}

cv::Mat OakD::GetImage()
{
    std::shared_ptr<dai::ImgFrame> inDepth = this->outputQueue->get<dai::ImgFrame>();
    return inDepth->getCvFrame();
}


int OakD::StartStream()
{
    while (true)
    {
        cv::Mat frame = this->GetImage();
        cv::imshow("Frame", frame);
        cv::waitKey(1);
    }

    return 0;
}