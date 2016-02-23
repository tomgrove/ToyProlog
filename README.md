# ToyProlog

A very much work in progress implementation of Warren's Abstract Machine ( the WAM ). A WAM is VM designed to execute compiled Prolog.
( https://en.wikipedia.org/wiki/Prolog  )

It feels like something that would be useful as an embedded language; a search revelaed nothing entirely appropriate - lots of simple 
implementations in JS, C#, etc. but little in C/C++ save very complex systems. I would like:

* A machine independent core that executes a prolog-like language 
* That can be compiled or interpreted
* Has simple and explicit memory management

This Repo is probably not going to be it - more of a feasibility study to discover what such a system would look like. The implementation
is taken from "Warren's Abstract Machine: A Tutorial Reconstruction"  byHassan Aït-Kaci ( http://wambook.sourceforge.net/ ) 



