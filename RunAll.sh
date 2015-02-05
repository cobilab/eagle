#!/bin/bash
# =============================================================================
# DOWNLOAD AND INSTALL PROGRAMS 
if false; then
  rm -fr eagle-master EAGLESRC*
  wget "https://github.com/pratas/eagle/archive/master.zip" -O EAGLESRC.zip ;
  unzip EAGLESRC.zip ;
  cd eagle-master/ 
  make cleanall ; make ;
  cp eagle mink rebat projector ../
  cd ..
  rm -fr eagle-master EAGLESRC*
else
  make cleanall ; make;
fi

# =============================================================================
