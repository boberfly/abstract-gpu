#!/bin/bash

echo "Travis install script"
echo "TRAVIS_OS_NAME $TRAVIS_OS_NAME"

if test "$TRAVIS_OS_NAME" = "linux"; then
    echo "Updating APT"
    #sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
    sudo apt-get -qq update || exit 1

    echo "Installing 64 bits dependencies"
    #sudo apt-get -qqy install gcc-6-multilib g++-6-multilib libx11-xcb-dev || exit 1
    sudo apt-get -qqy install libx11-xcb-dev || exit 1

    echo "Downloading the Vulkan SDK"
    wget -Ovulkansdk-linux.run "http://ronie.cl/files/ci/vulkansdk-linux-x86_64-1.0.65.0.run" || exit 1

    echo "Extracting the Vulkan SDK"
    chmod +x ./vulkansdk-linux.run || exit 1
    ./vulkansdk-linux.run || exit 1
    mv -v VulkanSDK/1.0.65.0 thirdparty/vulkan-sdk || exit 1
fi
