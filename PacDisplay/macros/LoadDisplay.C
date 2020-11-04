void LoadDisplay (const char* filename=""){
  gROOT->ProcessLine(".include ../include/PacDisplay ");
  gROOT->ProcessLine(".include ../PacDisplay/include");
  gROOT->ProcessLine(".include ../include");
  gROOT->ProcessLine(".L ../PacDisplay/macros/PacDisplayMacro.C+");
  if(filename != "")
    PacDisplayMacro* display = new PacDisplayMacro(filename);
}
