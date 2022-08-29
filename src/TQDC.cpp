#include "../include/TQDC.hpp"





TQDC::TQDC()
{

    fDataVec = new std::vector<EveData *>;
    fDataVec->reserve(1024);

}

TQDC::~TQDC()
{

    for (auto &&ele : *fDataVec) delete ele;
    delete fDataVec;
    fDataVec = nullptr;

}

void TQDC::Open()
{

    auto errCode = CAEN_DGTZ_OpenDigitizer(parHandl.LinkType, parHandl.LinkNum,
                                         parHandl.ConetNode,
                                         parHandl.VMEBaseAddress, &fHandler);
    CheckErrCode(errCode, "Open");

}

void TQDC::Close()
{

    auto errCode = CAEN_DGTZ_CloseDigitizer(fHandler);
    CheckErrCode(errCode, "Close");

}


void TQDC::GetInfo()
{

    CAEN_DGTZ_BoardInfo_t info;
    CAEN_DGTZ_GetInfo(fHandler, &info);


    std::cout << "\nHandler number:\t" << fHandler << "\n"
        << "Model name:\t" << info.ModelName << "\n"
        << "Model number:\t" << info.Model << "\n"
        << "No. channels:\t" << info.Channels << "\n"
        << "Format factor:\t" << info.FormFactor << "\n"
        << "Family code:\t" << info.FamilyCode << "\n"
        << "Firmware revision of the FPGA on the mother board (ROC):\t"
        << info.ROC_FirmwareRel << "\n"
        << "Firmware revision of the FPGA on the daughter board (AMC):\t"
        << info.AMC_FirmwareRel << "\n"
        << "Serial number:\t" << info.SerialNumber << "\n"
        << "PCB revision:\t" << info.PCB_Revision << "\n"
        << "No. bits of the ADC:\t" << info.ADC_NBits << "\n"
        << "Device handler of CAENComm:\t" << info.CommHandle << "\n"
        << "Device handler of CAENVME:\t" << info.VMEHandle << std::endl;

}






void TQDC::CheckErrCode(CAEN_DGTZ_ErrorCode errCode, std::string funcName)
{

    if (errCode != CAEN_DGTZ_Success) {

        std::cout << funcName << ":\t" << ErrorCodeMap.at(errCode) << std::endl;

    }

}


void TQDC::Reset()
{

    auto errCode = CAEN_DGTZ_Reset(fHandler);
    CheckErrCode(errCode, "Reset");

}




void TQDC::SendSWTrg(unsigned int nTrg)
{

    for (unsigned int i = 0; i < nTrg; i++) {

        auto errCode = CAEN_DGTZ_SendSWtrigger(fHandler);
        CheckErrCode(errCode, "Send SWTrigger");

    }

}


void TQDC::Start()
{

    auto errCode = CAEN_DGTZ_SWStartAcquisition(fHandler);
    CheckErrCode(errCode, "CAEN_DGTZ_SWStartAcquisition");

}

void TQDC::Stop()
{

    auto errCode = CAEN_DGTZ_SWStopAcquisition(fHandler);
    CheckErrCode(errCode, "CAEN_DGTZ_SWStopAcquisition");

}




void TQDC::AllocateMemory()
{

    CAEN_DGTZ_ErrorCode errCode;
    uint32_t size;

    errCode = CAEN_DGTZ_MallocReadoutBuffer(fHandler, &fpReadoutBuffer, &size);
    CheckErrCode(errCode, "MallocReadoutBuffer");

    fppPSDEvents = new CAEN_DGTZ_DPP_PSD_Event_t *[MAX_V1740DPP_CHANNEL_SIZE];
    errCode = CAEN_DGTZ_MallocDPPEvents(fHandler, (void **)fppPSDEvents, &size);
    CheckErrCode(errCode, "MallocDPPEvents");

    errCode = CAEN_DGTZ_MallocDPPWaveforms(fHandler, (void **)&fpPSDWaveform, &size);
    CheckErrCode(errCode, "MallocDPPWaveforms");

}

void TQDC::FreeMemory()
{

    CAEN_DGTZ_ErrorCode errCode;

    if (fpReadoutBuffer != nullptr) {
        errCode = CAEN_DGTZ_FreeReadoutBuffer(&fpReadoutBuffer);
        CheckErrCode(errCode, "CAEN_DGTZ_FreeReadoutBuffer");
        fpReadoutBuffer = nullptr;
    }

    if (fppPSDEvents != nullptr) {
        errCode = CAEN_DGTZ_FreeDPPEvents(fHandler, (void **)fppPSDEvents);
        CheckErrCode(errCode, "CAEN_DGTZ_FreeDPPEvents");
        fppPSDEvents = nullptr;
    }

    if (fpPSDWaveform != nullptr) {
        errCode = CAEN_DGTZ_FreeDPPWaveforms(fHandler, (void *)fpPSDWaveform);
        CheckErrCode(errCode, "CAEN_DGTZ_FreeDPPWaveforms");
        fpPSDWaveform = nullptr;
    }
    
}







std::vector<EveData *> *TQDC::GetEvents()
{
    for (auto &&ele : *fDataVec) delete ele;
    fDataVec->clear();

    ReadEvents();

    return fDataVec;
}




void TQDC::ReadEvents()
{
    CAEN_DGTZ_ErrorCode errCode;
    uint32_t bufferSize;
    errCode =
        CAEN_DGTZ_ReadData(fHandler, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT,
                            fpReadoutBuffer, &bufferSize);
    CheckErrCode(errCode, "ReadData");
    if (bufferSize == 0) return;

    uint32_t nEvents[MAX_V1740DPP_CHANNEL_SIZE];
    errCode = CAEN_DGTZ_GetDPPEvents(fHandler, fpReadoutBuffer, bufferSize,
                                    (void **)(fppPSDEvents), nEvents);

    if (errCode == CAEN_DGTZ_Success) {
        for (auto iCh = 0; iCh < MAX_V1740DPP_CHANNEL_SIZE; iCh++) {
        for (auto iEve = 0; iEve < nEvents[iCh]; iEve++) {
            const uint64_t TSMask = 0x7FFFFFFF;
            uint64_t timeTag = fppPSDEvents[iCh][iEve].TimeTag;

            if (timeTag < fPreviousTime[iCh]) {

                fTimeOffset[iCh] += (TSMask + 1);

            }

            fPreviousTime[iCh] = timeTag;
            auto tdc = (timeTag + fTimeOffset[iCh]) * fTimeSample;

            auto data = new EveData(parHandl.RecordLength);
            data->ModNumber = 0;
            data->ChNumber = iCh;
            data->ChargeLong = fppPSDEvents[iCh][iEve].ChargeLong;
            data->ChargeShort = fppPSDEvents[iCh][iEve].ChargeShort;
            data->TimeStamp = tdc;
            data->FineTS = 0;
            
            if (parHandl.caenParams.EnableExtendedTimeStamp) {
                //TODO


                // For safety and to kill the rounding error, cleary using double
                // No additional interpolation now
                /* double posZC =
                    uint16_t((fppPSDEvents[iCh][iEve].Extras >> 16) & 0xFFFF);
                double negZC = uint16_t(fppPSDEvents[iCh][iEve].Extras & 0xFFFF);
                double thrZC = 8192;  // (1 << 13). (1 << 14) is maximum of ADC
                if (parHandl.DiscrMode[iCh/8] == CAEN_DGTZ_DPP_DISCR_MODE_LED)
                    thrZC += parHandl.TrgTh[iCh/8];

                if ((negZC <= thrZC) && (posZC >= thrZC)) {
                    double dt = fTimeSample;
                    data->FineTS =
                        ((dt * 1000. * (thrZC - negZC) / (posZC - negZC)) + 0.5);
                } */
            }


            data->RecordLength = 0;

            if (parHandl.AcqMode == CAEN_DGTZ_DPP_ACQ_MODE_Mixed) {
                errCode = CAEN_DGTZ_DecodeDPPWaveforms(
                    fHandler, &(fppPSDEvents[iCh][iEve]), fpPSDWaveform);
                CheckErrCode(errCode, "DecodeDPPWaveforms");

                data->RecordLength = fpPSDWaveform->Ns;
                data->Trace1.assign(&fpPSDWaveform->Trace1[0],
                                    &fpPSDWaveform->Trace1[data->RecordLength]);
                data->Trace2.assign(&fpPSDWaveform->Trace2[0],
                                    &fpPSDWaveform->Trace2[data->RecordLength]);

                data->DTrace1.assign(&fpPSDWaveform->DTrace1[0],
                                    &fpPSDWaveform->DTrace1[data->RecordLength]);
                data->DTrace2.assign(&fpPSDWaveform->DTrace2[0],
                                    &fpPSDWaveform->DTrace2[data->RecordLength]);
                data->DTrace3.assign(&fpPSDWaveform->DTrace3[0],
                                    &fpPSDWaveform->DTrace3[data->RecordLength]);
                data->DTrace4.assign(&fpPSDWaveform->DTrace4[0],
                                    &fpPSDWaveform->DTrace4[data->RecordLength]);
            }

            fDataVec->push_back(data);
        }
        }
    }
}



void TQDC::Config()
{

    CAEN_DGTZ_ErrorCode errCode;

    for(auto iPar = 0; iPar < MAX_X740_GROUP_SIZE; iPar++)
    {

        //which channels receive triggers
        errCode = CAEN_DGTZ_SetChannelGroupMask(fHandler, iPar, parHandl.ChGrpMask[iPar]);
        CheckErrCode(errCode, "SetChannelGroupMask");


        //record length TODO
        //no idea what params to give for qdc
        /* auto recLength = parHandl.RecordLength[iPar] / fTimeSample;
        errCode = CAEN_DGTZ_SetRecordLength(fHandler, recLength, iPar);
        CheckErrCode(errCode, "SetRecordLength"); */


        //DC offset TODO
        //weird calculations
        auto offset = (1 << 16) * parHandl.GrpDCOffset[iPar] / 100;
        if (parHandl.caenParams.PulsePol[iPar] == 1)//don't actually know the value here !!!!!!
            offset = (1 << 16) * (100 - parHandl.GrpDCOffset[iPar]) / 100;
        errCode = CAEN_DGTZ_SetGroupDCOffset(fHandler, iPar, offset);
        CheckErrCode(errCode, "SetGroupDCOffset");


    }


    //only channel by channel TODO
    //does it work for qdc?
    auto recLength = parHandl.RecordLength / fTimeSample;
    errCode = CAEN_DGTZ_SetRecordLength(fHandler, recLength);
    CheckErrCode(errCode, "SetRecordLength");




    uint32_t preTrigger = parHandl.PreTriggerSize / fTimeSample;
    errCode = CAEN_DGTZ_SetDPPPreTriggerSize(fHandler, -1, preTrigger);
    CheckErrCode(errCode, "SetDPPPreTriggerSize");
    /* for (auto iCh = 0; iCh < fNChs; iCh++) {
        uint32_t preTrigger = fDigiPar.PreTriggerSize[iCh] / fTimeSample;
        errCode = CAEN_DGTZ_SetDPPPreTriggerSize(fHandler, iCh, preTrigger);
        CheckErrCode(errCode, "SetDPPPreTriggerSize");

        errCode = CAEN_DGTZ_SetChannelPulsePolarity(fHandler, iCh,
                                                    fDigiPar.Polarity[iCh]);
        CheckErrCode(errCode, "SetChannelPulsePolarity");
    } */
    //up to here



    //trigger
    //sw trigger
    errCode = CAEN_DGTZ_SetSWTriggerMode(fHandler, parHandl.SWTrgMode);
    CheckErrCode(errCode, "SetSWTriggerMode");

    errCode = CAEN_DGTZ_SetExtTriggerInputMode(fHandler, parHandl.ExtTrgMode);
    CheckErrCode(errCode, "SetExtTriggerInputMode");


    //sync multiple boards
    errCode = CAEN_DGTZ_SetRunSynchronizationMode(fHandler, parHandl.RunSyncMode);
    CheckErrCode(errCode, "SetRunSynchronizationMode");


    //group trigger
    //trigger threshold is not used for x740D DPP-QDC
    errCode = CAEN_DGTZ_SetGroupSelfTrigger(fHandler, parHandl.GrpTrg, parHandl.GrpMask);
    CheckErrCode(errCode, "SetGroupSelfTrigger");


    //IO level
    errCode = CAEN_DGTZ_SetIOLevel(fHandler, parHandl.IOLevel);
    CheckErrCode(errCode, "SetIOLevel");



    //acquisition
    //group enable mask
    errCode = CAEN_DGTZ_SetGroupEnableMask(fHandler, parHandl.GrpEnMask);
    CheckErrCode(errCode, "SetGroupEnableMask");


    //acq mode
    errCode = CAEN_DGTZ_SetAcquisitionMode(fHandler, parHandl.StartMode);
    CheckErrCode(errCode, "SetAcquisitionMode");

    //decimation factor
    errCode = CAEN_DGTZ_SetDecimationFactor(fHandler, parHandl.DecFactor);



    //DPP settings
    //acq mode
    errCode = CAEN_DGTZ_SetDPPAcquisitionMode(
      fHandler, parHandl.AcqMode, CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime);
    CheckErrCode(errCode, "SetDPPAcquisitionMode");


    //Num events
    errCode = CAEN_DGTZ_SetMaxNumAggregatesBLT(fHandler, 1023);
    CheckErrCode(errCode, "SetMaxNumAggregatesBLT");

    errCode =
        CAEN_DGTZ_SetDPPEventAggregation(fHandler, parHandl.EventBuffering, 0);
    CheckErrCode(errCode, "SetDPPEventAggregation");

    

    




}



void TQDC::QDCConfig()
{

    //TODO find function for qdc
    //auto errCode = CAEN_DGTZ_SetDPPParameters(fHandler, 0, &parHandl.caenParams);

    //try with group mask and see if it works
    auto errCode = CAEN_DGTZ_SetDPPParameters(fHandler, 0b11111111, &parHandl.caenParams);
    CheckErrCode(errCode, "SetDPPParameters");

} 
