#ifndef TQDC_hpp
#define TQDC_hpp 1

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "EveData.hpp"
#include "QDCparams.hpp"
#include "ErrorCodeMap.hpp"



class TQDC
{
    public:
    TQDC();
    ~TQDC();


    void Open();
    void Close();

    void GetInfo();
    void Reset();


    void CheckErrCode(CAEN_DGTZ_ErrorCode errCode, std::string funcName = "");


    std::vector<EveData *> *GetEvents();

    void Config();//TODO

    void QDCConfig();//TODO

    void AllocateMemory();
    void FreeMemory();

    void Start();
    void Stop();

    void SendSWTrg(unsigned int nTrg = 1);

    void EnableFineTS();




    private:
    void ReadEvents();



    //handler
    int fHandler;

    //params
    QDCparams parHandl;



    //data
    std::vector<EveData *> *fDataVec = nullptr;


    // Memory
    char *fpReadoutBuffer = nullptr;                         // readout buffer
    CAEN_DGTZ_DPP_PSD_Event_t **fppPSDEvents = nullptr;      // events buffer
    CAEN_DGTZ_DPP_PSD_Waveforms_t *fpPSDWaveform = nullptr;  // waveforms buffer



    //time
    int fTimeSample = 2;
    uint64_t fPreviousTime[MAX_V1740DPP_CHANNEL_SIZE];
    uint64_t fTimeOffset[MAX_V1740DPP_CHANNEL_SIZE];

};









#endif