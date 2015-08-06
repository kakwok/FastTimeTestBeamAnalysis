void scale(){

	TH1F * h = new TH1F("h1","h1",10,0,10);
	h->Sumw2();
	for(int i =0;i<10;i++){
	h->Fill(2);
	}
	for(int i =0;i<10;i++){	cout<<"bin err=" << h->GetBinError(i)<<"   ";	}
	cout << endl;
	h->Scale(0.5);
	for(int i =0;i<10;i++){	
		cout<<"bin err=" << h->GetBinError(i)<<"   ";	
	}
	cout << endl;
	h->Draw("E");
}
