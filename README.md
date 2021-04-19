# COMPUTREE

Computree is a processing platform for 3D points clouds in forestry. It is managed by the Computree group, composed of GIP ECOFOR, ONF, Arts et Métiers Paristech, IGN, INRA and Université de Sherbrooke.

## Installation

Download the package for your platform at [this address](http://rdinnovation.onf.fr/projects/computree/wiki/Fr_installation_v5).

Once downloaded, if you are using Windows, please run the script (double click):

```bash
win_setenv.cmd
```

## Compilation & development

You need to install [Qt package](https://download.qt.io/archive/qt/) including Qt creator and you favorite compiler.

Open the main project file

```bash
computree.pro
```

with Qt Creator, and then you can simply click on the 'play' symbol on bottom left to compile and run the program. Missing dependencies will be indicated. You shall install them as you want on your system, and indicate their directory in the files 

```bash
./config/check_*.pri
./config/default_path_*.pri
./config/include_necessary_*.pri
./config/include_dependencies.pri
```
If you want to compile the project in command line, please use:

```bash
cd computreev6
qmake computree.pro
make
make install
```

## Contributing
Please contact our team at contact@computree.onf.fr to get involved in Computree's evolutions.

## License
[GPL](http://www.gnu.org/licenses/gpl-3.0.html)