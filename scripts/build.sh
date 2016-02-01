#!/bin/zsh

cd ..
mkdir -pv build
chmod 755 build
mkdir -pv temp
chmod 755 temp

echo "> begin eir compilation"
clang -c -D EIR_USE_OPENGL -D EIR_USE_SDL -D EIR_DEBUG -D EIR_LOG_MEMORY -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers -I/Library/Frameworks/SDL2_mixer.framework/Headers -O0 sources/eir_unity_osx.c -g -o temp/eir_unity_osx.o
clang temp/eir_unity_osx.o -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework OpenGL -F/Library/Frameworks -o build/Eir
echo "> eir compilation done"
