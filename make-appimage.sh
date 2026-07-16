#!/bin/sh

set -eu

ARCH=$(uname -m)
export ARCH
export OUTPATH=./dist
export ADD_HOOKS="self-updater.hook"
export UPINFO="gh-releases-zsync|${GITHUB_REPOSITORY%/*}|${GITHUB_REPOSITORY#*/}|latest|*$ARCH.AppImage.zsync"
export ICON=https://raw.githubusercontent.com/joyrider3774/blips/refs/heads/main/graphics/blips.png
export DESKTOP=DUMMY
export MAIN_BIN=Blips

# Deploy dependencies
quick-sharun ./Blips

# Additional changes can be done in between here
cp -r "fonts" "./AppDir/fonts"
cp -r "graphics" "./AppDir/graphics"
cp -r "levelpacks" "./AppDir/levelpacks"
cp -r "music" "./AppDir/music"
cp -r "sound" "./AppDir/sound"
cp ./README.md "./AppDir/"
cp ./LICENSE "./AppDir/"
		  
# Turn AppDir into AppImage
quick-sharun --make-appimage
