#!/bin/bash

# Install deps
sudo apt-get -qq update
sudo apt-get install -y libtool-bin cmake flex bison libgtk-3-dev libgl1-mesa-dev python3-numpy ocl-icd-opencl-dev
# Install CUDA (https://docs.nvidia.com/cuda/archive/10.1/cuda-installation-guide-linux/index.html)
sudo dpkg -i cuda-repo-ubuntu1804_10.1.243-1_amd64.deb
sudo apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/<distro>/<architecture>/7fa2af80.pub
sudo apt-get -q update
sudo apt-get install -y cuda-nvcc cuda-cudart cuda-cudart-dev cuda-nvrtc cuda-nvrtc-dev
export PATH=/usr/local/cuda-10.1/bin${PATH:+:${PATH}}
export LD_LIBRARY_PATH=/usr/local/cuda-10.1/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

# Get cl.hpp file
wget https://www.khronos.org/registry/OpenCL/api/2.1/cl.hpp
sudo cp cl.hpp /usr/include/CL/cl.hpp

# Clone LinuxCompile
git clone https://github.com/LuxCoreRender/LinuxCompile.git

# Set up correct names for release version and SDK
if [[ -z "$VERSION_STRING" ]] ; then
    VERSION_STRING=latest
fi

if [[ "$FINAL" == "TRUE" ]] ; then
    SDK_BUILD=-sdk
	# Required to link executables
	export LD_LIBRARY_PATH="`pwd`/LinuxCompile/target-64-sse2/lib:$LD_LIBRARY_PATH"
fi

# Set up paths
cd LinuxCompile

#==========================================================================
# Compiling OpenCL-less version"
#==========================================================================

# # Clone LuxCore (this is a bit a waste but LinuxCompile procedure
# # doesn't work with symbolic links)
# git clone .. LuxCore$SDK_BUILD
# ./build-64-sse2 LuxCore$SDK_BUILD
# cp target-64-sse2/LuxCore$SDK_BUILD.tar.bz2 target-64-sse2/luxcorerender-$VERSION_STRING-linux64$SDK_BUILD.tar.bz2
# mv target-64-sse2/LuxCore$SDK_BUILD.tar.bz2 $BUILD_ARTIFACTSTAGINGDIRECTORY/luxcorerender-$VERSION_STRING-linux64$SDK_BUILD.tar.bz2

# #==========================================================================
# # Compiling OpenCL version"
# #==========================================================================

# # Clone LuxCore (this is a bit a waste but LinuxCompile procedure
# # doesn't work with symbolic links)
# git clone .. LuxCore-opencl$SDK_BUILD
# ./build-64-sse2 LuxCore-opencl$SDK_BUILD 5
# cp target-64-sse2/LuxCore-opencl$SDK_BUILD.tar.bz2 target-64-sse2/luxcorerender-$VERSION_STRING-linux64-opencl$SDK_BUILD.tar.bz2
# mv target-64-sse2/LuxCore-opencl$SDK_BUILD.tar.bz2 $BUILD_ARTIFACTSTAGINGDIRECTORY/luxcorerender-$VERSION_STRING-linux64-opencl$SDK_BUILD.tar.bz2

#==========================================================================
# Compiling CUDA version"
#==========================================================================

# Clone LuxCore (this is a bit a waste but LinuxCompile procedure
# doesn't work with symbolic links)
git clone .. LuxCore-cuda$SDK_BUILD
./build-64-sse2 LuxCore-cuda$SDK_BUILD
cp target-64-sse2/LuxCore-cuda$SDK_BUILD.tar.bz2 target-64-sse2/luxcorerender-$VERSION_STRING-linux64-cuda$SDK_BUILD.tar.bz2
mv target-64-sse2/LuxCore-cuda$SDK_BUILD.tar.bz2 $BUILD_ARTIFACTSTAGINGDIRECTORY/luxcorerender-$VERSION_STRING-linux64-cuda$SDK_BUILD.tar.bz2

cd ..
