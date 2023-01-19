protoc -I=./ --cpp_out=./ ./Protocol.proto
IF ERRORLEVEL 1 PAUSE