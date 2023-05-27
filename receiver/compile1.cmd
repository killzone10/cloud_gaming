@rem set PATH=%PATH%;bin

set INC1=include
set LIB1=lib

cl /c /std:c++20   /EHsc /ZI /I include decoder.cpp
cl /c /std:c++20 /EHsc /ZI /I include receiver.cpp
cl /c /std:c++20 /EHsc /ZI /I include socket.cpp
cl /c /std:c++20 /EHsc /ZI /I include packet.cpp
cl /c /std:c++20 /EHsc /ZI /I include FrameReceiver.cpp

cl /c /std:c++20 /EHsc /I %INC1%   receive.cpp 
cl  /DEBUG %LIB1%\avcodec.lib %LIB1%\swscale.lib %LIB1%\avutil.lib %LIB1%\avformat.lib shell32.lib %LIB1%\SDL2main.lib %LIB1%\SDL2.lib  receive.obj decoder.obj receiver.obj socket.obj packet.obj FrameReceiver.obj /link /SUBSYSTEM:CONSOLE


@rem decode_video.exe v.ES v
