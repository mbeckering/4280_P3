Storage=Global for P4 (P3=Local, so statSem is picky about scope)
Working test files: All code generation files + included cahuna2.fs19 (edited for whitespace and global scope)
Test files that don't work: 
Global S1 Out: Echo 3 inputs in reverse (works if you add a space before the semicolon on line 5)
Global S2 Out: Echo 4 inputs in reverse (local statSem recognizes "x3" on line 10 is out of scope, works if that declaration is moved)
LOCAL S1, LOCAL S2 (P4 is not local)

Author: Michael Beckering
SSO MDBVXD
