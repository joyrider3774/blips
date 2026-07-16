#!/bin/sh

set -eux

ARCH=$(uname -m)

export ICON=https://raw.githubusercontent.com/joyrider3774/blips/refs/heads/main/graphics/blips.png
export DESKTOP=DUMMY
export OUTPATH=./dist
export OUTNAME=Blips-"$ARCH".AppImage
export DEPLOY_OPENGL=1
export MAIN_BIN=Blips

echo "Installing package dependencies..."
echo "---------------------------------------------------------------"
pacman -Syu --noconfirm \
	base-devel       \
	cmake            \
	git              \
	libxcb           \
	libxcursor       \
	libxi            \
	libxkbcommon     \
	libxkbcommon-x11 \
	libxrandr        \
	libxtst          \
	libpng           \
	libxss           \
	patchelf         \
	fontconfig       \
	ttf-dejavu       \
	wget             \
	pulseaudio       \
	sdl3             \
	sdl3_mixer       \
	sdl3_image       \
	sdl3_ttf         \
	xorg-server-xvfb \
	zsync

echo "Installing debloated packages..."
echo "---------------------------------------------------------------"
get-debloated-pkgs --add-common --prefer-nano libdecor-mini

mkdir build
cd build
cmake ..
make
cp ./build/Blips ./Blips
