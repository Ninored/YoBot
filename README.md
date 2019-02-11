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

### Build

To build the project use the following commands:

```
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" ../
msbuild /m:4 YoYoBot.sln
```