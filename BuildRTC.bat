@echo このスクリプトを実行するには，cmakeおよびmsbuildの双方がPATHに入っている必要があります．
@echo cmakeのパスは，C:\Program Files (x86)\CMake 2.8\bin
@echo msbuildのパスは，C:\Windows\Microsoft.NET\Framework\v4.0.30319として処理をします．

@timeout 5

@call :BUILDW WebCamera


@timeout 10
@GOTO :EOF

:BUILDW
@if NOT EXIST build mkdir build
@cd build
@cmake ../
@msbuild %1.sln /p:Configuration=Release /p:Platform=WIN32
@if NOT EXIST components mkdir components
@copy .\src\Release\%1Comp.exe .\components\.
@copy ..\rtc.conf .\components\.
@exit /b