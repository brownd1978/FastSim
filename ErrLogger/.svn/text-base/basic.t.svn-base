This test validates the basic ErrLogger functionality.
--- run -------------------------------------------------------------------
testErrLogger
--- stdin -----------------------------------------------------------------
--- stdout ----------------------------------------------------------------
First, log with literal facility string, code integer
Now with code 0
Now log through string-code interface
logging test returns true
try using ErrMsg macro

test second call to testOp::print
pretext::friend operator<< called::posttext

emit two calls with severity trace
trace call 1
trace call 2

test buffer overflow
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123

After large buffer write
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123

After large buffer write
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123
123456789012345678901234567890123456789012345678901234567890123

After large buffer write
--- stderr ----------------------------------------------------------------
some text
 [interrupted by nested message from nextFacility]

 *** ErrLogger Warning *** 
The ErrStream was previously used without
endmsg being called to end the error message.
Please see ErrLogger/ErrStream.hh for more
information.

 Error Location/Facility: someFacility [code 703]
 *** ErrLogger Warning Finished ***

next text
some text
tracing through macro
friend operator<< called
---------------------------------------------------------------------------

This test sets the minimum error level to Error, so no output appears
--- run -------------------------------------------------------------------
testErrLogger2
--- stdin -----------------------------------------------------------------
--- stdout ----------------------------------------------------------------
First, log with literal facility string, code integer
Now with code 0
Now log through string-code interface
logging warning test returns false
logging error test returns true
try using ErrMsg macro

test second call to testOp::print

emit two calls with severity trace

test buffer for overflow
--- stderr ----------------------------------------------------------------
---------------------------------------------------------------------------

This test sets the minimum error level to Debugging, and shows line numbers
--- run -------------------------------------------------------------------
testErrLineLogger
--- stdin -----------------------------------------------------------------
--- stdout ----------------------------------------------------------------
First, log with literal facility string, code integer
Now with code 0
Now log through string-code interface
Now try using ErrMsg macro
Now see what happens with nested messages
ErrLineLogTest.cc(52):subsequent message at routine severity
--- stderr ----------------------------------------------------------------
someFacility [703]:some text
someFacility:some text
someFacility(code):some text
ErrLineLogTest.cc(48):tracing through macro
ErrLineLogTest.cc(51):incomplete message at error severity
 [interrupted by nested message from /afs/slac.stanford.edu/u/ec/gpdf/DataFlow/t1520-ErrLogger/ErrLogger/ErrLineLogTest.cc(52)]
ErrLog.cc(121):
 *** ErrLogger Warning *** 
The ErrStream was previously used without
endmsg being called to end the error message.
Please see ErrLogger/ErrStream.hh for more
information.

 Error Location/Facility: /afs/slac.stanford.edu/u/ec/gpdf/DataFlow/t1520-ErrLogger/ErrLogger/ErrLineLogTest.cc(51) [code 0]
 *** ErrLogger Warning Finished ***

ErrLineLogTest.cc(55):friend operator<< called
---------------------------------------------------------------------------
