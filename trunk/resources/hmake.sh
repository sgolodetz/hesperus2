#!/bin/bash

#############
# Functions #
#############

function bad_level
{
	echo "Error: The specified game or level does not exist"
	exit
}

function cleanup
{
	# General Cleanup
	/bin/rm -fR *.bru
	/bin/rm -fR *.dsf
	/bin/rm -fR *.sbr

	# Rendering Cleanup
	/bin/rm -fR *.dgm
	/bin/rm -fR *.hgm
	/bin/rm -fR *.lbt
	/bin/rm -fR *.lum
	/bin/rm -fR *.png
	/bin/rm -fR *.rbr
	/bin/rm -fR *.rg*
	/bin/rm -fR *.rp*
	/bin/rm -fR *.rt*
	/bin/rm -fR *.vis

	# Collision Cleanup
	/bin/rm -fR *.cbr
	/bin/rm -fR *.cg*
	/bin/rm -fR *.cp
	/bin/rm -fR *.ct*
	/bin/rm -fR *.ebr
	/bin/rm -fR *.nav
	/bin/rm -fR *.op
	/bin/rm -fR *.ot
}

function print_usage
{
	echo "Usage: ./hmake.sh <game> <level> [noclean]"
	exit
}

####################
# End Of Functions #
####################

if [[ $# -ne 2 && $# -ne 3 ]]
then
	print_usage
fi

if [[ ! -e "$1/levels/$2/$2.mef" ]]
then
	bad_level
fi

cd $1/levels/$2
PATH=$PATH:../../../../build/bin/engine/tools

mef2input $2.mef $2.bru $2.dsf $2.obs $2.lum
hdivide $2.bru $2.rbr $2.cbr $2.dgm $2.hgm $2.sbr

#####################
# Rendering Section #
#####################

hcsg -r $2.rbr $2.rg1
hbsp -r $2.rg1 $2.hgm $2.rt1
hportal -r $2.rt1 $2.rp1
hflood -r $2.rt1 $2.rp1 $2.rg2
hbsp -r $2.rg2 $2.hgm $2.rt2
hportal -r $2.rt2 $2.rp2
hvis $2.rp2 $2.vis
hdetail $2.rt2 $2.dgm $2.rt3
hlight $2.rt3 $2.vis $2.lum LM $2.lbt

#####################
# Collision Section #
#####################

hexpand $2.dsf $2.cbr

for f in *.ebr
do
	stem=${f%.*}
	hcsg -c $f ${stem}.cg1
	hbsp -c ${stem}.cg1 nohints ${stem}.ct1
	hportal -c ${stem}.ct1 ${stem}.cp
	hflood -c ${stem}.ct1 ${stem}.cp ${stem}.cg2
	hbsp -c ${stem}.cg2 nohints ${stem}.ct2
done

geomtreepairs=""
for f in *.cg2
do
	stem=${f%.*}
	geomtreepairs="$geomtreepairs $f ${stem}.ct2"
done

hobsp -c $geomtreepairs $2.ot
hoportal -c $2.ot $2.op
hnav $2.dsf $2.ot $2.nav

#####################
# Collation Section #
#####################

hcollate +L $2.lbt $2.rp2 $2.vis $2.ot $2.op $2.nav $2.dsf $2.obs $2.bsp

if [[ $# -eq 2 || $3 != "noclean" ]]
then
	cleanup
fi

