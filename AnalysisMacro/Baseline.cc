#include <TROOT.h>
#include <TBranch.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TF1.h>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

float avg( vector<float > list){
  float mean=0;
  for(vector< float >::iterator it =list.begin(); it!=list.end();it++){
        mean = mean + *it;
  }
  return mean/list.size();
}
float SD( vector< float > list){
  float SD =0;
  float mean = avg(list);
    for(vector< float >::iterator it =list.begin(); it!=list.end();it++){
        SD = SD + (*it-mean)*(*it-mean);
  }
 return sqrt(SD/list.size());
}

void Baseline(){
//void Baseline(TString inputlist){
//int Baseline(int argc, char * argv[] ){

   UInt_t          nDigiSamples			=0;
   UInt_t          nTdcChannels			=0;
   UInt_t          digiGroup[27648]		;   	     //[nDigiSamples]
   UInt_t          digiChannel[27648]		;          //[nDigiSamples]
   UInt_t          digiSampleIndex[27648]	;          //[nDigiSamples]
   Float_t         digiSampleValue[27648]	;          //[nDigiSamples]

   TBranch        *b_nDigiSamples		=0;
   TBranch        *b_nTdcChannels		=0;
   TBranch        *b_digiGroup			=0;		//[nDigiSamples]
   TBranch        *b_digiChannel		=0;       //[nDigiSamples]
   TBranch        *b_digiSampleIndex		=0;   //[nDigiSamples]
   TBranch        *b_digiSampleValue		=0;   //[nDigiSamples]

   TChain chain("H4tree");
   string line;
   char   fname[1000];
   ifstream file("data.txt");
   int cnt = 0;
   while ( getline( file,line)){
	//cout << line <<endl;
	strcpy(fname, line.c_str());
	//if(cnt ==1 )chain.Add(fname);
	chain.Add(fname);
	cnt++;
   }
//   chain.Add("root://eoscms//eos/cms//store/group/dpg_ecal/alca_ecalcalib/TimingTB_H2_Jul2015/raw/DataTree/3313/*.root");
   cout << "Adding the trees ..." <<endl;
   std::cout << "nentries is : " << chain.GetEntries() << std::endl;

   char input[500];

   chain.SetBranchAddress("nDigiSamples",&nDigiSamples,&b_nDigiSamples);
   chain.SetBranchAddress("nTdcChannels",&nTdcChannels,&b_nTdcChannels);
   chain.SetBranchAddress("digiGroup",&digiGroup,&b_digiGroup);
   chain.SetBranchAddress("digiChannel",&digiChannel,&b_digiChannel);
   chain.SetBranchAddress("digiSampleIndex",&digiSampleIndex,&b_digiSampleIndex);
   chain.SetBranchAddress("digiSampleValue",&digiSampleValue,&b_digiSampleValue);

   std::cout << "Opened chain " << chain.GetName() << std::endl;

   TH1F * waveform[3];
   TH1F * SlopeDist_short[3];
   TH1F * SlopeDist_full[3];
   TH1F * RMS_short[3];		// Fluctuation of the channels
   TH1F * Mean_short[3];	// Baseline of the channels
   TH1F * Res_short[3];		// Res=Sum of deviation fit / RMS
   TH1F * Res_full[3];
   TH1F * wave_sROI[3];
   TH1F * wave_fROI[3];

   char hist_name[200],hist_title[200];
   int   nWave =100;	// Number of waveform to be average
   int   iWave =0,Total_wave=0;
   
   sprintf(hist_title,"Sample wave ch0,avg %i wave",nWave);
   TH1F * Sample_wave_ch0 = new TH1F("sample_wave_ch0",hist_title,1000,0,200);
   Sample_wave_ch0->GetXaxis()->SetTitle("ns");
   Sample_wave_ch0->GetYaxis()->SetTitle("ADC");
   sprintf(hist_title,"Sample wave ch0,avg %i wave",nWave);
   TH1F * Sample_wave_ch3 = new TH1F("sample_wave_ch3",hist_title,1000,0,200);
   Sample_wave_ch3->GetXaxis()->SetTitle("ns");
   Sample_wave_ch3->GetYaxis()->SetTitle("ADC");
   sprintf(hist_title,"Sample wave ch0,avg %i wave",nWave);
   TH1F * Sample_wave_ch4 = new TH1F("sample_wave_ch4",hist_title,1000,0,200);
   Sample_wave_ch4->GetXaxis()->SetTitle("ns");
   Sample_wave_ch4->GetYaxis()->SetTitle("ADC");

   for(int i=0;i<3;i++){
	sprintf(hist_name, "SlopeDist_short_ch%i",(i==0?i:i+2));
	sprintf(hist_title,"Slope of fit (1,9)ns, ch_%i,avg %i wave",(i==0?i:i+2),nWave);
	SlopeDist_short[i] = new TH1F(hist_name,hist_title,100,0,0);
	SlopeDist_short[i]->GetXaxis()->SetTitle("ADC/ns");

	sprintf(hist_name, "SlopeDist_full_ch%i",(i==0?i:i+2));
	sprintf(hist_title,"Slope of fit (1,190)ns, ch_%i,avg %i wave",(i==0?i:i+2),nWave);
	SlopeDist_full[i]  = new TH1F(hist_name,hist_title,100,0,0);
	SlopeDist_full[i]->GetXaxis()->SetTitle("ADC/ns");

	sprintf(hist_name, "Mean_ch%i",(i==0?i:i+2));
	sprintf(hist_title,"Mean of ADC (1,9)ns, ch_%i,avg %i wave",(i==0?i:i+2),nWave);
	Mean_short[i]      = new TH1F(hist_name,hist_title,100,0,0);
	Mean_short[i]->GetXaxis()->SetTitle("ADC");

	sprintf(hist_name, "RMS_ch%i",(i==0?i:i+2));
	sprintf(hist_title,"RMS  of ADC in (1,9)ns, ch_%i,avg %i wave",(i==0?i:i+2),nWave);
	RMS_short[i]       = new TH1F(hist_name,hist_title,100,0,0);
	RMS_short[i]->GetXaxis()->SetTitle("ADC^2");

	sprintf(hist_name, "Res_short_ch%i",(i==0?i:i+2));
	sprintf(hist_title,"Residual in fit range (1,9)ns, ch_%i,avg %i wave",(i==0?i:i+2),nWave);
	Res_short[i]      = new TH1F(hist_name,hist_title,100,0,0);
	Res_short[i]->GetXaxis()->SetTitle("Residual");

	sprintf(hist_name, "Res_full_ch%i",(i==0?i:i+2));
	sprintf(hist_title,"Residual in fit range (1,190)ns, ch_%i,avg %i wave",(i==0?i:i+2),nWave);
	Res_full[i]      = new TH1F(hist_name,hist_title,100,0,0);
	Res_full[i]->GetXaxis()->SetTitle("Residual");

   }

   const int nentries = chain.GetEntries();
//   const int nentries = 10;

   float m =0, RMS[2]={0,0}, fit_RMS[2]={0,0};
   float sum2_err[3][1024],sum_err[3][1024];
   for(int i=0;i<3;i++){
	for(int j=0;j<1024;j++){	
		sum2_err[i][j]=0.0;
		sum_err[i][j]=0.0;
	}
   }
   char  temp_name[3][50];
   TF1 * pol1 = new TF1("pol1","[0]*x+[1]",1,9);
   TF1 * pol2 = new TF1("pol2","[0]*x+[1]",1,190);

  TFile * output = new TFile("Slope_5runs.root","RECREATE");
    for(int i=0;i<3;i++){
	sprintf(temp_name[i],"waveform_ch%i",(i==0?i:i+2) );
	waveform[i] = new  TH1F(temp_name[i],temp_name[i],1000,0,200);
	sprintf(temp_name[i],"wave_sROI_ch%i",(i==0?i:i+2) );
	wave_sROI[i] = new TH1F(temp_name[i],temp_name[i],1000,0.,0.);
	sprintf(temp_name[i],"wave_fROI_ch%i",(i==0?i:i+2) );
	wave_fROI[i] = new TH1F(temp_name[i],temp_name[i],1000,0.,0.);
   } 

   for(int iEvent=0; iEvent<nentries ;iEvent++) {
  	chain.GetEntry(iEvent);
	if(nTdcChannels==0){
		// Loop group 0 channel 0 
		for(int iSample=0; iSample<1024;iSample++){
			waveform[0] ->Fill(digiSampleIndex[iSample]*0.2,digiSampleValue[iSample]);
			sum_err[0][digiSampleIndex[iSample]]  +=     digiSampleValue[iSample];
			sum2_err[0][digiSampleIndex[iSample]] += pow(digiSampleValue[iSample],2);
			if(digiSampleIndex[iSample]*0.2 >=1 && digiSampleIndex[iSample]*0.2<=9)		wave_sROI[0]->Fill(digiSampleValue[iSample]);
			if(digiSampleIndex[iSample]*0.2 >=1 && digiSampleIndex[iSample]*0.2<=190)	wave_fROI[0]->Fill(digiSampleValue[iSample]);
		}
		// Loop group 0 channel 3 
		for(int iSample=3072; iSample<4096;iSample++){
			waveform[1] ->Fill(digiSampleIndex[iSample]*0.2,digiSampleValue[iSample]);
			sum_err[1][digiSampleIndex[iSample]]  +=     digiSampleValue[iSample];
			sum2_err[1][digiSampleIndex[iSample]] += pow(digiSampleValue[iSample],2);
			if(digiSampleIndex[iSample]*0.2 >=1 && digiSampleIndex[iSample]*0.2<=9)		wave_sROI[1]->Fill(digiSampleValue[iSample]);
			if(digiSampleIndex[iSample]*0.2 >=1 && digiSampleIndex[iSample]*0.2<=190)	wave_fROI[1]->Fill(digiSampleValue[iSample]);
		}
		// Loop group 0 channel 4
		for(int iSample=4096; iSample<5120;iSample++){
			waveform[2] ->Fill(digiSampleIndex[iSample]*0.2,digiSampleValue[iSample]);
			sum_err[2][digiSampleIndex[iSample]]  +=     digiSampleValue[iSample];
			sum2_err[2][digiSampleIndex[iSample]] += pow(digiSampleValue[iSample],2);
			if(digiSampleIndex[iSample]*0.2 >=1 && digiSampleIndex[iSample]*0.2<=9)		wave_sROI[2]->Fill(digiSampleValue[iSample]);
			if(digiSampleIndex[iSample]*0.2 >=1 && digiSampleIndex[iSample]*0.2<=190)	wave_fROI[2]->Fill(digiSampleValue[iSample]);
		}
		iWave++;
		if(iWave==nWave){		// do fiting
			iWave=0;
			Total_wave+=nWave;
			cout <<" Processed " << Total_wave << " empty waves "<<endl;
			for(int i=0;i<3;i++){
				waveform[i]->Scale(1./nWave);
				//for(int j=1;j<waveform[i]->GetNbinsX();j++)	waveform[i]->SetBinError(j,sqrt(waveform[i]->GetBinContent(j))/sqrt(nWave));
				for(int j=1;j<waveform[i]->GetNbinsX();j++){
					//cout <<i<<"  "<< sqrt(sum2_err[i][j]/nWave - pow(sum_err[i][j]/nWave,2)) <<endl;
					waveform[i]->SetBinError(j,sqrt( sum2_err[i][j]/nWave - pow(sum_err[i][j]/nWave,2) ) /sqrt(nWave));
					sum2_err[i][j]=0;			// re-set the sum2 of RMS of each point
					sum_err[i][j]=0;			// re-set the sum2 of RMS of each point
				}
				// pol1 = (1,9) ns  ; pol 2 = (1,190)ns
				pol1->SetParameter(0,0);
				pol2->SetParameter(0,0);
				if(i==0){	pol1->SetParameter(1,3667);	pol2->SetParameter(1,3667);}
				if(i==1){	pol1->SetParameter(1,410);	pol2->SetParameter(1,410);}
				if(i==2){	pol1->SetParameter(1,442);	pol2->SetParameter(1,442);}
				if(Total_wave==nWave){
					if(i==0) Sample_wave_ch0 = (TH1F*) waveform[i]->Clone("Sample_wave_ch0");
					if(i==2) Sample_wave_ch3 = (TH1F*) waveform[i]->Clone("Sample_wave_ch3");
					if(i==2) Sample_wave_ch4 = (TH1F*) waveform[i]->Clone("Sample_wave_ch4");
				}
				waveform[i] ->Fit(pol1,"QR");
				SlopeDist_short[i]->Fill(pol1->GetParameter(0));
				waveform[i] ->Fit(pol2,"QR");
				SlopeDist_full[i]->Fill(pol2->GetParameter(0));
				//cout <<" slope short= "<< pol1->GetParameter(0) << " slope full " << pol2->GetParameter(0)<<endl;
				// Compute Chi2:
				RMS[0] = wave_sROI[i]->GetRMS(1);
				RMS[1] = wave_fROI[i]->GetRMS(1);
				fit_RMS[0]=0;
				fit_RMS[1]=0;
				for(int j=1;j<waveform[i]->GetNbinsX();j++){
				    if(waveform[i]->GetBinCenter(j)>=1 && waveform[i]->GetBinCenter(j)<=9)
					fit_RMS[0] += pow(waveform[i]->GetBinContent(j)-pol1->Eval( waveform[i]->GetBinCenter(j) ),2);
				    if(waveform[i]->GetBinCenter(j)>=1 && waveform[i]->GetBinCenter(j)<=190)
					fit_RMS[1] += pow(waveform[i]->GetBinContent(j)-pol2->Eval( waveform[i]->GetBinCenter(j) ),2);
				}
				//cout <<"RMS data= "<< RMS[0]*RMS[0] <<"   fit_chi2= "<< (fit_RMS[0]/wave_sROI[i]->GetEntries())/RMS[0]*RMS[0]<<endl;
				//cout <<"RMS data= "<< RMS[1]*RMS[1] <<"   fit_chi2= "<< (fit_RMS[1]/wave_sROI[i]->GetEntries())/RMS[1]*RMS[1]<<endl;
				Mean_short[i]->Fill( wave_sROI[i]->GetMean(1));
				RMS_short[i] ->Fill( RMS[0]);
				Res_short[i]->Fill( (fit_RMS[0]/wave_sROI[i]->GetEntries())/RMS[0]*RMS[0]);
				Res_full[i] ->Fill( (fit_RMS[1]/wave_fROI[i]->GetEntries())/RMS[1]*RMS[1]);
				wave_sROI[i]->Reset();
				wave_fROI[i]->Reset();
				waveform[i]->Reset();
			}
		}
	}
	if(iEvent %1000==0) cout<<"Read "<<iEvent<<" events"<<endl;
	if(Total_wave% (200*nWave)==0){	//write the file for every 200 entries
   		for(int i=0;i<3;i++){
		Mean_short[i]     ->Write("",TObject::kOverwrite);
		RMS_short[i]      ->Write("",TObject::kOverwrite);
		SlopeDist_short[i]->Write("",TObject::kOverwrite);
		SlopeDist_full[i] ->Write("",TObject::kOverwrite);
		Res_short[i]      ->Write("",TObject::kOverwrite);
		Res_full[i]       ->Write("",TObject::kOverwrite);
		Sample_wave_ch0    ->Write();
		Sample_wave_ch3    ->Write();
		Sample_wave_ch4    ->Write();
 		}
	}
}
  output->Close();
}


