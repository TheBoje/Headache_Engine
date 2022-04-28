# IFT-3100

## Setup

In order to work properly, our project relies on some community-made plugins. To install them,
please use the following commands :

- TheBoje/ofxImGui - `cd path/to/of/addons && git clone git@github.com:TheBoje/ofxImGui.git`
- kylemcdonald/ofxCv - `cd path/to/of/addons && git clone git@github.com:kylemcdonald/ofxCv.git`

In `Makefile` change the `OF_ROOT` at line 9 to your current openFrameworks location.

The package `voro++` (version >= 0.4.5) must be installed on the system.

## Compilation

In order to compile the project, run `make && make RunRelease` in the `Makefile` location. The setup part needs to be done before the compilation part.