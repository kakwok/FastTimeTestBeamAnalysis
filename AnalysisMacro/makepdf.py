import ROOT as r

#filename = "tsRatios_248033.root"
filename = "Slope_5runs_400k.root"

#grep1 ="HBHE"
#grep2 ="Bad"
grep1 =""
grep2 =""

file=r.TFile.Open(filename)
if(grep1!=""):
	output = filename.split(".")[0]+"_"+grep1+"_"+grep2+".pdf"
else:
	output = filename.split(".")[0]+".pdf"
	

c1  = r.TCanvas("c1","c1",800,600)
c1.Divide(2,2)

nItems = len(file.GetListOfKeys())
nPages = 0
nPad   = 1
IsLast = False
i      = 1
r.gStyle.SetOptStat(1)

print "Found "+str(nItems)+" items"
print "Printing histograms that starts with:    "+grep1+grep2

names=[]
for key in file.GetListOfKeys():
	keyname= key.GetName()
	names.append(keyname)
names.sort()
print names
# Loop over all histograms
for key in names:
	hist_name = key
	hist = file.Get(hist_name)
	if(hist.GetEntries()==0):
		print hist_name+" is skipped for zero entry"
		continue
	if(hist_name.startswith(grep1) or hist_name.startswith(grep2)):
		c1.cd(nPad)
		if( ("Map" in hist_name) and ("HF" in hist_name)):
			r.gPad.SetLogy(0)
			r.gStyle.SetOptStat(0)
		if( ("Map" not in hist_name) and ("HF" in hist_name)):
			r.gStyle.SetOptStat(1)
			r.gPad.SetLogy(1)
			hist.GetXaxis().SetTitle("TS1/TS2")
		if( ("Map" in hist_name) and ("HBHE" in hist_name)):
			r.gPad.SetLogy(0)
			r.gStyle.SetOptStat(0)
			hist.GetZaxis().SetRangeUser(0,hist.GetMaximum())
#			if(hist_name=="HBHE_TSRatioMap_Avg_D1"):hist.GetZaxis().SetRangeUser(0.2,0.6)
		if( ("Map" not in hist_name) and ("HBHE" in hist_name)):
			r.gStyle.SetOptStat(1)
			r.gPad.SetLogy(1)
			hist.GetXaxis().SetTitle("TS5/TS4")
		if( ("Map" not in hist_name) and ("HBHE" in hist_name) and ("TSdist" in hist_name)) :
			r.gStyle.SetOptStat(1)
			r.gPad.SetLogy(1)
			hist.GetXaxis().SetTitle("TS number")
		if( ("Map" not in hist_name) and ("HBHE" in hist_name) and ("nTS" in hist_name)) :
			r.gStyle.SetOptStat(1)
			r.gPad.SetLogy(1)
			hist.GetXaxis().SetTitle("nTS_fired")
		hist.Draw("COLZ")
		# Open a new page
		if( nPad == 4):
			if(nPages==0): c1.Print(output+"(","pdf")
			if(nPages!=0 and IsLast==False): c1.Print(output,"pdf")
			if(i == nItems):
				c1.Print(output+")","pdf")
				IsLast = True
			nPages+=1
			nPad = 1
			c1.Clear()
			c1.Divide(2,2)
		if( i% 4!=0):
			nPad+=1
		i+=1
if(IsLast==False): 
	c1.Print(output+")","pdf")
