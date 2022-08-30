#include "../include/QDCparams.hpp"


QDCparams::QDCparams()
{
    LinkType = CAEN_DGTZ_OpticalLink;
    LinkNum = 0;
    ConetNode = 0;
    VMEBaseAddress = 0x0;
    BrdNum = 0;
    AcqMode = CAEN_DGTZ_DPP_ACQ_MODE_Mixed;

    SWTrgMode = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
    ExtTrgMode = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
    GrpTrg = CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
    GrpMask = 0b11111111;//for self trigger only


    EventBuffering = 0;  // 0 means automatic

    RunSyncMode = CAEN_DGTZ_RUN_SYNC_Disabled;

    IOLevel = CAEN_DGTZ_IOLevel_NIM;

    GrpEnMask = 0b11111111;//what groups to enable

    StartMode = CAEN_DGTZ_SW_CONTROLLED;

    DecFactor = 1;//only powers of 2

    RecordLength = 20000;
    PreTriggerSize = 10 * RecordLength / 100;

    caenParams.EnableExtendedTimeStamp = 0;

    for(auto iQDC = 0; iQDC<MAX_X740_GROUP_SIZE; iQDC++){

        //RecordLength[iQDC] = 20000;
        ChGrpMask[iQDC] = 0b11111111;//disable not connected channels or it will keep OR active
        GrpDCOffset[iQDC] = 20;

        //QDC
        //TODO give good values
        caenParams.trgho[iQDC] = 10;//no idea what values to give
        caenParams.GateWidth[iQDC] = 40;
        caenParams.PreGate[iQDC] = 20;
        caenParams.FixedBaseline[iQDC] = 2100;
        caenParams.DisTrigHist[iQDC] = 0;
        caenParams.DisSelfTrigger[iQDC] = 0;
        caenParams.BaselineMode[iQDC] = 2;
        caenParams.TrgMode[iQDC] = 0;
        caenParams.ChargeSensitivity[iQDC] = 2;
        caenParams.PulsePol[iQDC] = 1;
        caenParams.EnChargePed[iQDC] = 0;
        caenParams.TestPulsesRate[iQDC] = 0;
        caenParams.EnTestPulses[iQDC] = 0;
        caenParams.InputSmoothing[iQDC] = 0;
            


    }

}

QDCparams::~QDCparams(){};