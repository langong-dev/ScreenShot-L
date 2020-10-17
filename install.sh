#/bin/bash

echo ' LanGong ScreenShot-L Install Script'



compile (){
  echo ' >> Compile'
  echo '    -> (0/1) Compile'
  qmake ScreenShot.pro
  make
  echo '    -> (1/1) Done!'
}

installto (){
  bin=$1
  dir=$2
  echo ' >> Install'
  echo '    -> (0/1) Install bin to "'$bin'"'
  cp ScreenShot "$bin/screenshotl"
  cp -r ./* "$dir/screenshotl"
  echo '    -> (1/1) Done!'
}

if [ $1 == 'dir' ]; then
  compile
  installto $2 $3
else
  compile
  installto /usr/bin /usr/share
fi
