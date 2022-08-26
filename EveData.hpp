#ifndef EveData_hpp
#define EveData_hpp 1

#include <CAENDigitizerType.h>

#include <vector>

class EveData
{  // no getter setter.  using public member variables.
 public:
  EveData(uint32_t nSamples = 0)
  {
    ModNumber = 0;
    ChNumber = 0;
    TimeStamp = 0;
    FineTS = 0;
    ChargeShort = 0;
    ChargeLong = 0;

    RecordLength = nSamples;
    Trace1.reserve(nSamples);
    Trace2.reserve(nSamples);
    DTrace1.reserve(nSamples);
    DTrace2.reserve(nSamples);
    DTrace3.reserve(nSamples);
    DTrace4.reserve(nSamples);
  };

  ~EveData(){};

  unsigned char ModNumber;
  unsigned char ChNumber;
  uint64_t TimeStamp;
  double FineTS;
  int16_t ChargeShort;
  int16_t ChargeLong;
  uint32_t RecordLength;
  std::vector<int16_t> Trace1;
  std::vector<int16_t> Trace2;
  std::vector<uint8_t> DTrace1;
  std::vector<uint8_t> DTrace2;
  std::vector<uint8_t> DTrace3;
  std::vector<uint8_t> DTrace4;

 private:
};

#endif
