// -*- C++ -*-
/*!
 * @file
 * @brief
 * @date
 * @author
 *
 */

#ifndef MONITOR_H
#define MONITOR_H

#include <TCanvas.h>
#include <TF1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TH1.h>
#include <THttpServer.h>
#include <TPolyLine.h>
#include <TSpectrum.h>
#include <TStyle.h>
#include <curl/curl.h>

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

// #include "../../TDigiTES/include/TreeData.h"
#include "../include/TreeData.h"
#include "DaqComponentBase.h"

using namespace RTC;

// Number of peaks for TSpectrum
// constexpr int knPeaks = 8;

class Monitor : public DAQMW::DaqComponentBase
{
 public:
  Monitor(RTC::Manager *manager);
  ~Monitor();

  // The initialize action (on CREATED->ALIVE transition)
  // former rtc_init_entry()
  virtual RTC::ReturnCode_t onInitialize();

  // The execution action that is invoked periodically
  // former rtc_active_do()
  virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

 private:
  TimedOctetSeq m_in_data;
  InPort<TimedOctetSeq> m_InPort;

 private:
  int daq_dummy();
  int daq_configure();
  int daq_unconfigure();
  int daq_start();
  int daq_run();
  int daq_stop();
  int daq_pause();
  int daq_resume();

  int parse_params(::NVList *list);
  int reset_InPort();

  unsigned int read_InPort();
  // int online_analyze();

  BufferStatus m_in_status;
  bool m_debug;

  // Fitting and monitoring
  void FillHist(int size);
  long fCounter;

  static constexpr int kgMods = 8;
  static constexpr int kgChs = 32;
  static constexpr int kgGrp = kgChs/2;
  static constexpr int kgDet_l = 5;
  static constexpr int kgSeg_l = 12;

  static constexpr int kgDet_e = 12;
  static constexpr int kgSeg_e = 4;

  std::array<std::array<double, kgChs>, kgMods> linArg_a;
  std::array<std::array<double, kgChs>, kgMods> linArg_b;



  std::array<std::array<std::unique_ptr<TH1D>, kgChs>, kgMods> fHist;
  std::array<std::array<std::unique_ptr<TH1D>, kgChs>, kgMods> fHistADC;
  std::array<std::array<std::unique_ptr<TGraph>, kgChs>, kgMods> fWaveform;

  //calibrated spectrum
  std::array<std::array<std::unique_ptr<TH1D>, kgChs>, kgMods> fHistADC_calib;
  //energy spectrum
  std::array<std::array<std::unique_ptr<TH1D>, kgGrp>, kgMods> fHistEnSp;
  //position spectrum
  std::array<std::array<std::unique_ptr<TH1D>, kgGrp>, kgMods> fHistPoSp;
  //energy spectrum calibrated
  std::array<std::array<std::unique_ptr<TH1D>, kgGrp>, kgMods> fHistEnSp_calib;


  //2D hists
  //5 histograms with 12 values for the 5 detectors
  std::array<std::unique_ptr<TH2D>, kgDet_l> fHistDet_l;
  //1 histogram for a channel for test
  std::unique_ptr<TH2D> fHistTest_l;



  //LHASA folders
  std::array<std::array<std::unique_ptr<TH1D>, kgSeg_l>, kgDet_l> fHistRaw_l;
  std::array<std::array<std::unique_ptr<TH1D>, kgSeg_l>, kgDet_l> fHistCal_l;

  //ELISSA folders
  std::array<std::array<std::unique_ptr<TH1D>, kgSeg_e>, kgDet_e> fEnSpRaw_e;
  std::array<std::array<std::unique_ptr<TH1D>, kgSeg_e>, kgDet_e> fPoSp_e;
  std::array<std::array<std::unique_ptr<TH1D>, kgSeg_e>, kgDet_e> fEnSpCal_e;

  



  std::unique_ptr<THttpServer> fServ;

  void RegisterHists();
  void RegisterDetectors(std::string fileName, std::string calDirName,
                         std::string rawDirName);
  std::string fSignalListFile;
  std::string fBGOListFile;

  // Event rate uploading
  void UploadEventRate(int timeDuration);
  std::array<std::array<int, kgChs>, kgMods> fEventCounter;
  std::unique_ptr<TGraph> fGrEveRate;
  long fLastCountTime;
  std::string fEveRateServer;

  // Calibration
  void ReadPar();
  std::string fCalibrationFile;
  std::array<std::array<std::array<double, 2>, kgChs>, kgMods> fCalPar;
  std::array<std::array<std::unique_ptr<TF1>, kgChs>, kgMods> fCalFnc;
  double fBinWidth;






  // ASCII Dump
  void DumpHists();
  // std::unique_ptr<CURL> fCurl;
  CURL *fCurl;
  std::string fDumpAPI;
  std::string fDumpState;

  // Reset Histograms
  void ResetHists();

  //for configuration
  std::string eCalibfile;
  double calibSpectre_a;
  double calibSpectre_b;
  
  std::array<int, kgDet_l> detStartMod_l;
  std::array<int, kgDet_l> detStopMod_l;
  std::array<int, kgDet_l> detStartCh_l;
  std::array<int, kgDet_l> detStopCh_l;

  std::array<int, kgDet_e> detStartMod_e;
  std::array<int, kgDet_e> detStopMod_e;
  std::array<int, kgDet_e> detStartCh_e;
  std::array<int, kgDet_e> detStopCh_e;

  //read config file
  void read_cfg();



};









extern "C" {
void MonitorInit(RTC::Manager *manager);
};

#endif  // MONITOR_H
