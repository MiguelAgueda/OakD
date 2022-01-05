#include <iostream>
#include "depthai/depthai.hpp"

class OakD
{
private:
    dai::Pipeline pipeline;
    std::shared_ptr<dai::Device> device;
    std::shared_ptr<dai::node::XLinkOut> xout;
    std::shared_ptr<dai::node::XLinkOut> rgbOut;
    std::shared_ptr<dai::node::MonoCamera> monoLeft;
    std::shared_ptr<dai::node::MonoCamera> monoRight;
    std::shared_ptr<dai::node::ColorCamera> rgbMiddle;
    std::shared_ptr<dai::node::StereoDepth> depth;
    std::shared_ptr<dai::DataOutputQueue> outputQueue;

    // Closer-in minimum depth, disparity range is doubled (from 95 to 190):
    std::atomic<bool> extended_disparity{false};
    // Better accuracy for longer distance, fractional disparity 32-levels:
    std::atomic<bool> subpixel{false};
    // Better handling for occlusions:
    std::atomic<bool> lr_check{false};

    cv::Mat GetImage();
    cv::Mat GetRGB();

public:
    OakD(){};
    bool Connect();
    int StartStream();
};