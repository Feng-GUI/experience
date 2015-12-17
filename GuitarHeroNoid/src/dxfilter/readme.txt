Some notes that might be  helpfull to compile and link
Rafael Mizrahi


The GuitarHeroNoid DirectShow filter code was initialy developed using 
"Microsoft DirectX 9.0 SDK (Summer 2004)"
Now, we have upgraded the code to use "Microsoft DirectX SDK (April 2007)"
And since DirectShow was moved to Windows Platform SDK
http://msdn2.microsoft.com/en-us/library/ms783323.aspx
 
You also need to install DirectShow from Platform SDK 
http://www.microsoft.com/downloads/details.aspx?familyid=EBA0128F-A770-45F1-86F3-7AB010B398A3&displaylang=en 
 
* include the directories
"C:\Program Files\Microsoft DirectX SDK (April 2007)\Include" 
"C:\Program Files\Microsoft Platform SDK\Samples\Multimedia\DirectShow\BaseClasses";


* Building Microsoft DirectShow base classes:
in VS2003 command prompt,
browse to "C:\Program Files\Microsoft Platform SDK" and run SetEnv.Cmd /XP32 /RETAIL

browse to "C:\Program Files\Microsoft Platform SDK\Samples\Multimedia\DirectShow\BaseClasses"
and run 'nmake' to build the libraries.

* The GraphEdit utility is located in the Microsoft Platform SDK\Bin directory.

  
