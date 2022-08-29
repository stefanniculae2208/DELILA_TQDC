#ifndef QDCparams_hpp
#define ODCparams_hpp 1

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>



class QDCparams
{
    public:
    QDCparams();
    ~QDCparams();







    CAEN_DGTZ_ConnectionType LinkType;
    int LinkNum;
    int ConetNode;
    uint32_t VMEBaseAddress;
    int BrdNum;
    CAEN_DGTZ_DPP_AcqMode_t AcqMode;

    CAEN_DGTZ_TriggerMode_t SWTrgMode;
    CAEN_DGTZ_TriggerMode_t ExtTrgMode;
    CAEN_DGTZ_TriggerMode_t GrpTrg;
    uint32_t GrpMask;


    uint32_t EventBuffering;  // 0 means automatic

    CAEN_DGTZ_RunSyncMode_t RunSyncMode;

    CAEN_DGTZ_IOLevel_t IOLevel;

    uint32_t GrpEnMask;

    CAEN_DGTZ_AcqMode_t StartMode;

    uint16_t DecFactor = 1;//only powers of 2

    uint32_t RecordLength;
    uint32_t PreTriggerSize;



    uint32_t ChGrpMask[MAX_X740_GROUP_SIZE];

    uint32_t GrpDCOffset[MAX_X740_GROUP_SIZE];





    //QDC params
    CAEN_DGTZ_DPP_QDC_Params_t caenParams;




};



#endif
