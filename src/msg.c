#include "msg.h"
#include <stdio.h>
#include <stdlib.h>

void PrintMenu(void){

  fprintf(stderr,
  "                                                                        \n"
  "                                                                        \n"
  "      ███████  ███████  ████████  ██       ███████                      \n"
  "      ██       ██   ██  ██        ██       ██                           \n"
  "      █████    ███████  ██  ████  ██       █████                        \n"
  "      ██       ██   ██  ██    ██  ██       ██                           \n"
  "      ███████  ██   ██  ████████  ███████  ███████                      \n"
  "                                                                        \n");

  fprintf(stderr,
  "NAME                                                                    \n"
  "      EAGLE v%u.%u 2015-2020                                            \n"
  "      Efficient computation of minimal Relative Absent                  \n"
  "      Words (mRAWs) and its associated GC distributions,                \n"
  "      profiles, and patterns.                                           \n"
  "                                                                        \n"
  "AUTHORS                                                                 \n"
  "      D. Pratas and J. M. Silva.                                        \n"
  "                                                                        \n"
  "SYNOPSIS                                                                \n"
  "      ./EAGLE [OPTION]... [FILE] [FILE]                                 \n"
  "                                                                        \n"
  "SAMPLE                                                                  \n"
  "      Run: ./EAGLE -v -min 11 -max 16 human.fa SARS-CoV2.fa             \n"
  "                                                                        \n",
  VERSION, RELEASE);

  fprintf(stderr,
  "DESCRIPTION                                                             \n"
  "      Localization and quantification of minimal Relative               \n"
  "      Absent Words (mRAWs) and GC associated measures                   \n"
  "                                                                        \n"
  "      -h,  --help                                                       \n"
  "           usage guide (help menu)                                      \n"
  "                                                                        \n"
  "      -V,  --version                                                    \n"
  "           display program and version information                      \n"
  "                                                                        \n"
  "      -f,  --force                                                      \n"
  "           force mode. Overwrites old files                             \n"
  "                                                                        \n"
  "      -v,  --verbose                                                    \n"
  "           verbose mode (more information)                              \n"
  "                                                                        \n"
  "      -vv, --very-verbose                                               \n"
  "           very verbose mode (much more information)                    \n"
  "                                                                        \n"
  "      -t,  --threads                                                    \n"
  "           does NOT use threads if flag is set (slower)                 \n"
  "                                                                        \n"
  "      -i,  --ignore-ir                                                  \n"
  "           does NOT use inverted repeats if flag is set                 \n"
  "                                                                        \n"
  "      -c,  --ignore-profiles                                            \n"
  "           does NOT compute GC profiles                                 \n"
  "                                                                        \n"
  "      -o,  --stdout                                                     \n"
  "           write overall statistics to standard output                  \n"
  "                                                                        \n"
  "      -p,  --plots                                                      \n"
  "           print Shell code to generate plots (gnuplot)                 \n"
  "                                                                        \n");

  fprintf(stderr,
  "      -min [NUMBER],  --minimum [NUMBER]                                \n"
  "           k-mer minimum size (usually 10)                              \n"
  "                                                                        \n"
  "      -max [NUMBER],  --maximum [NUMBER]                                \n"
  "           k-mer maximum size (usually 16)                              \n"
  "                                                                        \n");

  fprintf(stderr,
  "      [FILE]                                                            \n"
  "           Input FASTA reference (e.g. human) -- MANDATORY.             \n"
  "           This content will be loaded in the models.                   \n"
  "                                                                        \n"
  "      [FILE]                                                            \n"
  "           Input FASTA target (e.g. SARS-CoV-2) -- MANDATORY.           \n"
  "           The mRAWs will be mapped on this content file.               \n"
  "                                                                        \n"
  "COPYRIGHT                                                               \n"
  "      Copyright (C) 2014-2020, IEETA/DETI, University of Aveiro.        \n"
  "      This is a Free software, under GPLv3. You may redistribute        \n"
  "      copies of it under the terms of the GNU - General Public          \n"
  "      License v3 <http://www.gnu.org/licenses/gpl.html>. There          \n"
  "      is NOT ANY WARRANTY, to the extent permitted by law.              \n"
  "                                                                        \n");
  }

void PrintVersion(void){
  fprintf(stderr,
  "                                                                        \n"
  "                          ==================                            \n"
  "                          |    EAGLE %u.%u   |                          \n"
  "                          ==================                            \n"
  "                                                                        \n"
  "      Efficient computation of minimal Relative Absent Words (mRAWs).   \n"
  "                                                                        \n"
  "         Copyright (C) 2014-2020 IEETA/DETI, University of Aveiro.      \n"
  "                                                                        \n"
  "                This is a Free software, under GPLv3.                   \n"
  "                                                                        \n"
  "You may redistribute copies of it under the terms of the GNU - General  \n"
  "Public License v3 <http://www.gnu.org/licenses/gpl.html>. There is NOT  \n"
  "ANY WARRANTY, to the extent permitted by law. Em@il: pratas @ ua . pt.  \n"
  "                                                                        \n", 
  VERSION, RELEASE);
  }

