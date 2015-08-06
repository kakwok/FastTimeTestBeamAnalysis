void test_fit(){
	TH1F * h1 = new TH1F("h1","h1",1000,0,200);
	TF1 * f1  = new TF1("f1","[0]*x+[1]",1,9);
	TF1 * f2  = new TF1("f2","[0]*x+[1]",1,190);
	
	f1->SetParameters(0,442);
	f2->SetParameters(0,442);
	for(int i=0;i<1000;i++){
		h1->Fill(i,0.1*i+gRandom->Gaus(442,10));
		h1->SetBinError(i,sqrt(h1->GetBinContent(i)));
	}	
	h1->Fit("f2","QR");

	TH1F * h2 =  new TH1F("h2","h2",10,0,10);
	h2 = (TH1F*) h1->Clone("h2");
	h2->SetTitle("new title");
	h2->Draw();
}
