# Introduction

This is a modern build order optimiser for StarCraft 2.

## Cloning the repo

To clone the repo please use this command:

```
git clone --recursive https://github.com/Ninored/YoBot.git
```

## Windows


### Setup dependencies

This project require `s2client-api`, please execute the following commands to build it:

```
cd s2client-api
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" ../
msbuild /m:4 s2client-api.sln
```

You can replace the `msbuild /m:4 s2client-api.sln` command part by opening `s2client-api.sln` in Visual Studio 2017 and building it.

### Build the project

To build the project use the following commands:

```
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" ../
msbuild /m:4 YoYoBot.sln
```

### Using Visual Studio 2017

If you are using Visual Studio 2017, you have to clone the repo and Open a CMake Project.
Simply go to `File > Open > CMake...`

## Unit tests

Units tests are performed using [Catch2](https://github.com/catchorg/Catch2) Unit teste. 