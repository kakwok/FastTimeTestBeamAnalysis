void test_fileloop(){

   char dirpath[1000];
   char *ext=".root";
   int  run =3313;
   //sprintf(dirpath,"root://eoscms//eos/cms/store/group/dpg_ecal/alca_ecalcalib/TimingTB_H2_Jul2015/raw/DataTree/%i/",run);
   sprintf(dirpath,"/afs/cern.ch/work/k/kakwok/public/FastTimeTestBeamAnalysis/AnalysisMacro/");

   cout<<dirpath<<endl;
   //TSystemDirectory dir(dirpath, dirpath);
   void * dir =  gSystem->OpenDirectory(dirpath);
   
   const char * file;
   while ( file =gSystem->GetDirEntry(dir)){
   TString s = file;
   cout << s <<endl;  
   }
//   TList *files = dir.GetListOfFiles();
   if (files) {
      TSystemFile *file;
      TString fname;
      TIter next(files);
      while ((file=(TSystemFile*)next())) {
         fname = file->GetName();
         if (!file->IsDirectory() && fname.EndsWith(ext)) {
            cout << fname.Data() << endl;
         }
     }
  }
}
