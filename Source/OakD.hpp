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
    std::shared_ptr<dai::node::SpatialLocationCalculator> spatial;
    std::shared_ptr<dai::node::XLinkOut> rgbOut;
    std::shared_ptr<dai::node::XLinkOut> stereoOut;
    std::shared_ptr<dai::node::XLinkOut> spatialOut;
    std::shared_ptr<dai::node::XLinkIn> spatialConfigIn;

    std::shared_ptr<dai::Device> device;
    std::shared_ptr<dai::DataOutputQueue> rgbOutputQueue;
    std::shared_ptr<dai::DataOutputQueue> stereoOutputQueue;
    std::shared_ptr<dai::DataOutputQueue> spatialOutputQueue;
    std::shared_ptr<dai::DataInputQueue> spatialConfigInputQueue;

public:
    OakD();
    bool Connect();
    cv::Mat GetImage();
    cv::Mat GetStereoImage();
    dai::SpatialLocationCalculatorData GetSpatialData(const cv::Mat &referenceFrame);
    void StartStream();
};