#ifndef QDCparams_hpp
#define ODCparams_hpp 1

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>



class QDCparams
{
    public:
    QDCparams()
    {

        caenParams.EnableExtendedTimeStamp = 0;

        for(auto iQDC = 0; iQDC<MAX_X740_GROUP_SIZE; iQDC++){

            RecordLength[iQDC] = 20000;
            ChGrpMask[iQDC] = 0b00000000;//disable not connected channels or it will keep OR active
            GrpDCOffset[iQDC] = 20;

            //QDC
            //TODO give good values
            caenParams.trgho[iQDC] = 0;//no idea what values to give
            caenParams.GateWidth[iQDC] = 0;
            caenParams.PreGate[iQDC] = 0;
            caenParams.FixedBaseline[iQDC] = 0;
            caenParams.DisTrigHist[iQDC] = 0;
            caenParams.DisSelfTrigger[iQDC] = 0;
            caenParams.BaselineMode[iQDC] = 0;
            caenParams.TrgMode[iQDC] = 0;
            caenParams.ChargeSensitivity[iQDC] = 0;
            caenParams.PulsePol[iQDC] = 0;
            caenParams.EnChargePed[iQDC] = 0;
            caenParams.TestPulsesRate[iQDC] = 0;
            caenParams.EnTestPulses[iQDC] = 0;
            caenParams.InputSmoothing[iQDC] = 0;
            


        }

    }



    ~QDCparams();







    CAEN_DGTZ_ConnectionType LinkType = CAEN_DGTZ_OpticalLink;
    int LinkNum = 0;
    int ConetNode = 0;
    uint32_t VMEBaseAddress = 0x0;
    int BrdNum = 0;
    CAEN_DGTZ_DPP_AcqMode_t AcqMode = CAEN_DGTZ_DPP_ACQ_MODE_Mixed;

    CAEN_DGTZ_TriggerMode_t SWTrgMode = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
    CAEN_DGTZ_TriggerMode_t ExtTrgMode = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
    CAEN_DGTZ_TriggerMode_t GrpTrg = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
    uint32_t GrpMask = 0b11111111;


    uint32_t EventBuffering = 0;  // 0 means automatic

    CAEN_DGTZ_RunSyncMode_t RunSyncMode = CAEN_DGTZ_RUN_SYNC_Disabled;

    CAEN_DGTZ_IOLevel_t IOLevel = CAEN_DGTZ_IOLevel_NIM;

    uint32_t GrpEnMask = 0b11111111;

    CAEN_DGTZ_AcqMode_t StartMode = CAEN_DGTZ_SW_CONTROLLED;

    uint16_t DecFactor = 1;//only powers of 2





    uint32_t RecordLength[MAX_X740_GROUP_SIZE];

    uint32_t ChGrpMask[MAX_X740_GROUP_SIZE];

    uint32_t GrpDCOffset[MAX_X740_GROUP_SIZE];





    //QDC params
    CAEN_DGTZ_DPP_QDC_Params_t caenParams;




};



#endif