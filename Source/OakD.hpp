#include <iostream>
#include "depthai/depthai.hpp"

class OakD
{
private:
    dai::Pipeline pipeline;
    std::shared_ptr<dai::node::MonoCamera> monoLeft;
    std::shared_ptr<dai::node::MonoCamera> monoRight;
    std::shared_ptr<dai::node::ColorCamera> rgbMiddle;
    std::shared_ptr<dai::node::StereoDepth> stereo;
    std::shared_ptr<dai::node::XLinkOut> rgbOut;
    std::shared_ptr<dai::node::XLinkOut> stereoOut;

    std::shared_ptr<dai::Device> device;
    std::shared_ptr<dai::DataOutputQueue> rgbOutputQueue;
    std::shared_ptr<dai::DataOutputQueue> stereoOutputQueue;

public:
    OakD(){};
    bool Connect();
    cv::Mat GetImage();
    cv::Mat GetStereoImage();
    void StartStream();
};