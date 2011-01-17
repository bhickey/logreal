logreal.h
=========

About
-----
This software implements a log space numeric type.
Hopefully it should enable you to do a lot of multiplication and division
without over or underflowing.

Usage
-----
To use it in your software, just include 'logreal.h'

Contact
-------
Found a bug? Want to improve the code?
Contact Brendan Hickey <[bhickey.net](http://www.bhickey.net)>

Licensing
---------
This is free and unencumbered software released into the public domain.

See COPYING for details.

Issues
------
Attemping to compute 0.5^1073 fails with a value (internal representation) of zero.
