#include "interface/H4treeReco.h"
#include "interface/WaveformUtils.hpp"
#include "interface/WaveformFit.hpp"
#include "interface/Waveform.hpp"

#include <iostream>

//#define DEBUG_VERBOSE

//
H4treeReco::H4treeReco(TChain *tree,JSONWrapper::Object *cfg,TString outUrl) : 
  H4tree(tree),
  cfg_(cfg),
  MaxTdcChannels_(16),
  MaxTdcReadings_(20),
  nActiveDigitizerChannels_(8)
{
  fOut_  = TFile::Open(outUrl,"RECREATE");
  recoT_ = new TTree("H4treeReco","H4treeReco");
  recoT_->SetDirectory(fOut_);

  //event header
  recoT_->Branch("runNumber",    &runNumber,    "runNumber/i");
  recoT_->Branch("spillNumber",  &spillNumber,  "spillNumber/i");
  recoT_->Branch("evtNumber",    &evtNumber,    "evtNumber/i");
  recoT_->Branch("evtTimeDist",  &evtTimeDist,  "evtTimeDist/i");
  recoT_->Branch("evtTimeStart", &evtTimeStart, "evtTimeStart/i");
  recoT_->Branch("nEvtTimes",    &nEvtTimes,    "nEvtTimes/i");

  //TDC 
  tdc_readings_.resize(MaxTdcChannels_);
  recoT_->Branch("nwc",       &nwc_,      "nwc/i");
  recoT_->Branch("wc_recox",   wc_recox_, "wc_recox[nwc]/F");
  recoT_->Branch("wc_recoy",   wc_recoy_, "wc_recoy[nwc]/F");
  recoT_->Branch("wc_xl_hits",   wc_xl_hits_, "wc_xl_hits[nwc]/i");
  recoT_->Branch("wc_xr_hits",   wc_xr_hits_, "wc_xr_hits[nwc]/i");
  recoT_->Branch("wc_yu_hits",   wc_yu_hits_, "wc_yu_hits[nwc]/i");
  recoT_->Branch("wc_yd_hits",   wc_yd_hits_, "wc_yd_hits[nwc]/i");

  //digitizer channel info
  recoT_->Branch("maxch",               &maxch_,               "maxch/i");
  recoT_->Branch("group",                group_,               "group[maxch]/F");
  recoT_->Branch("ch",                   ch_,                  "ch[maxch]/F");
  recoT_->Branch("pedestal",             pedestal_,            "pedestal[maxch]/F");
  recoT_->Branch("pedestalRMS",          pedestalRMS_,         "pedestalRMS[maxch]/F");
  recoT_->Branch("wave_max",             wave_max_,            "wave_max[maxch]/F");
  recoT_->Branch("wave_max_aft",         wave_max_aft_,        "wave_max_aft[maxch]/F");
  recoT_->Branch("wave_aroundmax",       wave_aroundmax_,      "wave_aroundmax[maxch][50]/F");
  recoT_->Branch("time_aroundmax",       time_aroundmax_,      "time_aroundmax[maxch][50]/F");
  recoT_->Branch("charge_integ",         charge_integ_,        "charge_integ[maxch]/F");
  recoT_->Branch("charge_integ_max",     charge_integ_max_,    "charge_integ_max[maxch]/F");
  recoT_->Branch("charge_integ_fix",     charge_integ_fix_,    "charge_integ_fix[maxch]/F");
  recoT_->Branch("charge_integ_smallw",  charge_integ_smallw_, "charge_integ_smallw[maxch]/F");
  recoT_->Branch("charge_integ_largew",  charge_integ_largew_, "charge_integ_largew[maxch]/F");
  recoT_->Branch("charge_integ_smallw_mcp",  charge_integ_smallw_mcp_, "charge_integ_smallw_mcp[maxch]/F");
  recoT_->Branch("charge_integ_largew_mcp",  charge_integ_largew_mcp_, "charge_integ_largew_mcp[maxch]/F");
  recoT_->Branch("charge_integ_smallw_noise",  charge_integ_smallw_noise_, "charge_integ_smallw_noise[maxch]/F");
  recoT_->Branch("charge_integ_largew_noise",  charge_integ_largew_noise_, "charge_integ_largew_noise[maxch]/F");
  recoT_->Branch("charge_integ_smallw_rnd",  charge_integ_smallw_rnd_, "charge_integ_smallw_rnd[maxch]/F");
  recoT_->Branch("charge_integ_largew_rnd",  charge_integ_largew_rnd_, "charge_integ_largew_rnd[maxch]/F");
  recoT_->Branch("t_max",                t_max_,     	       "t_max[maxch]/F");
  recoT_->Branch("t_max_frac30",         t_max_frac30_,        "t_max_frac30[maxch]/F");
  recoT_->Branch("t_max_frac50",         t_max_frac50_,        "t_max_frac50[maxch]/F");
  recoT_->Branch("t_at_threshold",       t_at_threshold_,      "t_at_threshold[maxch]/F");
  recoT_->Branch("t_over_threshold",     t_over_threshold_,    "t_over_threshold[maxch]/F");
  InitDigi();
}

//
void H4treeReco::InitDigi()
{
  //init channels of interest
  std::vector<JSONWrapper::Object> digis=(*cfg_)["digis"].daughters();
  trigger_=std::pair<int,int>(-1,-1);
  recChannelsH_=new TH1F("recChannels",";Channel name;Index",digis.size(),0,digis.size());
  for(size_t i=0; i<digis.size(); i++)
    {
      //add a ChannelPlot class for the reconstruction
      ChannelReco *chRec = new ChannelReco(digis[i],ChannelReco::kNull, false,false);
      GroupChannelKey_t key(chRec->GetGroup(),chRec->GetModule());
      chPlots_[key] = chRec;
      chPlots_[key]->SetWaveform( new Waveform() );
      if (chRec->GetName() == "Trigger" )
	  trigger_=key;
      recChannelsH_->GetXaxis()->SetBinLabel(i+1,chRec->GetName());
      recChannelsH_->SetBinContent(i+1,i);
    }

  //init wire chambers readout
  std::vector<JSONWrapper::Object> wcs=(*cfg_)["wirechambers"].daughters();
  nwc_=wcs.size();
  for(size_t i=0; i<wcs.size(); i++)
    {
      wcXl_[i]=wcs[i]["l"].toInt();
      wcXr_[i]=wcs[i]["r"].toInt();
      wcYd_[i]=wcs[i]["d"].toInt();
      wcYu_[i]=wcs[i]["u"].toInt();
    }
}

//
void H4treeReco::FillWaveforms()
{
  //first reset waveforms
  int ictr(0);
  for (std::map<GroupChannelKey_t,ChannelReco*>::iterator it=chPlots_.begin();it!=chPlots_.end();++it,ictr++)
    {
      it->second->ClearVectors();
      it->second->GetWaveform()->clear();
      for(int k=0; k<25; k++) 
	{
	  wave_aroundmax_[ictr][k]=-9999;
	  time_aroundmax_[ictr][k]=-9999;
	}
    }

  //fill waveforms
  for (uint iSample = 0 ; iSample < nDigiSamples ; ++iSample)
    {
      GroupChannelKey_t key(digiGroup[iSample],digiChannel[iSample]);
      if(chPlots_.find(key)==chPlots_.end()) continue;
      if(digiChannel[iSample]>nActiveDigitizerChannels_) continue;
      chPlots_[key]->GetWaveform()->addTimeAndSample(digiSampleIndex[iSample]*timeSampleUnit(digiFrequency[iSample]),
						     digiSampleValue[iSample]);
    }

  //reconstruct waveforms
  maxch_=0;  

#ifdef DEBUG_VERBOSE
  std::cout << "+++++" << std::endl;
#endif

  //first reconsruct trigger
  if (trigger_ != std::pair<int,int>(-1,-1))
      reconstructWaveform(trigger_);

  for (std::map<GroupChannelKey_t,ChannelReco*>::iterator it=chPlots_.begin();it!=chPlots_.end();++it)
      if ((*it).first!=trigger_)
	  reconstructWaveform((*it).first);
	  
}


void H4treeReco::reconstructWaveform(GroupChannelKey_t key)
{
  std::map<GroupChannelKey_t,ChannelReco*>::iterator it=chPlots_.find(key);
  // Extract waveform information:
  ChannelReco *chRec=it->second;
  Waveform * waveform = chRec->GetWaveform() ;
  if(waveform->_samples.size()==0) return;
  
  //use samples to get pedestal and RMS
  Waveform::baseline_informations wave_pedestal= waveform->baseline(chRec->GetPedestalWindowLo(),
								    chRec->GetPedestalWindowUp()); 
  
  //substract the pedestal from the samples
  waveform->offset(wave_pedestal.pedestal);
  
  //if pedestal is very high, the signal is negative -> invert it
  if(wave_pedestal.pedestal>chRec->GetThrForPulseInversion()) waveform->rescale(-1);	
  
  //find max amplitude in search window (5 is the number of samples around max for the interpolation)
  int searchWindowLo=chRec->GetSearchWindowLo();
  int searchWindowUp=chRec->GetSearchWindowUp();
  if (chRec->GetSearchWindowTriggerRelative() && (*it).first != trigger_)
    {
      //When this is enabled, trigger should always be reconstructed always as channel 0
      searchWindowLo+=(int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta())/(waveform->_times[1]*1E9));
      searchWindowUp+=(int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta())/(waveform->_times[1]*1E9));
    }

#ifdef DEBUG_VERBOSE
  std::cout << "<< " << searchWindowLo << "," <<  searchWindowUp << ">>" << std::endl;
#endif  
  Waveform::max_amplitude_informations wave_max=waveform->max_amplitude(searchWindowLo,
									searchWindowUp,
									chRec->GetSamplesToInterpolateAtMax()
									); 
  
  //find max amplitude in the search window after the max (to check for ringing issues)
  Waveform::max_amplitude_informations wave_max_aft=waveform->max_amplitude(std::min((int)wave_max.sample_at_max+(int)(chRec->GetSearchWindowAfterLo()/waveform->_times[1]),(int)waveform->_samples.size()),
									    std::min((int)wave_max.sample_at_max+(int)(chRec->GetSearchWindowAfterUp()/waveform->_times[1]),(int)waveform->_samples.size()),		  
									    chRec->GetSamplesToInterpolateAtMax()); 
  
  //fill information for the reco tree
  group_[maxch_]              = it->first.first;
  ch_[maxch_]                 = it->first.second;
  pedestal_[maxch_]           = wave_pedestal.pedestal;
  pedestalRMS_[maxch_]        = wave_pedestal.rms;
  wave_max_[maxch_]           = wave_max.max_amplitude;
  wave_max_aft_[maxch_]           = wave_max_aft.max_amplitude; //for studying ringing issues after the samples
  t_max_[maxch_]              = wave_max.time_at_max*1.e9;
  for(int i=chRec->GetSpyWindowLo(); i<=chRec->GetSpyWindowUp(); i++)
    {
      int idx2store = i-chRec->GetSpyWindowLo();
      int idx;
      if (chRec->GetSearchWindowTriggerRelative() && (*it).first != trigger_)
	idx       = (int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta())/(waveform->_times[1]*1E9))+i;
      else
	idx       = wave_max.sample_at_max+i;  
      float val( (idx>=0 && idx<(int)waveform->_samples.size()) ? waveform->_samples[idx] : -9999. );
      if (val > -9999)
	{
	  wave_aroundmax_[maxch_][idx2store]=val;
	  time_aroundmax_[maxch_][idx2store]=waveform->_times[idx];
	}
      else
	{
	  wave_aroundmax_[maxch_][idx2store]=-9999;
	  time_aroundmax_[maxch_][idx2store]=-9999;
	}
      
    }
  
  //charge integrated
  charge_integ_[maxch_]       = waveform->charge_integrated(wave_max.sample_at_max-chRec->GetChargeWindowLo()/waveform->_times[1],
							    wave_max.sample_at_max+chRec->GetChargeWindowUp()/waveform->_times[1]);

  
  //charge integrated up to the max
  charge_integ_max_[maxch_]   = waveform->charge_integrated(wave_max.sample_at_max-chRec->GetChargeWindowLo()/waveform->_times[1],
							    wave_max.sample_at_max);
  
  //interpolates the wave form in a time range to find the time at 30% of the max
  //7 is the number of samples to use in the interpolation
  t_max_frac30_[maxch_]       = 1.0e9*waveform->time_at_frac(wave_max.time_at_max-chRec->GetCFDWindowLo(),
							     wave_max.time_at_max,
							     0.3,
							     wave_max,
								 chRec->GetSamplesToInterpolateForCFD());
  
  //similar for 50% of the max
  t_max_frac50_[maxch_]       = 1.0e9*waveform->time_at_frac(wave_max.time_at_max-chRec->GetCFDWindowLo(),
							     wave_max.time_at_max,
							     0.5,
							     wave_max,
							     chRec->GetSamplesToInterpolateForCFD());
  
  //time estimate at fixed value (only if max is above threshold)
  t_at_threshold_[maxch_] = -999;
  if(wave_max.max_amplitude>chRec->GetThrForTiming())
	{
	  std::vector<float> crossingTimes = waveform->time_at_threshold(chRec->GetSearchWindowLo(),
									 chRec->GetSearchWindowUp(),
									 chRec->GetThrForTiming(),
									 chRec->GetSamplesToInterpolateForTD());
	  t_at_threshold_[maxch_]   = 1.0e9*(crossingTimes.size()>0 ? crossingTimes[0] : -999);
	  t_over_threshold_[maxch_] = 1.0e9*(crossingTimes.size()>1 ? crossingTimes[1]-crossingTimes[0] : -999);
	}

  charge_integ_fix_[maxch_]       = waveform->charge_integrated((int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta()-chRec->GetChargeWindowLo()*1E9)/(waveform->_times[1]*1E9)),(int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta()+chRec->GetChargeWindowUp()*1E9)/(waveform->_times[1]*1E9)));

#ifdef DEBUG_VERBOSE
  std::cout << t_at_threshold_[0] << "," << (int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta())/(waveform->_times[1]*1E9))-((chRec->GetSmallChargeWindowsSize()-1)/2) << "," << (int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta())/(waveform->_times[1]*1E9))+((chRec->GetSmallChargeWindowsSize()-1)/2) << std::endl;
#endif

  charge_integ_smallw_[maxch_]       = waveform->charge_integrated((int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta())/(waveform->_times[1]*1E9))-((chRec->GetSmallChargeWindowsSize()-1)/2),(int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta())/(waveform->_times[1]*1E9))+((chRec->GetSmallChargeWindowsSize()-1)/2));
  charge_integ_largew_[maxch_]       = waveform->charge_integrated((int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta())/(waveform->_times[1]*1E9))-((chRec->GetLargeChargeWindowsSize()-1)/2),(int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta())/(waveform->_times[1]*1E9))+((chRec->GetLargeChargeWindowsSize()-1)/2));

  if (chRec->GetMCPTimeDelta()!=0)
    {
      charge_integ_smallw_mcp_[maxch_] = waveform->charge_integrated((int)((t_max_[1]+chRec->GetMCPTimeDelta())/(waveform->_times[1]*1E9))-((chRec->GetSmallChargeWindowsSize()-1)/2),(int)((t_max_[1]+chRec->GetMCPTimeDelta())/(waveform->_times[1]*1E9))+((chRec->GetSmallChargeWindowsSize()-1)/2));
      charge_integ_largew_mcp_[maxch_] = waveform->charge_integrated((int)((t_max_[1]+chRec->GetMCPTimeDelta())/(waveform->_times[1]*1E9))-((chRec->GetLargeChargeWindowsSize()-1)/2),(int)((t_max_[1]+chRec->GetMCPTimeDelta())/(waveform->_times[1]*1E9))+((chRec->GetLargeChargeWindowsSize()-1)/2));
    }
  else
    {
      charge_integ_smallw_mcp_[maxch_]=-9999;
      charge_integ_largew_mcp_[maxch_]=-9999;
    }

  charge_integ_smallw_noise_[maxch_]       = waveform->charge_integrated((int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta()-35)/(waveform->_times[1]*1E9))-((chRec->GetSmallChargeWindowsSize()-1)/2),(int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta()-35)/(waveform->_times[1]*1E9))+((chRec->GetSmallChargeWindowsSize()-1)/2));
  charge_integ_largew_noise_[maxch_]       = waveform->charge_integrated((int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta()-35)/(waveform->_times[1]*1E9))-((chRec->GetLargeChargeWindowsSize()-1)/2),(int)((t_at_threshold_[0]+chRec->GetAbsoluteTimeDelta()-35)/(waveform->_times[1]*1E9))+((chRec->GetLargeChargeWindowsSize()-1)/2));

  charge_integ_smallw_rnd_[maxch_]=0;
  charge_integ_largew_rnd_[maxch_]=0;
  for (int i=0;i<chRec->GetSmallChargeWindowsSize();++i)
    {
      int irnd=(int)gRandom->Uniform(900);
      charge_integ_smallw_rnd_[maxch_] += waveform->charge_integrated(irnd,irnd);
    }
  for (int i=0;i<chRec->GetLargeChargeWindowsSize();++i)
    {
      int irnd=(int)gRandom->Uniform(900);
      charge_integ_largew_rnd_[maxch_] += waveform->charge_integrated(irnd,irnd);
    }

  maxch_++;
}


//
void H4treeReco::Loop()
{
  if (fChain == 0) return;
  
  Long64_t nentries = fChain->GetEntries();
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
    {
      
      //progress bar
      if(jentry%10==0) 
	{
	  printf("\r[H4treeReco] status [ %3d/100 ]",int(100*float(jentry)/float(nentries)));
	  std::cout << std::flush;
	}
      
      //readout the event
      fChain->GetEntry(jentry); 

      //save x/y coordinates from the wire chambers
      FillTDC();      
      
      //loop over the relevant channels and reconstruct the waveforms
      FillWaveforms();
	
      //optional:
      //save pulse, pedestal subtracted and aligned using trigger time?
      
      recoT_->Fill();
    }
}

//
void H4treeReco::FillTDC()
{
  //reset data
  for (uint j=0; j<MaxTdcChannels_; j++){ tdc_readings_[j].clear();}
  
  //fill with new data
  for (uint i=0; i<nTdcChannels; i++)
    {
      if (tdcChannel[i]<MaxTdcChannels_)
	{
	  tdc_readings_[tdcChannel[i]].push_back((float)tdcData[i]);
	}
    }

  //compute average positions
  for(UInt_t iwc=0; iwc<nwc_; iwc++)
    {
      //default values in case no tdc data available
      wc_recox_[iwc]=-999;
      wc_recoy_[iwc]=-999;

      wc_xl_hits_[iwc]=tdc_readings_[wcXl_[iwc]].size();
      wc_xr_hits_[iwc]=tdc_readings_[wcXr_[iwc]].size();
      wc_yu_hits_[iwc]=tdc_readings_[wcYu_[iwc]].size();
      wc_yd_hits_[iwc]=tdc_readings_[wcYd_[iwc]].size();

      if (tdc_readings_[wcXl_[iwc]].size()!=0 && tdc_readings_[wcXr_[iwc]].size()!=0){
	float TXl = *std::min_element(tdc_readings_[wcXl_[iwc]].begin(),tdc_readings_[wcXl_[iwc]].begin()+tdc_readings_[wcXl_[iwc]].size());
	float TXr = *std::min_element(tdc_readings_[wcXr_[iwc]].begin(),tdc_readings_[wcXr_[iwc]].begin()+tdc_readings_[wcXr_[iwc]].size());
	wc_recox_[iwc] = (TXr-TXl)*0.005; // = /40./5./10. //position in cm 0.2mm/ns with 25ps LSB TDC
      }
      if (tdc_readings_[wcYd_[iwc]].size()!=0 && tdc_readings_[wcYu_[iwc]].size()!=0){
	float TYd = *std::min_element(tdc_readings_[wcYd_[iwc]].begin(),tdc_readings_[wcYd_[iwc]].begin()+tdc_readings_[wcYd_[iwc]].size());
	float TYu = *std::min_element(tdc_readings_[wcYu_[iwc]].begin(),tdc_readings_[wcYu_[iwc]].begin()+tdc_readings_[wcYu_[iwc]].size());
	wc_recoy_[iwc] = (TYu-TYd)*0.005; // = /40./5./10. //position in cm 0.2mm/ns with 25ps LSB TDC
      }
    }
}


H4treeReco::~H4treeReco()
{
  fOut_->cd();
  recoT_->Write();
  recChannelsH_->SetDirectory(fOut_);
  recChannelsH_->Write();
  for(std::map<GroupChannelKey_t,ChannelReco*>::iterator it=chPlots_.begin();
      it!=chPlots_.end();
      it++)
    {
      TH1F *h=it->second->GetConfigSummary();
      h->SetDirectory(fOut_);
      h->Write();
    }
  fOut_->Close();
}
