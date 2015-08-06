#!/usr/bin/env python

import os
import sys
import optparse
import commands

EOSPREFIX='root://eoscms//eos/cms'

#run=[3317,3318,3319,3320,3312,3314,3315,3316,3363,3321,3322,3323,3324,3325,3326,3329,3358]
#run.extend([3362,3369,3370,3371,3372,3373,3374,3375,3376,3295,3296,3365,3367,3346,3347,3334])
#run.extend([3335,3336,3337,3338,3339,3340,3341,3342,3348,3330,3331,3332,3333,3349])
#os.system("eos ls root://eoscms//eos/cms/store/group/dpg_ecal/alca_ecalcalib/TimingTB_H2_Jul2015/raw/DataTree/3313/")
fileList=[]
cnt=0
run =[3317,3318,3319,3320]
for i in run:
	if(cnt <=10):
		print "reading run %s" % i  
		listCmd='cmsLs /store/group/dpg_ecal/alca_ecalcalib/TimingTB_H2_Jul2015/raw/DataTree/%s/ | awk \'{print $5}\'' % i
		fileList += commands.getstatusoutput(listCmd)[1].split()
	cnt+=1

outfile = open("data.txt","w")
for f in fileList:
	if '/store/' in f and not EOSPREFIX in f:  f='%s/%s' % (EOSPREFIX,f)
	outfile.write("%s \n" % f)
outfile.close()
