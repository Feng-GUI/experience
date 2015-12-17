========================
Guitar Hero Robot project
GarageGeeks
www.garagegeeks.org
========================

Image processing the video

src/dxfilter - DirectX DirectShow filter dll.
filter file is a COM dll named GuitarHero.ax

========================
Install:
========================
The GuitarHero.ax is a DirectX DirectShow filter which requiers DirectX9 run-time.
make sure to have inpout32.dll next to the file GuitarHero.ax
register the GuitarHero.ax by runningn 'regsvr32 GuitarHero.ax'
place GuitarHero.ini in your windows directory.

========================
Compile:
========================
install "Microsoft DirectX SDK (April 2007)"
http://www.microsoft.com/downloads/details.aspx?FamilyID=86cf7fa2-e953-475c-abde-f016e4f7b61a&DisplayLang=en

for development, you should also install
Windows(R) Server 2003 SP1 Platform SDK Full Download
because DirectShow moved from the DirectX into the Platform SDK

http://www.microsoft.com/downloads/details.aspx?familyid=EBA0128F-A770-45F1-86F3-7AB010B398A3&displaylang=en

========================
TODO:
========================

* dxfilter
  * Improve detection
  ** add plates color detection to make the detection more strong.
  ** solve the false negatve line bug.
  
   *Configuration:
   ** Use the ini file while it is placed at a directory next to the filter, instead of from Windows directory.
   ** move all delays to configuration
  
  * TCPIP mode:
  ** Put the IP address and Port in the configuration file or at the properties dialog 
  ** Right now, the Connect to tcpip server happens when the filter is loaded, 
    change this to Connect to tcpip server each time play is start.