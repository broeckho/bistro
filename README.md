# Stride project 



Stride is an Open Source project and its acronym stands for **S**imulate **tr**ansmission of **i**nfectious **d**is**e**ases.
 
The project is hosted in a GitHub [code repository](https://github.com/broeckho/stride.git) and has a continous integration  
 cycle [build & test](https://travis-ci.org/broeckho/stride) at Travis CI. The online [reference documentation](https://broeckho.github.io/stride) is automatically  
 generated with each new commit of the code.

The current status of the project build is : [![Build Status](https://travis-ci.org/broeckho/stride.svg?branch=master)](https://travis-ci.org/broeckho/stride)

Information on layout, installation, dependencies of the project, and platforms that  
are supported, can be found in the files INSTALL.txt, DEPENDENCIES.txt and  
PLATFORMS.txt respectively.
For license info, see LICENSE.txt. 

A breakdown of project artefacts (excluding third-party material) generated with the  
cloc tool on 12/05/2018:

Language|files|blank|comment|code
:-------|-------:|-------:|-------:|-------:
C++|60|777|1463|3713
C/C++ Header|88|950|2450|2552
Python|26|266|388|847
R|6|322|353|729
CMake|20|103|602|572
YAML|2|17|22|126
Markdown|5|36|0|99
make|1|20|39|99
Bourne Again Shell|2|17|40|92
SWIG|5|19|12|65
XML|2|4|0|56
Bourne Shell|3|23|65|54
XSLT|1|8|0|36
--------|--------|--------|--------|--------
SUM:|221|2562|5434|9040 

Current contributors are (in alphabetical order):

* Jan Broeckhove (jan.broeckhove@uantwerpen.be)
* Elise Kuylen (elise.kuylen@uantwerpen.be)
* Lander Willem (lander.willem@uantwerpen.be)

and contributing students

* Niels Aerens 
* Thomas Avé  
* Tobia De Koninck 
* Robin Jadoul 

## GenGeo
The GenGeo can be tested by compiling Stride and then running the `./cmake-build-release/installed/bin/gengeopop` program.
This executable can be given some files as parameter (use `gengeopop --help to show the usage`), these paths are relative to the data directory.
By default the program will use the following files:
 - `flanders_cities.csv`
 - `flanders_commuting.csv`
 - `households_flanders.csv`
The GeoGrid will be written into `gengeopop.json`, which can be opened using the MapViewer.


## GenGeo MapViewer
The MapViewer is compiled with the other binaries. It can run by `./cmake-build-release/installed/bin/mapviewer`. You then have to load a geogrid file by clicking the load file button on the right hand side. The map on the left hand side shows all the locations in the geogrid. You can get more info by clicking on a marker. The middle column will then have the info about the location and the contact centers. You can click on one of the contact centers in that list, you will then get info about it in the right column.
You can select multiple locations by using SHIFT.


## Integration

The Stride binary can use a Population in three ways:

  - `default`   Generate a Population using `PopulationBuilder` (use `./bin/stride`)
  - `import`    Import a Population from a `proto` or `json` file, which was generated using `gengeopop`. (use `./bin/stride -c run_import_default.xml`)
  - `generate`  Generate a Population using GenGeoPop and provide data files and parameters. (use `./bin/stride -c run_generate_default.xml`)