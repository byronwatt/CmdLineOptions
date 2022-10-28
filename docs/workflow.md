# workflow for windows

to avoid using work resources, here's how to compile on windows if you don't have visual studio:

## of course get code for windows,... but i don't think that comes with a compiler ?

[code.visualstudio.com](https://code.visualstudio.com/download)

## install ubuntu from the microsoft store

start -> microsoft store -> search for ubuntu

## install python, meson, ninja and bats which I think is:

```bash
sudo apt-get update 
sudo apt-get install python3
pip3 install meson ninja
sudo apt-get install bats
```
## clone the repo
```bash
git clone https://github.com/byronwatt/CmdLineOptions.git
cd CmdLineOptions
git submodule init
git submodule update
```
## configure
```bash
meson setup build
```
## compile
```bash
meson compile -C build
```
## run example
```bash
build/example --help
```
## run tests
```
bats test/test*.bats
```
## debugging
gdb is currently failing to run so need to figure out how to install a real development environment (and still pay nothing because I'm CHEAP)

# coverage

meson setup coverage -Db_coverage=true