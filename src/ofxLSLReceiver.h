#pragma once

#include "lsl_cpp.h"
#include "ofLog.h"
#include "ofMain.h"
#include "ofxLSLResolver.h"

using namespace lsl;

namespace ofxLSL {

class BaseReceiver : public ofThread {
  friend Resolver;
 public:
  BaseReceiver(std::string _streamName, std::string _sourceId);
  ~BaseReceiver();

  void setCapacity(int value) { sampleCapacity.store(value); }
  bool isConnected();
  std::string getStreamName() { return streamName; }
  std::string getSourceId()   { return sourceId; }

protected:
  void handleConnect(const std::shared_ptr<lsl::stream_inlet>& value);
  void handleDisconnect(const std::shared_ptr<lsl::stream_inlet>& value);

  std::shared_ptr<Resolver> resolver;
  std::mutex resolverMutex;

  bool start();
  bool stop();
  void run();
  virtual void pull() = 0;

  atomic<bool> active;
  std::mutex runMutex;
  std::unique_ptr<std::thread> pullThread;
  std::condition_variable pullSignal;
  std::mutex pullMutex;

  std::string streamName;
  std::string sourceId;
  std::shared_ptr<lsl::stream_inlet> inlet;

  atomic<int> sampleCapacity;
};


template <typename T>
class TimedSample {
public:
  double timeStamp = 0.0;
  double timeCorrection;  //!< offset to convert merker source time to local computer time. For more info: https://github.com/sccn/liblsl/blob/e762b9e10ad0d77651923c0afa7f435af38e8a9b/include/lsl_cpp.h#L1083-L1102
  std::vector<T> sample;
};

template <typename T>
class Receiver : public BaseReceiver {
public:
  Receiver(std::string _streamName, std::string _sourceId = "") :
    BaseReceiver(_streamName, _sourceId) {}

  std::vector<std::shared_ptr<TimedSample<T>>> flush() {
    std::lock_guard<std::mutex> lock(pullMutex);
    std::vector<std::shared_ptr<TimedSample<T>>> flushSamples;
    flushSamples.swap(samples);
    return flushSamples;
  }
  double getTimeCorrection(double timeout = lsl::FOREVER)
  {
	  // For more info: https://github.com/sccn/liblsl/blob/e762b9e10ad0d77651923c0afa7f435af38e8a9b/include/lsl_cpp.h#L1083-L1102
	  return inlet->time_correction(timeout);
  }
  ~Receiver() { stop(); }

protected:
  void pull() override {
    auto sampleBuffer = std::make_shared<TimedSample<T>>();
    auto ts = inlet->pull_sample(sampleBuffer->sample, 0.0);
    sampleBuffer->timeStamp = ts;
    if (ts > 0) {
      std::lock_guard<std::mutex> lock(pullMutex);
	  sampleBuffer->timeCorrection = getTimeCorrection(1);  // update the time correction value when pulling sample
      samples.push_back(sampleBuffer);
      while (samples.size() && samples.size() > sampleCapacity) {
        samples.erase(samples.begin());
      }
    }
  }

  std::vector<std::shared_ptr<TimedSample<T>>> samples;
};


template <typename T>
class EventSample {
public:
  double timeStamp = 0.0;
  std::vector<T> sample;
  std::string streamName;
  std::string sourceId;
};


template <typename T>
class EventReceiver : public BaseReceiver {
public:
  EventReceiver(std::string _streamName, std::string _sourceId = "") :
    BaseReceiver(_streamName, _sourceId) {}

  ~EventReceiver() { stop(); }

  ofEvent<const std::shared_ptr<EventSample<T>>&> onSample;

protected:
  void pull() override {
    auto sampleBuffer = std::make_shared<EventSample<T>>();
    auto ts = inlet->pull_sample(sampleBuffer->sample, 0.0);
    sampleBuffer->timeStamp = ts;
    sampleBuffer->streamName = streamName;
    sampleBuffer->sourceId = sourceId;
    if (ts > 0) ofNotifyEvent(onSample, sampleBuffer, this);
  }
};
}
