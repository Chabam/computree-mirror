@echo off

rem Get Doxygen (for HTML) here : https://www.doxygen.nl/download.html
rem Get Graphviz (for UML) here : http://www.graphviz.org/download/

rem Modify the paths as you have installed your programs so they can be found by this script
SET doxy=%UserProfile%\Desktop\Doxygen_1.8.20;
SET graphviz=%UserProfile%\Desktop\Graphviz\bin;

SET PATH=%doxy%%graphviz%%path%

cd %cd%
start doxygen.exe Doxyfile
exit
