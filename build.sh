#!/bin/bash
input=$1
if [ $1 ];then
	if [  "$input" = "all" ];then
		echo "Execute make all , willing to rebuild the project:"
		echo "Begin Clean Build Enviroment"
		rm -rf build
		mkdir build
		cd build

		echo "Begin execute cmake "
		cmake ../

		echo "Begin compile :"

		make -j8       
	else
		echo "Undefined params try ./build.sh all or ./build.sh"
	fi


else

	echo "Execute make base on previous build"

   	if [ ! -d "./build" ];then
   		echo "build dir is not exist, execute the make all cmd :"
       	mkdir  build
  	fi
 	cd  build
  	echo "Begin execute cmake " 
  	cmake ../

  	echo "Begin compile : "
   
  	make -j8

	
fi
