#pragma once

#include "ofMain.h"
#include "ofLog.h"
#include "lsl_cpp.h"

using namespace lsl;

class ofxLSLSender {

public:
  ofxLSLSender(){hadConsumers = true;}
  ~ofxLSLSender(){;}

  void addStream(const std::string &name, const std::string &type, int32_t channel_count=1, double nominal_srate=IRREGULAR_RATE, channel_format_t channel_format=cf_float32, const std::string &source_id=std::string()) {
    stream_info info(name, type, channel_count, nominal_srate, channel_format, source_id);
    addStream(name, info);
  }

  void addStream(const std::string &name, const stream_info &_info) {
    // resolve and check if stream exists

    auto it = outlets.find(name);
    if (it != outlets.end()) {
      ofLogWarning("ofxLSLSender::addStream") << "stream '" << name << "' already exists, overwriting";
      return;
    }

    outlets[name] = std::make_unique<stream_outlet>(_info);
    infos[name] = std::make_unique<stream_info>(_info);
  }


  template<typename T>
  void addSample(const std::string &name, const vector<T> &_values) {
    auto it = outlets.find(name);
    if (it == outlets.end()) {
      ofLogWarning("ofxLSLSender::addSample") << "stream '" << name << "' does not exists";
      return;
    }

    auto& info = infos[name];
    if (info->channel_count() != _values.size()) {
      ofLogWarning("ofxLSLSender::addSample") << "wrong number of samples";
      return;
    }

    auto& outlet = outlets[name];

    bool hasConsumers = outlet->have_consumers();
    if (hasConsumers != hadConsumers){
      hadConsumers = hasConsumers;
      if (hasConsumers) {
        ofLogVerbose("ofxLSLSender::addSample") << "'" << name << "' is consumed";
      }
      else {
        ofLogVerbose("ofxLSLSender::addSample") << "'" << name << "' is not consumed";
      }

    }

    if (!hasConsumers) {
      return;
    }

    outlet->push_sample(_values);
  }

  void send();

private:
  std::map<std::string, std::unique_ptr<stream_outlet>> outlets;
  std::map<std::string, std::unique_ptr<stream_info>> infos;

  bool hadConsumers;

};
