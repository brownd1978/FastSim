/*
** ++
**  Package:
**	OdfCommon
**
**  Abstract:
**      odfTransitionId is a dataless class defining a class-local enum 
**      over the valid online state diagram transitions.
**
**      It is in a common online/offline package because the transitions'
**      identities are useful information offline.
**      
**  Author:
**      Michael Huffer, SLAC, (415) 926-4269
**
**  Creation Date:
**	000 - June 20 1,1997
**
**  Revisor initials:
**      gpdf - Gregory Dubois-Felsmann
**
**  Revision History:
**	001 - gpdf 1997.12.11
**            File moved from Odf/utility to OdfCommon.  Enum definition
**            changed to use explicit assignment of enumeration values.
**
** --
*/

#ifndef ODF_TRANSITIONID
#define ODF_TRANSITIONID

/*
** ++
**
**   WARNING!  The numerical values corresponding to particular transitions
**   MUST NEVER CHANGE during the lifetime of the experiment, as they will
**   be stored in the persistent data.
**
**   If transitions are deleted from the model, their numbers must never be
**   reused, and if new ones are added, they must be added at the end of the
**   list with higher values.
**
** --
*/

class odfTransitionId
  {
  public:
    enum Value
      {
      Unknown       =  0,  //  These transitions cannot be sent to
      Reset         =  1,  //  the segment-level else they will
      Shutdown      =  2,  //  look like front-panel markers.
      Map           =  3, 
      Unmap         =  4,
      Configure     =  5, 
      Unconfigure   =  6,
      BeginRun      =  7, 
      EndRun        =  8,
      BeginMeta     =  9, 
      EndMeta       = 10,
      Resume        = 11, 
      Pause         = 12,
      BeginMacro    = 13,
      EndMacro      = 14,
      BeginMajor    = 15, 
      EndMajor      = 16,
      BeginMinor    = 17, 
      EndMinor      = 18,
      Enable        = 19, 
      Disable       = 20,
      L1Accept      = 21, 
      Crash         = 22
      };

    /*
    ** The "numberof" constant must always be one more than the highest
    ** numerical value of a transition!
    */
    enum { numberof = 23 };
  };

#endif
